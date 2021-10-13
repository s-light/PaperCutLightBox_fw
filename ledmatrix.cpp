// NOLINT(legal/copyright)
/******************************************************************************

    matrix.cpp
        all things led matrix related

    libraries used:
        ~ slight_DebugMenu
            written by stefan krueger (s-light),
                github@s-light.eu, http://s-light.eu, https://github.com/s-light/
            license: MIT
        ~ Tlc59711.h
            License: MIT
            Copyright (c) 2016 Ulrich Stern
            https://github.com/ulrichstern/Tlc59711

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

#include "./ledmatrix.h"


MyLEDMatrix::MyLEDMatrix() {
    ready = false;
}

MyLEDMatrix::~MyLEDMatrix() {
    end();
}

void MyLEDMatrix::begin(Stream &out) {
    // clean up..
    end();
    // start up...
    if (ready == false) {
        // setup
        pmap_init();
        tlc_init(out);
        // enable
        ready = true;
    }
}

void MyLEDMatrix::end() {
    if (ready) {
        // nothing to do..
    }
}

void MyLEDMatrix::update() {
    if (ready) {
        // nothing to do..
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TLC5957 / LED-Driver

void MyLEDMatrix::tlc_init(Stream &out) {
    out.println(F("setup tlc:"));

    out.println(F("    tlc.beginFast()"));
    // tlc.beginFast();

    tlc.beginFast(
        // bufferXfer (default: true)
        false,
        // spiClock (default: 10 * 1000 * 1000)
        20 * 1000 * 1000
        // postXferDelayMicros (default: 4)
        // 4
        // NOLINTNEXTLINE(whitespace/parens)
    );

    // tlc.setBrightness(127, 127, 127);

    out.println(F("    start with leds off"));
    tlc.setRGB();
    tlc.write();

    out.println(F("    set leds to 0, 0, 1"));
    tlc.setRGB(0, 0, 1);
    tlc.write();


    // out.println(F("  set spi_baudrate"));
    // 2MHz
    // tlc.spi_baudrate = 2.0 * 1000 * 1000;
    // 0.001MHz = 1000kHz
    // tlc.spi_baudrate = 0.001 * 1000 * 1000;


    // out.print(F("  tlc.pixel_count: "));
    // out.print(tlc.pixel_count);
    // out.println();
    // out.print(F("  tlc.chip_count: "));
    // out.print(tlc.chip_count);
    // out.println();
    // out.print(F("  tlc.buffer_byte_count: "));
    // out.print(tlc.buffer_byte_count);
    // out.println();
    // out.print(F("  tlc.spi_baudrate: "));
    // out.print(tlc.spi_baudrate);
    // out.print(F("Hz"));
    // out.print(F("  = "));
    // out.print(tlc.spi_baudrate / 1000.0, 4);
    // out.print(F("kHz"));
    // out.print(F("  = "));
    // out.print(tlc.spi_baudrate / (1000.0 * 1000.0), 4);
    // out.println(F("MHz"));
    //
    // out.print(F("  tlc.get_fc_ESPWM(): "));
    // out.print(tlc.get_fc_ESPWM());
    out.println();
    out.println(F("  finished."));
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// mapping

// uint16_t mymap_LEDBoard_4x4_16bit(uint8_t col, uint8_t row) {
uint16_t MyLEDMatrix::mymap_LEDBoard_4x4_16bit(uint8_t col, uint8_t row) {
    // """Map row and col to pixel_index."""
    // get Board position
    uint16_t board_col = col / LEDBOARD_COL_COUNT;
    uint16_t board_row = row / LEDBOARD_ROW_COUNT;
    uint16_t board_sub_col = col % LEDBOARD_COL_COUNT;
    uint16_t board_sub_row = row % LEDBOARD_ROW_COUNT;

    // Serial.println("mymap_LEDBoard_4x4_16bit");
    // Serial.print("  col: ");
    // Serial.println(col);
    // Serial.print("  row: ");
    // Serial.println(row);
    //
    // Serial.print("  board_col: ");
    // Serial.println(board_col);
    // Serial.print("  board_row: ");
    // Serial.println(board_row);
    //
    // Serial.print("  board_sub_col: ");
    // Serial.println(board_sub_col);
    // Serial.print("  board_sub_row: ");
    // Serial.println(board_sub_row);


    uint16_t board_index = BOARDS_ORDER[board_row][board_col];
    uint16_t board_rotation = BOARDS_ROTATION[board_row][board_col];
    // Serial.print("  board_index: ");
    // Serial.println(board_index);
    // Serial.print("  board_rotation: ");
    // Serial.println(board_rotation);

    uint16_t board_pixel_offset = board_index * LEDBOARD_PIXEL_COUNT;
    // Serial.print("  board_pixel_offset: ");
    // Serial.println(board_pixel_offset);

    uint16_t board_sub_pixel_offset =
        LEDBOARD_SINGLE[board_rotation][board_sub_row][board_sub_col];
    // Serial.print("  board_sub_pixel_offset: ");
    // Serial.println(board_sub_pixel_offset);

    uint16_t pixel_index = board_pixel_offset + board_sub_pixel_offset;
    // Serial.print("  pixel_index: ");
    // Serial.println(pixel_index);

    return pixel_index;
}

// void pmap_init() {
void MyLEDMatrix::pmap_init() {
    // """Prepare Static Map."""
    for (size_t row_index = 0; row_index < MATRIX_ROW_COUNT; row_index++) {
        for (size_t col_index = 0; col_index < MATRIX_COL_COUNT; col_index++) {
            // uint16_t index = mymap_LEDBoard_4x4_HD(
            // uint16_t index = mymap_LEDBoard_4x4_HD_CrystalLightGuide(
            uint16_t index = mymap_LEDBoard_4x4_16bit(col_index, row_index);
            // Serial.print(row_index);
            // Serial.print(" ");
            // Serial.print(col_index);
            // Serial.print(" ");
            // Serial.print(index);
            // Serial.println();
            pmap[col_index][row_index] = index;
        }
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// menu


void MyLEDMatrix::print_pmap(Print &out) {
    out.println(F("print pixel map"));

    // slight_DebugMenu::print_uint16_array_2D(
    //     out, pmap, MATRIX_COL_COUNT, MATRIX_ROW_COUNT);

    Print &stream_out = out;
    size_t count_col = MATRIX_COL_COUNT;
    size_t count_row = MATRIX_ROW_COUNT;

    size_t col_i = 0;
    size_t row_i = 0;
    // print_uint16_align_right(stream_out, row_i);
    // stream_out.print(F(" --> "));
    // print_uint16_align_right(stream_out, pmap[col_i][row_i]);

    // print header line
    stream_out.print(F("  row / col"));
    col_i = 0;
    slight_DebugMenu::print_uint16_align_right(stream_out, 0);
    for (col_i = 1; col_i < count_col; col_i++) {
        stream_out.print(F(", "));
        slight_DebugMenu::print_uint16_align_right(stream_out, col_i);
    }
    stream_out.println();
    // stream_out.println();
    // print rows
    for (row_i = 0; row_i < count_row; row_i++) {
        // print row numer
        slight_DebugMenu::print_uint16_align_right(stream_out, row_i);
        stream_out.print(F("  --> "));
        col_i = 0;
        slight_DebugMenu::print_uint16_align_right(
            stream_out, pmap[col_i][row_i]);
        for (col_i = 1; col_i < count_col; col_i++) {
            stream_out.print(F(", "));
            slight_DebugMenu::print_uint16_align_right(
                stream_out, pmap[col_i][row_i]);
        }
        stream_out.println();
    }
}


void MyLEDMatrix::print_2Dmatrix(Print &out) {
    out.println(F("print 2DMatrix row col values"));
    Print &stream_out = out;

    // enable float support
    // https://github.com/arduino/ArduinoCore-samd/issues/217
    asm(".global _printf_float");


    size_t count_col = MATRIX_COL_COUNT;
    size_t count_row = MATRIX_ROW_COUNT;

    size_t col_i = 0;
    size_t row_i = 0;
    // print_uint16_align_right(stream_out, row_i);
    // stream_out.print(F(" --> "));
    // print_uint16_align_right(stream_out, pmap[col_i][row_i]);

    // print header line
    col_i = 0;
    float col = normalize_to_01(col_i, 0, MATRIX_COL_COUNT-1);
    stream_out.printf("int(float)     %2d(%1.2f)", col_i, col);
    for (col_i = 1; col_i < count_col; col_i++) {
        col = normalize_to_01(col_i, 0, MATRIX_COL_COUNT-1);
        stream_out.printf(",%2d(%1.2f)", col_i, col);
    }
    stream_out.println();

    // print rows
    for (row_i = 0; row_i < count_row; row_i++) {
        // normalize row
        // float row = map_range(
        //     row_i,
        //     0, MATRIX_ROW_COUNT-1,
        //     -0.5, 0.5);
        float row = normalize_to_01(row_i, 0, MATRIX_ROW_COUNT-1);

        // print row numer
        stream_out.printf("%2d(%1.2f) -->   ", row_i, row);

        col_i = 0;
        stream_out.printf(" %7d", pmap[col_i][row_i]);
        for (col_i = 1; col_i < count_col; col_i++) {
            stream_out.printf(", %7d", pmap[col_i][row_i]);
        }
        stream_out.println();
    }
}
