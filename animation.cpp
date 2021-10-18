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

// include own headerfile
// NOLINTNEXTLINE(build/include)
#include "./animation.h"

#include "./easing.h"

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
        matrix.begin(out);
        animation_init(out);

        // enable
        ready = true;
    }
}

void MyAnimation::end() {
    if (ready) {
        // matrix.end();
    }
}

void MyAnimation::update() {
    if (ready) {
        animation_update();
        // matrix.update());
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

    if (index >= MATRIX_COL_COUNT) {
        index = MATRIX_COL_COUNT -1;
    }

    out.print(index);
    out.print(F(" to "));
    // uint16_t value = atoi(&command[command_offset]);
    uint16_t value = 2000;
    out.print(value);
    matrix.tlc.setRGB(index, value, value, value);
    out.println();
}

void MyAnimation::menu__set_pixel(Print &out, char *command) {
    out.print(F("Set pixel "));
    // p0,1:500
    // split string with help of tokenizer
    // https://www.cplusplus.com/reference/cstring/strtok/#
    char * command_pointer = &command[1];
    // NOLINTNEXTLINE(runtime/threadsafe_fn)
    command_pointer = strtok(command_pointer, " ,");
    uint8_t col_i = atoi(command_pointer);
    // NOLINTNEXTLINE(runtime/threadsafe_fn)
    command_pointer = strtok(NULL, " :");
    // command_pointer = strtok(&command[1], ",");
    uint8_t row_i = atoi(command_pointer);
    // NOLINTNEXTLINE(runtime/threadsafe_fn)
    command_pointer = strtok(NULL, " :");
    // uint16_t value = atoi(command_pointer);
    uint16_t value = 2000;

    if (col_i >= MATRIX_COL_COUNT) {
        col_i = MATRIX_COL_COUNT -1;
    }
    if (row_i >= MATRIX_ROW_COUNT) {
        row_i = MATRIX_ROW_COUNT -1;
    }

    uint16_t pixel_index = matrix.pmap[col_i][row_i];
    out.print(F(" ("));
    out.print(col_i);
    out.print(F(","));
    out.print(row_i);
    out.print(F("->"));
    out.print(pixel_index);
    out.print(F(")"));

    out.print(F(" to "));
    out.print(value);
    out.println();

    matrix.tlc.setRGB(pixel_index, value, value, value);
}

void MyAnimation::menu__set_all_pixel(Print &out, char *command) {
    out.print(F("Set all pixel to "));
    // uint16_t value = atoi(&command[1]);
    //
    // matrix.tlc.setRGB(value, value, value);
    // out.print(value);
    // out.println();

    uint16_t red = 0;
    uint16_t green = 0;
    uint16_t blue = 0;

    // Z65535,65535,65535
    // split string with help of tokenizer
    // https://www.cplusplus.com/reference/cstring/strtok/#
    char * command_pointer = &command[1];
    // NOLINTNEXTLINE(runtime/threadsafe_fn)
    command_pointer = strtok(command_pointer, " ,");
    red = atoi(command_pointer);
    // NOLINTNEXTLINE(runtime/threadsafe_fn)
    command_pointer = strtok(NULL, " ,");
    if (command_pointer == NULL) {
        green = red;
        blue = red;
    } else {
        green = atoi(command_pointer);
        // NOLINTNEXTLINE(runtime/threadsafe_fn)
        command_pointer = strtok(NULL, " ,");
        blue = atoi(command_pointer);
    }

    if (red > brightness_max_i) {
        red = brightness_max_i;
    }
    if (green > brightness_max_i) {
        green = brightness_max_i;
    }
    if (blue > brightness_max_i) {
        blue = brightness_max_i;
    }

    out.printf(" r:%5d, g:%5d, b:%5d\r\n", red, green, blue);
    matrix.tlc.setRGB(red, green, blue);
}

void MyAnimation::menu__time_meassurements(Print &out) {
    out.println(F("time_meassurements:"));


    uint32_t tm_total_start = 0;
    uint32_t tm_total_end = 0;
    uint32_t tm_total_duration = 0;

    uint32_t tm_start = 0;
    uint32_t tm_end = 0;
    uint32_t tm_duration = 0;
    uint32_t tm_loop_count = 10;



    out.print(F("effect_Matrix2D 1: "));
    tm_start = micros();
    effect_Matrix2D();
    tm_end = micros();
    tm_duration = (tm_end - tm_start);
    out.print(tm_duration);
    out.print(F("us / call"));
    out.println();

    out.print(F("matrix.tlc.write() 1:     "));
    tm_start = micros();
    matrix.tlc.write();
    tm_end = micros();
    tm_duration = (tm_end - tm_start);
    out.print(tm_duration);
    out.print(F("us / call"));
    out.println();



    out.print(F("effect_Matrix2D:   "));
    tm_total_start = millis();;
    tm_total_end = 0;
    tm_total_duration = 0;
    tm_start = 0;
    tm_end = 0;
    tm_duration = 0;
    tm_loop_count = 100;

    for (size_t i = 0; i < tm_loop_count; i++) {
        tm_start = millis();
        effect_Matrix2D();
        tm_end = millis();
        tm_duration += (tm_end - tm_start);
    }
    tm_total_end = millis();
    tm_total_duration += (tm_total_end - tm_total_start);

    out.print(tm_duration / static_cast<float>(tm_loop_count));
    out.print(F("ms / call"));
    out.println();
    // out.print(F("tm_duration: "));
    // out.print(tm_duration);
    // out.print(F("ms"));
    // out.println();
    // out.print(F("tm_total_duration: "));
    // out.print(tm_total_duration);
    // out.print(F("ms"));
    // out.println();


    out.print(F("matrix.tlc.write():       "));
    tm_start = 0;
    tm_end = 0;
    tm_duration = 0;
    tm_loop_count = 100;
    for (size_t i = 0; i < tm_loop_count; i++) {
        tm_start = micros();
        matrix.tlc.write();
        tm_end = micros();
        tm_duration += (tm_end - tm_start);
    }
    out.print((tm_duration / static_cast<float>(tm_loop_count)) / 1000);
    out.print(F("ms / call"));
    out.println();




    out.print(F("calculate_effect_position: "));

    tm_start = 0;
    tm_end = 0;
    tm_duration = 0;
    tm_loop_count = 100;
    for (size_t i = 0; i < tm_loop_count; i++) {
        tm_start = micros();
        calculate_effect_position();
        tm_end = micros();
        tm_duration += (tm_end - tm_start);
    }
    out.print((tm_duration / static_cast<float>(tm_loop_count)) / 1000);
    out.print(F("ms / call"));
    out.println();





    out.println(F("animation_update:"));

    tm_start = 0;
    tm_end = 0;
    tm_duration = 0;
    tm_loop_count = 100;

    tm_total_start = millis();;
    tm_total_end = 0;
    tm_total_duration = 0;

    bool animation_run_backup = animation_run;
    animation_run = true;
    effect_start = micros();
    effect_end = micros() + (1000*1000);  // 1sec

    for (size_t i = 0; i < tm_loop_count; i++) {
        tm_start = micros();
        animation_update();
        tm_end = micros();
        tm_duration += (tm_end - tm_start);
    }
    tm_total_end = millis();
    tm_total_duration += (tm_total_end - tm_total_start);

    out.print((tm_duration / static_cast<float>(tm_loop_count)) / 1000);
    out.print(F("ms / call"));
    out.println();
    out.print(F("tm_total_duration: "));
    out.print(tm_total_duration);
    out.print(F("ms"));
    out.println();

    animation_run = animation_run_backup;





    // out.println(F("10000 millis:"));
    //
    // tm_start = 0;
    // tm_end = 0;
    // tm_duration = 10000;
    // tm_loop_count = 0;
    //
    // tm_start = millis();
    // tm_end = tm_start + tm_duration;
    // while (millis() < tm_end) {
    //     // just wait
    //     tm_loop_count++;
    // }
    // tm_end = millis();
    // tm_duration = (tm_end - tm_start);
    //
    // out.print(tm_duration);
    // out.print(F("ms"));
    // out.println();
    // out.print(tm_loop_count);
    // out.print(F("calls"));
    // out.println();
    //
    // out.println();
    // out.println();
}


void MyAnimation::menu__set_hue(Print &out, char *command) {
    out.print(F("Set hue "));
    uint8_t command_offset = 1;
    float value = atof(&command[command_offset]);
    out.print(value, 4);
    hue = value;
    out.println();
}

void MyAnimation::menu__set_saturation(Print &out, char *command) {
    out.print(F("Set saturation "));
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
    out.print(" --> ");
    value = set_brightness(value);
    out.print(value, 5);
    out.println();
}



void MyAnimation::menu__test_colors(Print &out) {
    CHSV color1 = CHSV(0.1, 1.0, 1.0);
    CHSV color2 = CHSV(0.7, 1.0, 1.0);
    out.print("color1  : ");
    color1.print(out);
    out.println();
    out.print("color2  : ");
    color2.print(out);
    out.println();

    CHSV color_multi = (color1 * color2);
    out.print("color * : ");
    CHSV::print(out, color_multi);
    out.println();

    CHSV color_add = (color1 + color2);
    out.print("color + : ");
    CHSV::print(out, color_add);
    out.println();
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// brightness

float MyAnimation::set_brightness(float brightness_) {
    brightness = clamp(brightness_, static_cast<float>(0.0), brightness_max);
    // if (brightness_ <= 0) {
    //     brightness = 0;
    // }
    // if (brightness_ > brightness_max) {
    //     brightness = brightness_max;
    // } else {
    //     brightness = brightness_;
    // }
    return brightness;
}

float MyAnimation::set_hue(float hue_) {
    hue = clamp(hue_, static_cast<float>(0.0), static_cast<float>(1.0));
    return hue;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animation

void MyAnimation::animation_init(Stream &out) {
    out.println(F("init animation:")); {
        out.print(F("  effect_duration: "));
        out.print(effect_duration);
        out.println(F("ms"));

        // out.println(F("  Set all Pixel to 21845."));
        // matrix.tlc.setRGB(21845, 21845, 21845);
        out.println(F("  Set all Pixel to red=blue=100."));
        matrix.tlc.setRGB(200, 50, 0);
        matrix.tlc.write();


        effect_start = micros();
        effect_end = micros() + (effect_duration*1000);
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
        matrix.tlc.write();
        effect_loopcount++;
    }
}

void MyAnimation::calculate_effect_position() {
    effect_position = normalize_to_01(micros(), effect_start, effect_end);

    // effect_loopcount++;
    if (effect_position >  1.0) {
        effect_position = 0.0;
        // float duration_seconds = (millis() - effect_start) / 1000.0;
        uint32_t duration_us = micros() - effect_start;
        float duration_seconds =  duration_us / (1000.0 * 1000);
        float fps = effect_loopcount / duration_seconds;

        // if (animation_run) {
        //     Serial.print("millis():");
        //     Serial.print(millis());
        //     Serial.println();
        //     Serial.print("effect_start:");
        //     Serial.print(effect_start);
        //     Serial.println();
        //     Serial.print("duration_ms:");
        //     Serial.print(duration_ms);
        //     Serial.println();
        //     Serial.print("duration_seconds:");
        //     Serial.print(duration_seconds);
        //     Serial.println();
        //     Serial.print("effect_loopcount:");
        //     Serial.print(effect_loopcount);
        //     Serial.println();
        // }

        effect_loopcount = 0;
        effect_start = micros();
        effect_end = micros() + (effect_duration*1000);

        if (animation_run) {
            Serial.print("----- effect_position loop restart (");
            Serial.print(fps);
            Serial.print("FPS)");
            // Serial.print("   !!! millis() timming is off - ");
            // Serial.print("so this calculation is currently wrong !!!");
            Serial.println();
        }
    }
    // if (animation_run) {
    //     Serial.print("p:");
    //     Serial.println(effect_position);
    // }
}



void MyAnimation::effect__pixel_checker() {
    uint16_t step = map_range_01_to(
        effect_position, 0, MATRIX_PIXEL_COUNT);
    matrix.tlc.setRGB(0, 0, 0);
    matrix.tlc.setRGB(step, 0, 0, 500);
}

void MyAnimation::effect__line() {
    matrix.tlc.setRGB(0, 0, 0);
    uint16_t col_i_highlight = map_range_01_to(
        effect_position,
        0, MATRIX_COL_COUNT);
    uint16_t row_i_highlight = map_range_01_to(
        effect_position,
        0, MATRIX_ROW_COUNT);
    // for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
    //     matrix.tlc.setRGB(matrix.pmap[col_i][row_i], 0, 0, 500);
    // }
    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
            if (row_i == row_i_highlight) {
                matrix.tlc.setRGB(matrix.pmap[col_i][row_i], 0, 0, 1000);
            }
            if (col_i == col_i_highlight) {
                matrix.tlc.setRGB(matrix.pmap[col_i][row_i], 0, 1000, 0);
            }
        }
    }
}

void MyAnimation::effect__rainbow() {
    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
            // full rainbow
            CHSV color_hsv = CHSV(effect_position, 1.0, brightness);
            CRGB color_rgb = hsv2rgb(color_hsv);
            matrix.tlc.setRGB(
                matrix.pmap[col_i][row_i],
                // convert float to uint16_t
                color_rgb.r * 65535,
                color_rgb.g * 65535,
                color_rgb.b * 65535);
            // matrix.tlc.setRGB(
            //     matrix.pmap[col_i][row_i],
            //     0, col_i * step * 10 , row_i * 100);
        }
    }
}



