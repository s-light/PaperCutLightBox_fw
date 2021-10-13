/******************************************************************************

    matrix.h
        all things led matrix related

    libraries used:
        ~

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



#ifndef ledmatrix_H_
#define ledmatrix_H_

// include Core Arduino functionality
#include <Arduino.h>


#include <SPI.h>
// #include <slight_TLC5957.h>
#include <Tlc59711.h>


// used for print_uint16_align_right
#include <slight_DebugMenu.h>

#include "./mapping.h"

class MyLEDMatrix {
public:

    static constexpr uint16_t LEDBOARD_COL_COUNT = 4;
    static constexpr uint16_t LEDBOARD_ROW_COUNT = 4;
    static constexpr uint16_t LEDBOARD_PIXEL_COUNT = (
        LEDBOARD_ROW_COUNT * LEDBOARD_COL_COUNT);
    static constexpr uint16_t LEDBOARD_CHIP_COUNT = LEDBOARD_PIXEL_COUNT / 4;

    static constexpr uint8_t LEDBOARD_SINGLE
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

    // static constexpr uint8_t BOARDS_COL_COUNT = 5;
    // static constexpr uint8_t BOARDS_ROW_COUNT = 7;
    static constexpr uint8_t BOARDS_COL_COUNT = 7;
    static constexpr uint8_t BOARDS_ROW_COUNT = 3;
    // static constexpr uint8_t BOARDS_COL_COUNT = 1;
    // static constexpr uint8_t BOARDS_ROW_COUNT = 1;
    static constexpr uint8_t BOARDS_COUNT = BOARDS_COL_COUNT * BOARDS_ROW_COUNT;

    static constexpr uint16_t CHIPS_COUNT = BOARDS_COUNT * LEDBOARD_CHIP_COUNT;

    // static constexpr uint8_t BOARDS_ORDER[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    //     {34, 33, 32, 31, 30},
    //     {29, 28, 27, 26, 25},
    //     {24, 23, 22, 21, 20},
    //     {3, 7, 11, 15, 19},
    //     {2, 6, 10, 14, 18},
    //     {1, 5, 9, 13, 17},
    //     {0, 4, 8, 12, 16},
    // };
    //
    // static constexpr uint8_t BOARDS_ROTATION[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    //     {2, 2, 2, 2, 2},
    //     {2, 2, 2, 2, 2},
    //     {2, 2, 2, 2, 2},
    //     {3, 3, 3, 3, 3},
    //     {3, 3, 3, 3, 3},
    //     {3, 3, 3, 3, 3},
    //     {3, 3, 3, 3, 3},
    // };

    static constexpr uint8_t BOARDS_ORDER[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
        {20, 19, 18, 17, 16, 15, 14},
        {13, 12,  9,  8,  7,  3,  2},
        {11, 10,  6,  5,  4,  1,  0},
    };

    // 0 =  0° → socket at bottom
    // 1 = 90° → socket at left
    // 2 = 180° → socket at top
    // 3 = 270° → socket at right
    static constexpr uint8_t BOARDS_ROTATION[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
        {2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2},
    };

    // static constexpr uint8_t BOARDS_ORDER[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    //     // {3},
    //     // {2},
    //     // {1},
    //     {0},
    // };
    //
    // static constexpr uint8_t BOARDS_ROTATION[BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    //     // {3},
    //     // {3},
    //     // {3},
    //     {3},
    // };

    static constexpr uint8_t MATRIX_COL_COUNT = LEDBOARD_COL_COUNT * BOARDS_COL_COUNT;
    static constexpr uint8_t MATRIX_ROW_COUNT = LEDBOARD_ROW_COUNT * BOARDS_ROW_COUNT;
    static constexpr uint16_t MATRIX_PIXEL_COUNT = MATRIX_COL_COUNT * MATRIX_ROW_COUNT;


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
    Tlc59711 tlc = Tlc59711(CHIPS_COUNT);


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    MyLEDMatrix();
    ~MyLEDMatrix();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    // basic library api
    void begin(Stream &out);
    void update();
    void end();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // pixel map
    // LEDBoard_4x4_16bit mapping

    uint16_t pmap[MATRIX_COL_COUNT][MATRIX_ROW_COUNT];

    void print_pmap(Print &out);
    void print_2Dmatrix(Print &out);

private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // private functions
    void tlc_init(Stream &out);

    void pmap_init();
    uint16_t mymap_LEDBoard_4x4_16bit(uint8_t col, uint8_t row);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;

};  // class MyLEDMatrix

#endif  // ledmatrix_H_
