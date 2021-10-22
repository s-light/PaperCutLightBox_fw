// NOLINT(legal/copyright)

/******************************************************************************

    __doc__ = """
    simple line for mapping-check
    Enjoy the colors :-)
    """

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
#include "./fx_line.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions
//
// // FXLine::FXLine(uint32_t duration_ = 5 * 1000) {
// FXLine::FXLine() {
//     // duration = duration_;
// }
//
// FXLine::~FXLine() {
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animation

CHSV FXLine::get_pixel(
    __attribute__((unused)) float col,
    __attribute__((unused)) float row
) {
    CHSV pixel_hsv = CHSV(hue, saturation, brightness);

    // if row and col value -0.5 .. 0.5 then use this:
    // float row_width = (1.0 / MATRIX_ROW_COUNT / 1.5);
    // float col_width = (1.0 / MATRIX_COL_COUNT / 1.5);
    // float offset_half = map_range(position, 0.0, 1.0, -0.5, 0.5);

    // if row and col value 0.0 .. 1.0 then use this:
    const float row_width = (1.0 / MATRIX_ROW_COUNT);
    const float col_width = (1.0 / MATRIX_COL_COUNT);
    // float position_half = position;

    // float base = col * 0.2 + position;
    // float base = map_range(col, -0.5, 0.5, 0.0, 1.0);
    // float base = map_range(col, -0.5, 0.5, 0.0, 1.0);
    // base = map_range(position, 0, 1.0, 0.0, 0.1);
    // base *= 10.0;
    // // base *= 5.0;
    // float position_PI = position * (3.141592 / 2);
    // base += position_PI;
    // // base += (position*2);
    // Serial.printf("(%+2.2f|%+2.2f): %2.3f\r\n", col, row, base);
    // pixel_hsv.value = sin(base);
    // pixel_hsv.hue = sin(base);
    // pixel_hsv.value = base;

    // Serial.printf(
    //     "%+2.2f  "
    //     "%+2.2f|%+2.2f  "
    //     "%+2.2f|%+2.2f\r\n",
    //     position,
    //     row, col,
    //     row_width, abs(position - row));
    // Serial.printf(
    //     "%+2.2f  "
    //     "%+2.2f  "
    //     "%+2.2f  "
    //     "%+2.2f\r\n",
    //     position_half,
    //     row,
    //     row_width,
    //     abs(position_half - row));

    // if (abs(position_half - row) <= row_width) {
    if (abs(position - row) <= row_width) {
        pixel_hsv.hue = 0.1;
        pixel_hsv.value = 1.0;
    }
    // if (abs(position_half - col) <= col_width) {
    if (abs(position - col) <= col_width) {
        pixel_hsv.hue = 0.4;
        pixel_hsv.value = 1.0;
    }

    pixel_hsv.value *= visibility;
    return pixel_hsv;
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
