//https://editor.soulmatelights.com/gallery/1401-mapping-test
// Your pattern code goes here.
// This draw() function is called repeatedly and the LEDs will be
// printed after every loop.
//
// Some helpful variables:
// ROWS: number of rows
// COLS: number of columns
// N_LEDS: total number of LEDs (LED_COLS * LED_ROWS)
// leds: the LED array to print to
//
// You can also use:
// uint16_t XY(x, y) - the index of a given x/y coordinate
// uint8_t beatsin8(bpm, minimum, maximum, offset) - an 8-bit sine wave
//
// For more information, visit https://github.com/FastLED/FastLED/wiki/Overview

const uint8_t MATRIX_ROW_COUNT = ROWS;
const uint8_t MATRIX_COL_COUNT = COLS;




template < class T >
  T map_range(T x, T in_min, T in_max, T out_min, T out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

template < class T1, class T2 >
  T2 map_range(T1 x, T1 in_min, T1 in_max, T2 out_min, T2 out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

template < class T1, class T2 >
  T2 map_range(T1 x, T2 in_min, T2 in_max, T2 out_min, T2 out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

template < class T1, class T2, class T3 >
  T3 map_range(T1 x, T2 in_min, T2 in_max, T3 out_min, T3 out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

template < class T1, class T2 >
  T2 normalize_to_01(T1 x, T1 in_min, T1 in_max) {
    return ((x - in_min) * 1.0) / (in_max - in_min);
  }

template < class T1 >
  double normalize_to_01(T1 x, T1 in_min, T1 in_max) {
    return ((x - in_min) * 1.0) / (in_max - in_min);
  }

template < class T1, class T2 >
  double normalize_to_01(T1 x, T2 in_min, T2 in_max) {
    return ((x - in_min) * 1.0) / (in_max - in_min);
  }

template < class T1, class T2, class T3 >
  T3 normalize_to_01(T1 x, T2 in_min, T2 in_max) {
    return ((x - in_min) * 1.0) / ((in_max - in_min) * 1.0);
  }




struct CHSVX {
  // based on / copied partly from:
  // https://github.com/FastLED/FastLED/blob/master/pixeltypes.h
  union {
    float hue;
    float h;
  };
  union {
    float saturation;
    float sat;
    float s;
  };
  union {
    float value;
    float val;
    float v;
  };

  // default values are UNITIALIZED
  inline CHSVX() __attribute__((always_inline)) {}

  // allow construction from H, S, V
  inline CHSVX(float ih, float is, float iv) __attribute__((always_inline)): h(ih), s(is), v(iv) {}

};




struct CRGBX {
  // based on / copied partly from:
  // https://github.com/FastLED/FastLED/blob/master/pixeltypes.h
  union {
    float r;
    float red;
  };
  union {
    float g;
    float green;
  };
  union {
    float b;
    float blue;
  };

  // default values are UNINITIALIZED
  inline CRGBX() __attribute__((always_inline)) {}

  // allow construction from R, G, B
  inline CRGBX(float ir, float ig, float ib) __attribute__((always_inline)): r(ir), g(ig), b(ib) {}
};




CRGBX hsv2rgb(CHSVX hsv) {
  // based on :
  // https://github.com/adafruit/Adafruit_CircuitPython_FancyLED/blob/master/adafruit_fancyled/adafruit_fancyled.py#L72

  // Hue circle = 0.0 to 6.0
  float hue = hsv.hue * 6.0;
  // Sextant index is next-lower integer of hue
  float sxt = floor(hue);
  // Fraction-within-sextant is 0.0 to <1.0
  float frac = hue - sxt;
  // mod6 the sextant so it's always 0 to 5
  uint8_t sxt_int = static_cast < int > (sxt) % 6;

  float r = 0.0;
  float g = 0.0;
  float b = 0.0;

  switch (sxt_int) {
    case 0: {
      // Red to <yellow
      r = 1.0;
      g = frac;
      b = 0.0;
    }
    break;
  case 1: {
    // Yellow to <green
    r = 1.0 - frac;
    g = 1.0;
    b = 0.0;
  }
  break;
  case 2: {
    // Green to <cyan
    r = 0.0;
    g = 1.0;
    b = frac;
  }
  break;
  case 3: {
    // Cyan to <blue
    r = 0.0;
    g = 1.0 - frac;
    b = 1.0;
  }
  break;
  case 4: {
    // Blue to <magenta
    r = frac;
    g = 0.0;
    b = 1.0;
  }
  break;
  case 5: {
    // Magenta to <red
    r = 1.0;
    g = 0.0;
    b = 1.0 - frac;
  }
  break;
  }

  // Inverse-of-saturation
  float invsat = 1.0 - hsv.saturation;

  CRGBX result_rgb = CRGBX();
  result_rgb.red = ((r * hsv.saturation) + invsat) * hsv.value;
  result_rgb.green = ((g * hsv.saturation) + invsat) * hsv.value;
  result_rgb.blue = ((b * hsv.saturation) + invsat) * hsv.value;
  return result_rgb;
}












uint16_t effect_duration = 5 * 1000; //ms
uint32_t effect_start = 0;
uint32_t effect_end = 0;
uint32_t effect_loopcount = 0;
float effect_position = 0.0;

float hue = 0.05;
float saturation = 1.0;
float contrast = 1.70;
float brightness = 0.1;




void calculate_effect_position() {
  //effect_position = normalize_to_01(micros(), effect_start, effect_end);
  effect_position += 0.01;
  // effect_loopcount++;
  if (effect_position > 1.0) {
    effect_position = 0.0;
    effect_start = micros();
    effect_end = micros() + (effect_duration * 1000);
  }
}















CHSVX effect__mapping_checker(float col, float row, float offset) {
  CHSVX pixel_hsv = CHSVX(0.7, 1.0, 0.1);

  float row_width = (1.0 / MATRIX_ROW_COUNT / 1.5);
  float col_width = (1.0 / MATRIX_COL_COUNT / 1.5);
  // float base = col * 0.2 + offset;
  float offset_half = map_range(offset, 0.0, 1.0, -0.5, 0.5);
  // float base = map_range(col, -0.5, 0.5, 0.0, 1.0);
  // float base = map_range(col, -0.5, 0.5, 0.0, 1.0);
  // base = map_range(offset, 0, 1.0, 0.0, 0.1);
  // base *= 10.0;
  // // base *= 5.0;
  // float offset_PI = offset * (3.141592 / 2);
  // base += offset_PI;
  // // base += (offset*2);
  // Serial.printf("(%+2.2f|%+2.2f): %2.3f\r\n", col, row, base);
  // pixel_hsv.value = sin(base);
  // pixel_hsv.hue = sin(base);
  // pixel_hsv.value = base;

  // Serial.printf(
  //     "%+2.2f  "
  //     "%+2.2f|%+2.2f  "
  //     "%+2.2f|%+2.2f\r\n",
  //     offset,
  //     row, col,
  //     row_width, abs(offset - row));
  // Serial.printf(
  //     "%+2.2f  "
  //     "%+2.2f  "
  //     "%+2.2f  "
  //     "%+2.2f\r\n",
  //     offset_half,
  //     row,
  //     row_width,
  //     abs(offset_half - row));

  if (abs(offset_half - row) <= row_width) {
    pixel_hsv.hue = 0.1;
    pixel_hsv.value = 1.0;
  }
  if (abs(offset_half - col) <= col_width) {
    pixel_hsv.hue = 0.4;
    pixel_hsv.value = 1.0;
  }

  return pixel_hsv;
}






























































CHSVX effect_Matrix2D_get_pixel(
  __attribute__((unused)) float col,
  __attribute__((unused)) float row,
  __attribute__((unused)) float offset
) {
  CHSVX pixel_hsv = CHSVX(hue, saturation, 1.0);

  // plasma
  // CHSVX plasma = effect__plasma(col, row, offset);
  // pixel_hsv = plasma;

  // wave
  // pixel_hsv = effect__wave(col, row, offset);
  // pixel_hsv = wave;


  // sparkle
  // CHSVX sparkle = effect__sparkle(col, row, offset);
  // pixel_hsv = sparkle;

  pixel_hsv = effect__mapping_checker(col, row, offset);


  // TODO(s-light): develop 'layer' / 'multiplyer' system...
  // merge layers

  return pixel_hsv;
}

void effect_Matrix2D() {
  // float offset = map_range_01_to(effect_position, 0.0, (PI * 30));
  // float offset = map_range_01_to(effect_position, 0, MATRIX_ROW_COUNT);
  float offset = effect_position;

  // Serial.println("");

  // float offset_PI = map_range(offset, 0.0, 1.0, 0.0, (3.14/2));
  // PI 3,141592653589793
  // float offset_PI = offset * (3.141592 / 2);
  // Serial.printf(
  //     "%2.3f %2.3f "
  //     "%2.3f %2.3f \r\n",
  //     // offset,
  //     // sin(offset),
  //     offset_PI,
  //     sin(offset_PI)
  //     // NOLINTNEXTLINE(whitespace/parens)
  // );

  for (size_t row_i = 0; row_i < MATRIX_ROW_COUNT; row_i++) {
    // normalize row
    float row = map_range(
      row_i,
      0, MATRIX_ROW_COUNT - 1,
      -0.5, 0.5);
    // float row = normalize_to_01(row_i, 0, MATRIX_ROW_COUNT-1);
    for (size_t col_i = 0; col_i < MATRIX_COL_COUNT; col_i++) {
      // normalize col
      // float col = map_range__int2float(
      float col = map_range(
        col_i,
        0, MATRIX_COL_COUNT - 1,
        -0.5, 0.5);
      // float col = normalize_to_01(col_i, 0, MATRIX_COL_COUNT-1);

      // ------------------------------------------
      // CHSVX pixel_hsv = effect_Matrix2D_get_pixel(col, row, col_i, row_i, offset);
      CHSVX pixel_hsv = effect_Matrix2D_get_pixel(col, row, offset);
      // CHSVX pixel_hsv = effect_Matrix2D_get_pixel(col, row, offset_PI);

      // ------------------------------------------
      // final conversions
      // global brightness
      pixel_hsv.value *= brightness;

      // CHSVX pixel_hsv = CHSVX(0.5, 0.0, 0.10);
      // convert to rgb
      CRGBX pixel_rgb = hsv2rgb(pixel_hsv);
      // gamma & global brightness
      // fancyled.gamma_adjust(brightness=self.brightness);
      uint16_t index = XY(col_i, row_i);
      leds[index] = CRGB(
        // convert float to uint16_t
        pixel_rgb.r * 65535,
        pixel_rgb.g * 65535,
        pixel_rgb.b * 65535
      );
    }
  }
}















void draw() {
  //EVERY_N_MILLISECONDS(10) {
  //  offset += 1;
  //}

  calculate_effect_position();
  effect_Matrix2D();

}
