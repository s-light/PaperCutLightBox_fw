// NOLINT(legal/copyright)

/******************************************************************************

    matrix,cpp
        all things led matrix related.

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



// include Core Arduino functionality
#include <Arduino.h>

#include <slight_DebugMenu.h>

// include own headerfile
#include "./matrix.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions

MyMatrix::MyMatrix() {
    // ready = false;
    pmap_init();
}

MyMatrix::~MyMatrix() {
    // end();
}

// void MyMatrix::begin(Stream &out) {
//     // clean up..
//     end();
//     // start up...
//     if (ready == false) {
//         // setup
//
//         // enable
//         ready = true;
//     }
// }
//
// void MyMatrix::end() {
//     if (ready) {
//         // nothing to do..
//     }
// }
//
// void MyMatrix::update() {
//     if (ready) {
//         // do it :-)
//     }
// }


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// pixel map

// Horizontal
// const uint8_t LEDBOARD_SINGLE
//         [LEDBOARD_ROW_COUNT][LEDBOARD_COL_COUNT] = {
//     {15, 14, 13, 12},
//     {11, 10, 9, 8},
//     {7, 6, 5, 4},
//     {3, 2, 1, 0},
// }

// // Vertical
// const uint8_t MyMatrix::LEDBOARD_SINGLE
//         [LEDBOARD_ROW_COUNT][LEDBOARD_COL_COUNT] = {
//     {3, 7, 11, 15},
//     {2, 6, 10, 14},
//     {1, 5, 9, 13},
//     {0, 4, 8, 12},
// };

const uint8_t MyMatrix::LEDBOARD_SINGLE
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

const uint8_t MyMatrix::BOARDS_ORDER
        [BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    {30, 31, 32, 33, 34},
    {25, 26, 27, 28, 29},
    {20, 21, 22, 23, 24},
    {3, 7, 11, 15, 19},
    {2, 6, 10, 14, 18},
    {1, 5, 9, 13, 17},
    {0, 4, 8, 12, 16},
};

const uint8_t MyMatrix::BOARDS_ROTATION
        [BOARDS_ROW_COUNT][BOARDS_COL_COUNT] = {
    {2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3},
};


void MyMatrix::pmap_init() {
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

uint8_t MyMatrix::mymap_LEDBoard_4x4_16bit(
    uint8_t col, uint8_t row
) {
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

// uint8_t MyMatrix::mymap_LEDBoard_4x4_HD(
//     uint8_t col, uint8_t row
// ) {
//     // """Map row and col to pixel_index."""
//     // get Board position
//     uint8_t board_col = col / LEDBOARD_COL_COUNT;
//     uint8_t board_row = row / LEDBOARD_ROW_COUNT;
//     uint8_t board_sub_col = col % LEDBOARD_COL_COUNT;
//     uint8_t board_sub_row = row % LEDBOARD_ROW_COUNT;
//
//     uint8_t board_offset = BOARDS_ORDER[board_row][board_col];
//     uint8_t pixel_offset = LEDBOARD_SINGLE[board_sub_row][board_sub_col];
//
//     uint8_t pixel_index = (pixel_offset * BOARDS_COUNT) + board_offset;
//
//     return pixel_index;
// }
//
// uint8_t MyMatrix::mymap_LEDBoard_4x4_HD_CrystalLightGuide(
//     uint8_t col, uint8_t row
// ) {
//     // """Map row and col to pixel_index."""
//     // get Board position
//     uint8_t board_col = col / LEDBOARD_COL_COUNT;
//     uint8_t board_row = row / LEDBOARD_ROW_COUNT;
//     // uint8_t board_sub_col = col % LEDBOARD_COL_COUNT;
//     // uint8_t board_sub_row = row % LEDBOARD_ROW_COUNT;
//
//     uint8_t board_offset = BOARDS_ORDER[board_row][board_col];
//     // pixel_offset = LEDBOARD_SINGLE[board_sub_row][board_sub_col];
//
//     uint8_t pixel_offset = LEDBOARDS_ROTATED[row][col];
//     // print("{:>3} {:>3}: {:>3}".format(col, row, pixel_offset));
//     uint8_t pixel_index = (pixel_offset * BOARDS_COUNT) + board_offset;
//
//     return pixel_index;
// }



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