CHSV MyAnimation::effect__plasma(float col, float row, float offset) {
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


float MyAnimation::calcRadius(float x, float y) {
  x *= x;
  y *= y;
  return sqrt(x + y);
}

// int16_t MyAnimation::calcDist(
//     uint8_t x, uint8_t y, int8_t center_x, int8_t center_y
// ) {
float MyAnimation::calcDist(float x, float y, float center_x, float center_y) {
  float a = center_y - y;
  float b = center_x - x;
  float dist = calcRadius(a, b);
  return dist;
}

CHSV MyAnimation::effect__wave(float col, float row, float offset) {
    // calculate wave
    // online
    // https://editor.soulmatelights.com/gallery/1015-circle
    float radius = 0.6;

    float dist = calcDist(col, row, 0.5, offset);
    // what does this next line actually does???
    dist = dist / radius;

    float value = 1.0;
    value = 1.0 * normalize_0n_to_10(dist, radius);
    // value = map_range(dist, 0.0, radius, 1.0, 0.0);
    value += offset * 3;
    // map to *full circle* in radians
    value = sin(map_range_01_to_0n(value, (360*PI/180)));

    // exclude / blur outside of circle
    value *= easeOut(1.0 - dist);
    if (dist > radius) {
        value = 0.0;
    }

    // map to color
    CHSV pixel_hsv = CHSV(0.2, 1.0, value);
    return pixel_hsv;
}

CHSV MyAnimation::effect__points(uint8_t col, uint8_t row, float offset) {
    // https://editor.soulmatelights.com/gallery/1450-points-int
    float value = 0.0;
    // uint16_t offset_row = offset * MATRIX_ROW_COUNT;
    uint16_t offset_col = offset * MATRIX_COL_COUNT;
    if (
        (row > (MATRIX_ROW_COUNT * 0.45) )
        && (row < (MATRIX_ROW_COUNT * (0.75)))
    ) {
        if (
          (col % 2)
          && (offset_col >= col)
        ) {
          value = 1.0;
        }
        // value_i = ((offset_col % MATRIX_COL_COUNT) - col + 1) * 255;
        // value_i = (col % 2) * 80;
        // value_i = (
        //     (col % 2)
        //     * ((offset_col % MATRIX_COL_COUNT) - col + 1)
        //     * 255);
    }
    // map to color
    CHSV pixel_hsv = CHSV(0.7, 1.0, value);
    return pixel_hsv;
}


CHSV MyAnimation::effect__mapping_checker(float col, float row, float offset) {
    // checker pattern
    CHSV pixel_hsv = CHSV(0.7, 1.0, 0.1);

    float row_width = (1.0 / MATRIX_ROW_COUNT / 1.5);
    float col_width = (1.0 / MATRIX_COL_COUNT / 1.5);
    // float base = col * 0.2 + offset;
    float offset_half = map_range(offset, 0.0, 1.0, -0.5, 0.5);
    // float base = map_range(col, -0.5, 0.5, 0.0, 1.0);
    // float base = map_range(col, -0.5, 0.5, 0.0, 1.0);
    // base = map_range(offset, 0, 1.0, 0.0, 0.1);
    // base *= 10.0;
    // // base *= 5.0;
    // float offset_PI = offset * (3.141592 / 2);
    // base += offset_PI;
    // // base += (offset*2);
    // Serial.printf("(%+2.2f|%+2.2f): %2.3f\r\n", col, row, base);
    // pixel_hsv.value = sin(base);
    // pixel_hsv.hue = sin(base);
    // pixel_hsv.value = base;

    // Serial.printf(
    //     "%+2.2f  "
    //     "%+2.2f|%+2.2f  "
    //     "%+2.2f|%+2.2f\r\n",
    //     offset,
    //     row, col,
    //     row_width, abs(offset - row));
    // Serial.printf(
    //     "%+2.2f  "
    //     "%+2.2f  "
    //     "%+2.2f  "
    //     "%+2.2f\r\n",
    //     offset_half,
    //     row,
    //     row_width,
    //     abs(offset_half - row));

    if (abs(offset_half - row) <= row_width) {
        pixel_hsv.hue = 0.1;
        pixel_hsv.value = 1.0;
    }
    if (abs(offset_half - col) <= col_width) {
        pixel_hsv.hue = 0.4;
        pixel_hsv.value = 1.0;
    }

    return pixel_hsv;
}

CHSV MyAnimation::effect__mapping_checker(
    uint16_t col, uint16_t row, float offset
) {
    // checker pattern
    // CHSV pixel_hsv = CHSV(offset, 1.0, 0.01);
    CHSV pixel_hsv = CHSV(0.7, 1.0, 1.0);
    uint8_t offset_row = offset * MATRIX_ROW_COUNT;
    uint8_t offset_col = offset * MATRIX_COL_COUNT;
    // Serial.printf(
    //     "%+2.2f:%02d  %02d|%02d\r\n",
    //     offset,
    //     offset_row,
    //     row,
    //     col);
    if (offset_row == row) {
        pixel_hsv.hue = 0.1;
        pixel_hsv.value = 1.0;
    }
    if (offset_col == col) {
        pixel_hsv.hue = 0.4;
        pixel_hsv.value = 1.0;
    }
    return pixel_hsv;
}

// NOLINT(legal/copyright)
CHSV MyAnimation::effect__sparkle(
    __attribute__((unused)) float col,
    __attribute__((unused)) float row,
    __attribute__((unused)) float offset
) {
    CHSV pixel_hsv = CHSV(0.5, 1.0, 1.0);
    return pixel_hsv;
}




// CHSV MyAnimation::effect_Matrix2D_get_pixel(
//     __attribute__((unused)) float col,
//     __attribute__((unused)) float row,
//     __attribute__((unused)) float offset
// ) {
CHSV MyAnimation::effect_Matrix2D_get_pixel(
    __attribute__((unused)) float col,
    __attribute__((unused)) float row,
    __attribute__((unused)) uint16_t col_i,
    __attribute__((unused)) uint16_t row_i,
    __attribute__((unused)) float offset
) {
    CHSV pixel_hsv = CHSV(hue, saturation, 1.0);

    // plasma
    // CHSV plasma = effect__plasma(col, row, offset);
    // pixel_hsv = plasma;

    // wave
    // pixel_hsv *= effect__wave(col, row, offset);
    pixel_hsv = effect__wave(col, row, offset);
    // pixel_hsv *= wave;

    // points
    // pixel_hsv *= effect__points(col_i, row_i, offset);
    CHSV pixel_points = effect__points(col_i, row_i, offset);
    // overlay
    if (pixel_points.value > 0.0) {
        pixel_hsv = pixel_points;
    }

    // sparkle
    // CHSV sparkle = effect__sparkle(col, row, offset);
    // pixel_hsv = sparkle;

    // pixel_hsv = effect__mapping_checker(col_i, row_i, offset);
    // pixel_hsv = effect__mapping_checker(col, row, offset);


    // TODO(s-light): develop 'layer' / 'multiplyer' system...
    // merge layers

    return pixel_hsv;
}

void MyAnimation::effect_Matrix2D() {
    // float offset = map_range_01_to(effect_position, 0.0, (PI * 30));
    // float offset = map_range_01_to(
    //     effect_position,
    //     0, MATRIX_ROW_COUNT);
    float offset = effect_position;

    // Serial.println("");

    // float offset_PI = map_range(offset, 0.0, 1.0, 0.0, (3.14/2));
    // PI 3,141592653589793
    // float offset_PI = offset * (3.141592 / 2);
    // Serial.printf(
    //     "%2.3f %2.3f "
    //     "%2.3f %2.3f \r\n",
    //     // offset,
    //     // sin(offset),
    //     offset_PI,
    //     sin(offset_PI)
    //     // NOLINTNEXTLINE(whitespace/parens)
    // );

    for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
        // normalize row
        // float row = map_range(
        //     row_i,
        //     0, MATRIX_ROW_COUNT-1,
        //     -0.5, 0.5);
        float row = normalize_0n_to_01(row_i, MATRIX_ROW_COUNT-1);
        for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
            // normalize col
            // float col = map_range__int2float(
            // float col = map_range(
            //     col_i,
            //     0, MATRIX_COL_COUNT-1,
            //     -0.5, 0.5);
            float col = normalize_0n_to_01(col_i, MATRIX_COL_COUNT-1);

            // ------------------------------------------
            CHSV pixel_hsv = effect_Matrix2D_get_pixel(
                col, row,
                col_i, row_i,
                offset);
            // CHSV pixel_hsv = effect_Matrix2D_get_pixel(col, row, offset);

            // ------------------------------------------
            // final conversions
            // global brightness
            pixel_hsv.value *= brightness;

            // CHSV pixel_hsv = CHSV(0.5, 0.0, 0.10);
            // convert to rgb
            CRGB pixel_rgb = hsv2rgb(pixel_hsv);
            // gamma & global brightness
            // fancyled.gamma_adjust(brightness=self.brightness);
            matrix.tlc.setRGB(
                matrix.pmap[col_i][row_i],
                // convert float to uint16_t
                pixel_rgb.r * 65535,
                pixel_rgb.g * 65535,
                pixel_rgb.b * 65535);
            // matrix.tlc.setRGB(
            //     matrix.pmap[col_i][row_i],
            //     10000,
            //     10000,
            //     0);
        }
    }
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
