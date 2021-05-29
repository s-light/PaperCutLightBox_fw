/******************************************************************************

    __doc__ = """
    animation.py - TLC5971 & FancyLED & 2D Array / Mapping.

    it combines the TLC5971 library with FancyLED and 2D Array / Mapping.

    Enjoy the colors :-)
    """

    libraries used:
        ~ slight_DebugMenu
        ~ slight_FaderLin
        ~ slight_TLC5957
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



#ifndef MyAnimation_H_
#define MyAnimation_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <slight_DebugMenu.h>

#include <SPI.h>
// #include <slight_TLC5957.h>
#include <Tlc59711.h>

#include "./color.h"
// #include "./matrix.h"
static const uint16_t CHIPS_COUNT__XX = 20*28*4;




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
    // static const uint8_t BOARDS_COL_COUNT = 1;
    // static const uint8_t BOARDS_ROW_COUNT = 2;
    static const uint8_t BOARDS_COUNT = BOARDS_COL_COUNT * BOARDS_ROW_COUNT;

    static const uint16_t CHIPS_COUNT = BOARDS_COUNT * LEDBOARD_CHIP_COUNT;

    static const uint8_t BOARDS_ORDER[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
        {34, 33, 32, 31, 30},
        {29, 28, 27, 26, 25},
        {24, 23, 22, 21, 20},
        {3, 7, 11, 15, 19},
        {2, 6, 10, 14, 18},
        {1, 5, 9, 13, 17},
        {0, 4, 8, 12, 16},
    };

    static const uint8_t BOARDS_ROTATION[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
        {2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2},
        {3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3},
    };

    // static const uint8_t BOARDS_ORDER[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    //     // {3},
    //     // {2},
    //     {1},
    //     {0},
    // };
    //
    // static const uint8_t BOARDS_ROTATION[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    //     // {3},
    //     // {3},
    //     {3},
    //     {3},
    // };

    static const uint8_t MATRIX_COL_COUNT = LEDBOARD_COL_COUNT * BOARDS_COL_COUNT;
    static const uint8_t MATRIX_ROW_COUNT = LEDBOARD_ROW_COUNT * BOARDS_ROW_COUNT;
    static const uint16_t MATRIX_PIXEL_COUNT = MATRIX_COL_COUNT * MATRIX_ROW_COUNT;



class MyAnimation {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // structs

    // typedef void (* tAnimationUpdatePixel) ();
    // typedef void (* tAnimationUpdatePosition) ();
    //
    // struct animation_t {
    //   const tAnimationUpdatePixel update_pixel;
    //   const tAnimationUpdatePosition update_position;
    //   const uint16_t mask;
    //   const uint16_t defaultv;
    // };




    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // matrix definitions
    // please extract this to own file / fix linking errors with matrix.h
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // pixel map
    // LEDBoard_4x4_16bit mapping

    uint16_t pmap[MATRIX_COL_COUNT][MATRIX_ROW_COUNT];

    uint16_t mymap_LEDBoard_4x4_16bit(uint8_t col, uint8_t row);

    void pmap_init();





    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TLC5957

    // possible options and defaults:
    // slight_TLC5957(
    //     uint8_t chip_count,
    //     uint8_t lat_pin = 7,
    //     uint8_t gclk_pin = 9,
    //     uint8_t sclk_pin = SCK,
    //     uint8_t sout_pin = MOSI,
    //     uint8_t sin_pin = MISO
    // );
    // use default pins
    // slight_TLC5957 tlc = slight_TLC5957(MATRIX_PIXEL_COUNT, 7);
    // Tlc59711 tlc(CHIPS_COUNT);
    // Tlc59711 tlc = Tlc59711(CHIPS_COUNT);
    Tlc59711 tlc = Tlc59711(CHIPS_COUNT__XX);


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    MyAnimation();
    ~MyAnimation();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    // basic library api
    void begin(Stream &out);
    void update();
    void end();

    // menu & helper
    void menu__set_pixel_index(Print &out, char *command);
    void menu__set_pixel(Print &out, char *command);
    void menu__test_buffer(Print &out);
    void menu__time_meassurements(Print &out);
    void print_pmap(Print &out);

    void menu__set_hue(Print &out, char *command);
    void menu__set_contrast(Print &out, char *command);
    void menu__set_brightness(Print &out, char *command);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations

    bool animation_run = true;

    uint16_t effect_duration = 1 * 1000; //ms

    // lounge blue - night
    // float hue = 0.7;
    // float contrast = 1.5;
    // float brightness = 0.01;

    // forest green - daylight
    float hue = 0.36;
    float contrast = 1.70;
    float brightness = 0.001;

    // const float PI = 3.141592;
    // is already defined by arduino or some other defaults...

private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // private functions

    // animation
    void animation_init(Stream &out);
    void animation_update();
    void calculate_effect_position();

    void effect__pixel_checker();
    void effect__line();
    void effect__rainbow();

    void effect_Matrix2D();
    CHSV effect_Matrix2D_get_pixel(float col, float row, float offset);
    CHSV effect__plasma(float col, float row, float offset);
    CHSV effect__sparkle(float col, float row, float offset);
    CHSV effect__mapping_checker(float col, float row, float offset);

    // others
    void tlc_init(Stream &out);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;

    uint32_t effect_start = 0;
    uint32_t effect_end = 0;
    uint32_t effect_loopcount = 0;
    // double effect_position = 0.0;
    float effect_position = 0.0;

};  // class MyAnimation

#endif  // MyAnimation_H_
