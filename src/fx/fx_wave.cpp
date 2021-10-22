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
#include "./fx_wave.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions
//
// // FXWaveþ::FXWave(uint32_t duration_ = 5 * 1000) {
// FXWave::FXWave() {
//     // duration = duration_;
// }
//
// FXWave::~FXWave() {
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// animation


float FXWave::calcRadius(float x, float y) {
  x *= x;
  y *= y;
  return sqrt(x + y);
}

// int16_t FXWave::calcDist(
//     uint8_t x, uint8_t y, int8_t center_x, int8_t center_y
// ) {
float FXWave::calcDist(float x, float y, float center_x, float center_y) {
  float a = center_y - y;
  float b = center_x - x;
  float dist = calcRadius(a, b);
  return dist;
}


CHSV FXWave::get_pixel(__attribute__((unused)) PixelPos * pixel_pos) {
    // calculate wave
    // online
    // https://editor.soulmatelights.com/gallery/1015-circle
    float radius = 0.7;
    float position_y = map_range_01_to(
        position,
        -(radius-0.02),
        1.0 + (radius-0.2));

    float dist = calcDist(pixel_pos->col, pixel_pos->row, 0.5, position_y);
    // float dist = calcDist(pixel_pos->col, pixel_pos->row, 0.5, 0.9);
    // what does this next line actually does???
    dist = dist / radius;

    float value = brightness;
    value = 1.0 * map_range_0n_to_10(dist, radius);
    // value = map_range(dist, 0.0, radius, 1.0, 0.0);
    value += position * 3.5;
    // value += position;
    // map to *full circle* in radians
    value = sin(map_range_01_to_0n(value, (360*PI/180)));
    // shift black point
    value = map_range_01_to(value, 0.5, 1.0);
    // exclude / blur outside of circle
    float mask = 1.0;
    // mask *= easeIn_double(dist);
    mask *= easeIn(map_range_0n_to_10(dist*1.0, radius*2.0));
    // mask *= easeIn(map_range_0n_to_10(dist*1.0, radius*1.0));
    // mask *= easeIn((1.0 - dist)*1.4);
    // mask *= easeInExpo(1.0 * map_range_0n_to_10(dist*1.0, radius*1.1));
    // mask = clamp01(mask);
    if (dist > (radius+0.08)) {
        mask = 0.0;
    }
    value *= mask;
    // value = mask;
    // value = map_range_01_to(value, 0.9, 1.0);

    // map to color
    CHSV pixel_hsv = CHSV(hue, saturation, value);

    pixel_hsv.value *= visibility;
    return pixel_hsv;
}






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
