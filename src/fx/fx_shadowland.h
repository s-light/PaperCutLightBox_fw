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
        CHSV(0.99, 1.0, 1.0),
        CHSV(0.8, 1.0, 0.0),
        // back
        CHSV(0.6, 1.0, 0.0),
        CHSV(0.6, 1.0, 0.0),
        CHSV(0.6, 1.0, 0.0),
        // mountains
        CHSV(0.5, 1.0, 1.0),
        CHSV(0.5, 1.0, 0.0),
        CHSV(0.5, 1.0, 0.0),
        // hills / land
        CHSV(0.4, 1.0, 0.0),
        CHSV(0.4, 1.0, 0.0),
        CHSV(0.4, 1.0, 0.1),
        CHSV(0.4, 1.0, 0.0),
        CHSV(0.4, 1.0, 0.0),
        CHSV(0.4, 1.0, 0.0),
        // land
        CHSV(0.9, 1.0, 1.0),
        CHSV(0.3, 1.0, 0.1),
        CHSV(0.3, 1.0, 0.0),
        CHSV(0.3, 1.0, 0.0),
        CHSV(0.3, 1.0, 0.0),
        // front trees / grass
        CHSV(0.2, 1.0, 0.0),
        CHSV(0.2, 1.0, 0.0),
        CHSV(0.2, 1.0, 0.0),
        CHSV(0.2, 1.0, 0.0),
        // front visible leds
        CHSV(0.1, 1.0, 0.0)
    };

private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes

};  // class FXShadowland

#endif  // FXShadowland_H_
