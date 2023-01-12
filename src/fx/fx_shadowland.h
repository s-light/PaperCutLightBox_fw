/******************************************************************************

    __doc__ = """
    shadowland - 
    Enjoy the colors :-)
    """

    libraries used:
        ~

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



#ifndef FXShadowland_H_
#define FXShadowland_H_

// include Core Arduino functionality
#include <Arduino.h>

#include "./fx_base.h"

#include "../ledmatrix.h"
// #include "./color.h"
// #include "./mapping.h"

class FXShadowland : public FXBase{
public:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    // FXShadowland();
    // ~FXShadowland();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public

    // void update_position();
    virtual CHSV get_pixel(PixelPos * pixel_pos);

    
    CHSV night[LAYER_ROW_COUNT] = {
        // we have 24 layers
        // stars
        CHSV(0.05, 1.0, 1.0),  // 0
        CHSV(0.05, 1.0, 1.0),  // 1
        // back
        CHSV(0.90, 1.0, 0.1),  // 2
        CHSV(0.80, 1.0, 0.1),  // 3
        // mountain back
        CHSV(0.70, 1.0, 0.2),  // 4
        // mountains front
        CHSV(0.60, 1.0, 0.2),  // 5
        CHSV(0.70, 1.0, 0.2),  // 6
        // river
        CHSV(0.65, 1.0, 1.0),  // 7
        // land with river
        CHSV(0.28, 1.0, 1.0),  // 8
        // hills / land
        CHSV(0.30, 1.0, 1.0),  // 9
        CHSV(0.28, 1.0, 1.0),  // 10
        CHSV(0.30, 1.0, 1.0),  // 11
        CHSV(0.28, 1.0, 1.0),  // 12
        CHSV(0.30, 1.0, 1.0),  // 13
        // land
        CHSV(0.28, 1.0, 0.6),  // 14
        CHSV(0.28, 1.0, 0.6),  // 15
        CHSV(0.28, 1.0, 0.5),  // 16
        // front trees / grass
        CHSV(0.22, 1.0, 0.20),  // 17
        CHSV(0.22, 1.0, 0.10),  // 18
        CHSV(0.22, 1.0, 0.10),  // 19
        CHSV(0.22, 1.0, 0.02),  // 20
        CHSV(0.22, 1.0, 0.02),  // 21
        // front visible leds
        CHSV(0.20, 1.0, 0.001),  // 22
        CHSV(0.20, 1.0, 0.001)   // 23
    };

private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes

};  // class FXShadowland

#endif  // FXShadowland_H_
