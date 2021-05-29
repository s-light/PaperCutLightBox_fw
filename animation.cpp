// NOLINT(legal/copyright)

/******************************************************************************

    __doc__ = """
    animation.py - TLC5971 & FancyLED & 2D Array / Mapping.

    it combines the TLC5971 library with FancyLED and 2D Array / Mapping.

    Enjoy the colors :-)
    """

    libraries used:
        ~ slight_DebugMenu
        ~ slight_FaderLin
            written by stefan krueger (s-light),
                github@s-light.eu, http://s-light.eu, https://github.com/s-light/
            license: MIT
        ~ Tlc59711.h
            License: MIT
            Copyright (c) 2016 Ulrich Stern
            https://github.com/ulrichstern/Tlc59711

    written by stefan krueger (s-light),
        github@s-light.eu, http://s-light.eu, https://github.com/s-light/

******************************************************************************/
/******************************************************************************
The MIT License (MIT)

Copyright (c) 2021 Stefan Krüger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/


// include Core Arduino functionality
#include <Arduino.h>

#include <slight_DebugMenu.h>
#include <slight_FaderLin.h>

#include <SPI.h>
// #include <slight_TLC5957.h>
#include <Tlc59711.h>

// include own headerfile
// NOLINTNEXTLINE(build/include)
#include "./animation.h"

#include "./color.h"
// #include "./matrix.h"
#include "./mapping.h"

// namespace MCAnim = MyAnimation;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// matrix definitions
// please extract this to own file / fix linking errors with matrix.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// pixel map
// LEDBoard_4x4_16bit mapping

static const uint16_t LEDBOARD_COL_COUNT = 4;
static const uint16_t LEDBOARD_ROW_COUNT = 4;
static const uint16_t LEDBOARD_PIXEL_COUNT = (
    LEDBOARD_ROW_COUNT * LEDBOARD_COL_COUNT);
static const uint16_t LEDBOARD_CHIP_COUNT = LEDBOARD_PIXEL_COUNT / 4;

static const uint8_t LEDBOARD_SINGLE
        [4][LEDBOARD_ROW_COUNT][LEDBOARD_COL_COUNT] = {
    // with all 4 rotations
    {
        // 0 =  0° → socket at bottom
        { 0,  1,  4,  5},
        { 2,  3,  6,  7},
        { 8,  9, 12, 13},
        {10, 11, 14, 15},
    },
    {
        // 1 = 90° → socket at left
        {10,  8,  2,  0},
        {11,  9,  3,  1},
        {14, 12,  6,  4},
        {15, 13,  7,  5},
    },
    {
        // 2 = 180° → socket at top
        {15, 14, 11, 10},
        {13, 12,  9,  8},
        { 7,  6,  3,  2},
        { 5,  4,  1,  0},
    },
    {
        // 3 = 270° → socket at right
        { 5,  7, 13, 15},
        { 4,  6, 12, 14},
        { 1,  3,  9, 11},
        { 0,  2,  8, 10},
    },
};

static const uint8_t BOARDS_COL_COUNT = 5;
static const uint8_t BOARDS_ROW_COUNT = 7;
static const uint8_t BOARDS_COUNT = BOARDS_COL_COUNT * BOARDS_ROW_COUNT;

static const uint16_t CHIPS_COUNT = BOARDS_COUNT * LEDBOARD_CHIP_COUNT;

static const uint8_t BOARDS_ORDER
        [BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    {30, 31, 32, 33, 34},
    {25, 26, 27, 28, 29},
    {20, 21, 22, 23, 24},
    {3, 7, 11, 15, 19},
    {2, 6, 10, 14, 18},
    {1, 5, 9, 13, 17},
    {0, 4, 8, 12, 16},
};

static const uint8_t BOARDS_ROTATION
        [BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    {2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
};

static const uint8_t MATRIX_COL_COUNT = LEDBOARD_COL_COUNT * BOARDS_COL_COUNT;
static const uint8_t MATRIX_ROW_COUNT = LEDBOARD_ROW_COUNT * BOARDS_ROW_COUNT;
static const uint16_t MATRIX_PIXEL_COUNT = MATRIX_COL_COUNT * MATRIX_ROW_COUNT;

uint16_t pmap[MATRIX_COL_COUNT][MATRIX_ROW_COUNT];

uint16_t mymap_LEDBoard_4x4_16bit(uint8_t col, uint8_t row) {
    // """Map row and col to pixel_index."""
    // get Board position
    uint8_t board_col = col / LEDBOARD_COL_COUNT;
    uint8_t board_row = row / LEDBOARD_ROW_COUNT;
    uint8_t board_sub_col = col % LEDBOARD_COL_COUNT;
    uint8_t board_sub_row = row % LEDBOARD_ROW_COUNT;

    uint8_t board_offset = BOARDS_ORDER[board_row][board_col];
    uint8_t board_rotation = BOARDS_ROTATION[board_row][board_col];
    uint8_t pixel_offset =
    LEDBOARD_SINGLE[board_rotation][board_sub_row][board_sub_col];

    uint8_t pixel_index = (pixel_offset * BOARDS_COUNT) + board_offset;

    return pixel_index;
}

void pmap_init() {
    // """Prepare Static Map."""
    for (size_t row_index = 0; row_index < MATRIX_ROW_COUNT; row_index++) {
        for (size_t col_index = 0; col_index < MATRIX_COL_COUNT; col_index++) {
            pmap[col_index][row_index] =
                // mymap_LEDBoard_4x4_HD(
                // mymap_LEDBoard_4x4_HD_CrystalLightGuide(
                mymap_LEDBoard_4x4_16bit(
                    col_index, row_index);
        }
    }
}







// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions


MyAnimation::MyAnimation() {
    ready = false;
}

MyAnimation::~MyAnimation() {
    end();
}

void MyAnimation::begin(Stream &out) {
    // clean up..
    end();
    // start up...
    if (ready == false) {
        // setup
        // pmap_init();
        tlc_init(out);
        animation_init(out);

        // enable
        ready = true;
    }
}

void MyAnimation::end() {
    if (ready) {
        // nothing to do..
    }
}

void MyAnimation::update() {
    if (ready) {
        animation_update();
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// menu

void MyAnimation::menu__test_buffer(Print &out) {
    out.println(F("SetBuffer:"));
    out.println(F("--- old"));

    out.println();
}

void MyAnimation::menu__set_pixel_index(Print &out, char *command) {
    out.print(F("Set pixel index "));
    uint8_t command_offset = 1;
    uint8_t index = atoi(&command[command_offset]);
    // a better way than this would be to search for the ':'
    // i have used this a long time ago for MAC address format parsing
    // was something with 'tokenize' or similar..
    command_offset = 3;
    if (index > 9) {
        command_offset = command_offset +1;
    }
    out.print(index);
    out.print(F(" to "));
    uint16_t value = atoi(&command[command_offset]);
    out.print(value);
    tlc.setRGB(index, value, value, value);
    out.println();
}

void MyAnimation::menu__set_pixel(Print &out, char *command) {
    out.print(F("Set pixel "));
    // p0,1:500
    // split string with help of tokenizer
    // https://www.cplusplus.com/reference/cstring/strtok/#
    char * command_pointer = &command[1];
    // command_pointer = strtok(command_pointer, ":");
    uint8_t col_i = atoi(command_pointer);
    // command_pointer = strtok(NULL, ",");
    command_pointer = strtok(&command[1], ",");
    uint8_t row_i = atoi(command_pointer);
    command_pointer = strtok(NULL, " :");
    uint8_t value = atoi(command_pointer);

    out.print(F(" ("));
    out.print(col_i);
    out.print(F(","));
    out.print(row_i);
    out.print(F(")"));

    out.print(F(" to "));
    out.print(value);
    tlc.setRGB(pmap[col_i][row_i], value, value, value);
    out.println();
}

void MyAnimation::menu__time_meassurements(Print &out) {
    out.println(F("time_meassurements:"));

    uint32_t tm_start = 0;
    uint32_t tm_end = 0;
    uint32_t tm_duration = 0;
    uint32_t tm_loop_count = 10;

    for (size_t i = 0; i < tm_loop_count; i++) {
        tm_start = millis();
        effect_Matrix2D();
        tlc.write();
        tm_end = millis();
        tm_duration += (tm_end - tm_start);
    }

    out.print(tm_duration / static_cast<float>(tm_loop_count));
    out.print(F("ms / call"));
    out.println();
}


void MyAnimation::menu__set_hue(Print &out, char *command) {
    out.print(F("Set hue "));
    uint8_t command_offset = 1;
    float value = atof(&command[command_offset]);
    out.print(value, 4);
    hue = value;
    out.println();
}

void MyAnimation::menu__set_contrast(Print &out, char *command) {
    out.print(F("Set contrast "));
    uint8_t command_offset = 1;
    float value = atof(&command[command_offset]);
    out.print(value, 4);
    contrast = value;
    out.println();
}

void MyAnimation::menu__set_brightness(Print &out, char *command) {
    out.print(F("Set brightness "));
    uint8_t command_offset = 1;
    float value = atof(&command[command_offset]);
    out.print(value, 5);
    brightness = value;
    out.println();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TLC5957 / LED-Driver

void MyAnimation::tlc_init(Stream &out) {
    out.println(F("setup tlc:"));

    out.println(F("    tlc.beginFast()"));
    tlc.beginFast();

    // tlc.setBrightness(127, 127, 127);

    out.println(F("    start with leds off"));
    tlc.setRGB();
    tlc.write();

    out.println(F("    set leds to 0, 0, 1"));
    tlc.setRGB(0, 0, 1);
    tlc.write();


    // out.println(F("  set spi_baudrate"));
    // 2MHz
    // tlc.spi_baudrate = 2.0 * 1000 * 1000;
    // 0.001MHz = 1000kHz
    // tlc.spi_baudrate = 0.001 * 1000 * 1000;


    // out.print(F("  tlc.pixel_count: "));
    // out.print(tlc.pixel_count);
    // out.println();
    // out.print(F("  tlc.chip_count: "));
    // out.print(tlc.chip_count);
    // out.println();
    // out.print(F("  tlc.buffer_byte_count: "));
    // out.print(tlc.buffer_byte_count);
    // out.println();
    // out.print(F("  tlc.spi_baudrate: "));
    // out.print(tlc.spi_baudrate);
    // out.print(F("Hz"));
    // out.print(F("  = "));
    // out.print(tlc.spi_baudrate / 1000.0, 4);
    // out.print(F("kHz"));
    // out.print(F("  = "));
    // out.print(tlc.spi_baudrate / (1000.0 * 1000.0), 4);
    // out.println(F("MHz"));
    //
    // out.print(F("  tlc.get_fc_ESPWM(): "));
    // out.print(tlc.get_fc_ESPWM());
    out.println();
    out.println(F("  finished."));
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animation

void MyAnimation::animation_init(Stream &out) {
    out.println(F("init animation:")); {
        out.print(F("  effect_duration: "));
        out.print(effect_duration);
        out.println(F("ms"));

        // out.println(F("  Set all Pixel to 21845."));
        // tlc.setRGB(21845, 21845, 21845);
        out.println(F("  Set all Pixel to red=blue=100."));
        tlc.setRGB(100, 0, 100);
        tlc.write();

        effect_start = millis();
        effect_end = millis() + effect_duration;
    }
    out.println(F("  finished."));
}

void MyAnimation::animation_update() {
    calculate_effect_position();
    if (animation_run) {
        // effect__pixel_checker();
        // effect__line();
        // effect__rainbow();

        // effect_Matrix2D();

        // write data to chips
        tlc.write();
    }
}

void MyAnimation::calculate_effect_position() {
    effect_position = normalize_to_01(millis(), effect_start, effect_end);
    effect_loopcount++;
    if (effect_position >  1.0) {
        effect_position = 0;
        float duration_seconds = (millis() - effect_start) / 1000.0;
        float fps = effect_loopcount / duration_seconds;
        effect_loopcount = 0;
        effect_start = millis();
        effect_end = millis() + effect_duration;
        if (animation_run) {
            Serial.print("effect_position loop restart. (");
            Serial.print(fps);
            Serial.print("FPS)");
            Serial.println();
        }
    }
}



void MyAnimation::effect__pixel_checker() {
    uint8_t step = map_range_01_to(
        effect_position, 0, MATRIX_PIXEL_COUNT);
    tlc.setRGB(0, 0, 0);
    tlc.setRGB(step, 0, 0, 500);
}

void MyAnimation::effect__line() {
    uint16_t col_i = map_range_01_to(effect_position, 0, MATRIX_COL_COUNT);
    tlc.setRGB(0, 0, 0);
    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        tlc.setRGB(pmap[col_i][row_i], 0, 0, 500);
    }
}

void MyAnimation::effect__rainbow() {
    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
            // full rainbow
            CHSV color_hsv = CHSV(effect_position, 1.0, brightness);
            CRGB color_rgb = hsv2rgb(color_hsv);
            tlc.setRGB(
                pmap[col_i][row_i],
                // convert float to uint16_t
                color_rgb.r * 65535,
                color_rgb.g * 65535,
                color_rgb.b * 65535);
            // tlc.setRGB(
            //     pmap[col_i][row_i],
            //     0, col_i * step * 10 , row_i * 100);
        }
    }
}



CHSV MyAnimation::effect__plasma(
    float col, float row, float offset
) {
    // calculate plasma
    // mostly inspired by
    // https://www.bidouille.org/prog/plasma
    // moving rings
    float cx = col + 0.5 * sin(offset / 5);
    float cy = row + 0.5 * cos(offset / 3);
    // double xy_value = sin(
    float xy_value = sin(
        sqrt(100 * (cx*cx + cy*cy) + 1)
        + offset);
    // mapping
    float pixel_hue = map_range(
        xy_value,
        -1.0, 1.0,
        // self._hue_min, self._hue_max
        // 0.0, 0.08
        hue - 0.05, hue + 0.2);
    float pixel_saturation = map_range(
        xy_value,
        -1.0, 1.0,
        1.0, 1.0);
    float pixel_value = map_range(
        xy_value,
        1.0, -1.0,
        // self._contrast_min, self._contrast_max
        // -0.005, 1.0
        1.0 - contrast, 1.0);
    // map to color
    CHSV pixel_hsv = CHSV(pixel_hue, pixel_saturation, pixel_value);
    return pixel_hsv;
}



CHSV MyAnimation::effect__mapping_checker(
    float col, float row, float offset
) {
    // calculate plasma
    // mostly inspired by
    // https://www.bidouille.org/prog/plasma
    // moving rings
    float cx = col + 0.5 * sin(offset / 5);
    float cy = row + 0.5 * cos(offset / 3);
    // double xy_value = sin(
    float xy_value = sin(
        sqrt(100 * (cx*cx + cy*cy) + 1)
        + offset);
    // mapping
    float pixel_hue = map_range(
        xy_value,
        -1.0, 1.0,
        // self._hue_min, self._hue_max
        // 0.0, 0.08
        hue - 0.05, hue + 0.2);
    float pixel_saturation = map_range(
        xy_value,
        -1.0, 1.0,
        1.0, 1.0);
    float pixel_value = map_range(
        xy_value,
        1.0, -1.0,
        // self._contrast_min, self._contrast_max
        // -0.005, 1.0
        1.0 - contrast, 1.0);
    // map to color
    CHSV pixel_hsv = CHSV(pixel_hue, pixel_saturation, pixel_value);
    return pixel_hsv;
}

// NOLINT(legal/copyright)
CHSV MyAnimation::effect__sparkle(
    __attribute__((unused)) float col,
    __attribute__((unused)) float row,
    __attribute__((unused)) float offset
) {
    CHSV pixel_hsv = CHSV(0.5, 1.0, 0.0);
    return pixel_hsv;
}


CHSV MyAnimation::effect_Matrix2D_get_pixel(
    float col, float row, float offset
) {
    CHSV pixel_hsv = CHSV(0.5, 0.0, 1.0);

    // plasma
    // CHSV plasma = effect__plasma(col, row, offset);
    // pixel_hsv = plasma;

    CHSV mapping_checker = effect__mapping_checker(col, row, offset);
    pixel_hsv = mapping_checker;

    // sparkle
    // CHSV sparkle = effect__sparkle(
    //     col, row, offset);
    // not used currently..

    // TODO(s-light): develop 'layer' / 'multiplyer' system...

    return pixel_hsv;
}

void MyAnimation::effect_Matrix2D() {
    // float offset = map_range_01_to(effect_position, 0.0, (PI * 30));
    float offset = map_range_01_to(effect_position, 0, MATRIX_ROW_COUNT);

    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        // normalize row
        // float row = map_range__int2float(
        float row = map_range(
            row_i,
            0, MATRIX_ROW_COUNT-1,
            -0.5, 0.5);
        for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
            // normalize col
            // float col = map_range__int2float(
            float col = map_range(
                col_i,
                0, MATRIX_COL_COUNT-1,
                -0.5, 0.5);

            // ------------------------------------------
            CHSV pixel_hsv = effect_Matrix2D_get_pixel(col, row, offset);

            // ------------------------------------------
            // final conversions
            // global brightness
            pixel_hsv.value *= brightness;
            // convert to rgb
            CRGB pixel_rgb = hsv2rgb(pixel_hsv);
            // gamma & global brightness
            // fancyled.gamma_adjust(brightness=self.brightness);
            tlc.setRGB(
                pmap[col_i][row_i],
                // convert float to uint16_t
                pixel_rgb.r * 65535,
                pixel_rgb.g * 65535,
                pixel_rgb.b * 65535);
        }
    }
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
