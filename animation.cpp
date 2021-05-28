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
#include "./matrix.h"
#include "./mapping.h"

// namespace MCAnim = MyAnimation;

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

void MyAnimation::menu__set_pixel(Print &out, char *command) {
    out.print(F("Set pixel "));
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
    tlc.set_pixel_16bit_value(index, value, value, value);
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
        tlc.show();
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

    out.println(F("  tlc.beginFast()"));
    tlc.beginFast();

    out.println(F("\t start with leds off"));
    tlc.setRGB();
    tlc.write();

    out.println(F("\t set leds to 0, 0, 1"));
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
        // tlc.set_pixel_all_16bit_value(21845, 21845, 21845);
        out.println(F("  Set all Pixel to red=blue=100."));
        tlc.set_pixel_all_16bit_value(100, 0, 100);
        tlc.show();

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

        effect_Matrix2D();

        // write data to chips
        tlc.show();
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
    tlc.set_pixel_all_16bit_value(0, 0, 0);
    tlc.set_pixel_16bit_value(step, 0, 0, 500);
}

void MyAnimation::effect__line() {
    uint8_t step = map_range_01_to(effect_position, 0, MATRIX_COL_COUNT);
    tlc.set_pixel_all_16bit_value(0, 0, 0);
    for (size_t row_index = 0; row_index < MATRIX_ROW_COUNT; row_index++) {
        tlc.set_pixel_16bit_value(pmap[row_index][step], 0, 0, 500);
    }
}

void MyAnimation::effect__rainbow() {
    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
            // full rainbow
            CHSV color_hsv = CHSV(effect_position, 1.0, brightness);
            CRGB color_rgb = hsv2rgb(color_hsv);
            tlc.set_pixel_float_value(
                pmap[row_i][col_i],
                color_rgb.r, color_rgb.g, color_rgb.b);
            // tlc.set_pixel_16bit_value(
            //     pmap[row_i][col_i],
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

CHSV MyAnimation::effect__sparkle(
    float col, float row, float offset
) {
    CHSV pixel_hsv = CHSV(0.5, 1.0, 0.0);
    return pixel_hsv;
}


CHSV MyAnimation::effect_Matrix2D_get_pixel(
    float col, float row, float offset
) {
    CHSV pixel_hsv = CHSV(0.5, 0.0, 1.0);

    // plasma
    CHSV plasma = effect__plasma(col, row, offset);
    pixel_hsv = plasma;

    // sparkle
    CHSV sparkle = effect__sparkle(
        col, row, offset);
    // not used currently..

    // TODO(s-light): develop 'layer' / 'multiplyer' system...

    return pixel_hsv;
}

void MyAnimation::effect_Matrix2D() {
    float offset = map_range_01_to(effect_position, 0.0, (PI * 30));
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
            tlc.set_pixel_float_value(
                pmap[row_i][col_i],
                pixel_rgb.r, pixel_rgb.g, pixel_rgb.b);
        }
    }
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
