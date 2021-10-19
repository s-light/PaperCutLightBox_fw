/******************************************************************************

    some mapping helpers..

    written by stefan krueger (s-light),
        github@s-light.eu, http://s-light.eu, https://github.com/s-light/

******************************************************************************/
/******************************************************************************
The MIT License (MIT)

Copyright (c) 2020 Stefan Krüger

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



#ifndef easing_H_
#define easing_H_

// include Core Arduino functionality
#include <Arduino.h>

// easing functions inspired by https://easings.net

template<class T>
T easeIn(T t) {
    // return t * t * t * t * t * t * t * t * t * t * t;
    return t * t * t * t * t * t * t;
    // return t * t * t * t * t * t;
    // return t * t * t;
    // return t * t;
}

// double easeIn_double(double t) {
//     return t * t * t * t * t * t * t * t * t * t * t;
// }

template<class T>
T easeOut(T t) {
    t = 1.0 - t;
    // return 1.0 - (t * t * t * t * t);
    // return 1.0 - (t * t * t);
    // return 1.0 - (t * t);
    return 1.0 - (t);
}

template<class T>
T easeInExpo(T t) {
    return (t == 0) ? 0 : pow(2, 10 * t - 10);
}

#endif  // easing_H_
