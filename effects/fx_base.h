/******************************************************************************

    __doc__ = """
    base class for effects
    Enjoy the colors :-)
    """

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



#ifndef FXBase_H_
#define FXBase_H_

// include Core Arduino functionality
#include <Arduino.h>

#include "../color.h"
#include "../mapping.h"

class FXBase {
public:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    FXBase();
    ~FXBase();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    void update_position();
    CHSV get_pixel(float col, float row);
    // CHSV get_pixel(
    //     float col, float row,
    //     uint16_t col_i, uint16_t row_i,
    //     float offset);
    // void get_all();

    void run(bool run_);
    void start_singleshot();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // helper
    // float set_brightness(float brightness_);
    // float set_hue(float hue_);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations


    uint32_t duration = 5 * 1000; //ms
    float hue = 0.5;
    float saturation = 1.0;
    float brightness = 1.0;
    float contrast = 1.00;

    // const float PI = 3.141592;
    // is already defined by arduino or some other defaults...

private:

    // int16_t calcDist(uint8_t x, uint8_t y, int8_t center_x, int8_t center_y);
    float calcDist(float x, float y, float center_x, float center_y);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool running = true;
    bool single_shot = false;
    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t loopcount = 0;
    // double position = 0.0;
    float position = 0.0;

};  // class FXBase

#endif  // FXBase_H_
