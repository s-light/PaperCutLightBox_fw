// NOLINT(legal/copyright)

/******************************************************************************

    __doc__ = """
    plasma :-)
    the old school way...
    Enjoy the colors :-)
    """

    written by stefan krueger (s-light),
        github@s-light.eu, http://s-light.eu, https://github.com/s-light/

******************************************************************************/
/******************************************************************************
The MIT License (MIT)

Copyright (c) 2023 Stefan Krüger

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
#include "./fx_points.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions
//
// // FXPointsþ::FXPoints(uint32_t duration_ = 5 * 1000) {
// FXPoints::FXPoints() {
//     // duration = duration_;
// }
//
// FXPoints::~FXPoints() {
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animation

CHSV FXPoints::get_pixel(__attribute__((unused)) PixelPos * pixel_pos) {
    // https://editor.soulmatelights.com/gallery/1450-points-int
    float value = 0.0;

    float points_f = map_range_0n_to_01(point_count, point_count_max);
    // uint16_t position_row = points_f * MATRIX_ROW_COUNT;
    uint16_t position_col = points_f * MATRIX_COL_COUNT;

    // TODO(s-light): implement usage of point_count

    if (
        (pixel_pos->row_i > (MATRIX_ROW_COUNT * row_start) )
        && (pixel_pos->row_i < (MATRIX_ROW_COUNT * row_end))
    ) {
        if (
          (pixel_pos->col_i % 2)
          && (position_col >= pixel_pos->col_i)
        ) {
          value = brightness;
          // add sinus overlay
          // value = sin(map_range_01_to_0n(value, (360*PI/180)));
          // shift black point
          // value = map_range_01_to(value, 0.5, 1.0);
        }
        // value_i =
        //     ((position_col % MATRIX_COL_COUNT) - pixel_pos->col + 1) * 255;
        // value_i = (pixel_pos->col % 2) * 80;
        // value_i = (
        //     (pixel_pos->col % 2)
        //     * ((position_col % MATRIX_COL_COUNT) - pixel_pos->col + 1)
        //     * 255);
    }

    // CHSV pixel_hsv = CHSV(hue, saturation, brightness);
    // map to color
    CHSV pixel_hsv = CHSV(hue, saturation, value);

    pixel_hsv.value *= visibility;
    return pixel_hsv;
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
