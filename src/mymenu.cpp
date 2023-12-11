// NOLINT(legal/copyright)

/******************************************************************************

    __doc__ = """
    debugmenu
    """

    libraries used:
        ~ slight_DebugMenu
            written by stefan krueger (s-light),
                git@s-light.eu, http://s-light.eu, https://github.com/s-light/
            license: MIT

    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/

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

// cpplint
// disable linting for next line:
// NOLINTNEXTLINE(readability/nolint)
// https://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py

// include own headerfile
// NOLINTNEXTLINE(build/include)
#include "./mymenu.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions

MyMenu::MyMenu(
    Stream& out_,
    MyAnimation& animation,
    MyInput& myinput,
    PowerHandling& powerhandling,
    const sketchinfo_func sketchinfo_print
)
    : myDebugMenu{slight_DebugMenu(Serial, Serial, 20)},
      out(out_),
      animation(animation),
      myinput(myinput),
      powerhandling(powerhandling),
      //      ^ '(' needed. its a kind of bug in gcc..
      // https://stackoverflow.com/questions/10509603/why-cant-i-initialize-a-reference-in-an-initializer-list-with-uniform-initializ
      ready{false},
      sketchinfo_print{sketchinfo_print}  // NOLINTNEXTLINE(whitespace/braces)
{
    // nothing to do right now..
}

MyMenu::~MyMenu() {
    end();
}

void MyMenu::begin() {
    // clean up..
    end();
    // start up...
    if (ready == false) {
        // setup
        out.println("MyMenu begin:");

        // LiveSign
        pinMode(infoled_pin, OUTPUT);
        digitalWrite(infoled_pin, LOW);
        // as of arduino 1.0.1 you can use INPUT_PULLUP

        // out.println("  mydebugmenu.begin");
        myDebugMenu.set_callback(
            std::bind(&MyMenu::handleMenu_Main, this, std::placeholders::_1)
        );
        myDebugMenu.begin(true);

        out.println("done:");
        // enable
        ready = true;
    }
}

void MyMenu::end() {
    if (ready) {
        ready = false;
    }
}

void MyMenu::update() {
    if (ready) {
        myDebugMenu.update();
        debugOut_update();
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// debug things

void MyMenu::debugOut_update() {
    uint32_t duration_temp = millis() - debugOut_LastAction;
    if (duration_temp > debugOut_interval) {
        debugOut_LastAction = millis();

        if (debugOut_Serial_Enabled) {
            // out.print(millis());
            // out.print(F("ms; "));
            // Serial.println();

            out.printf(
                "%8dms   "
                "batt: %3.0f%% (%.3fV)",
                // "\r\n",
                millis(),
                powerhandling.batteryMonitor.cellPercent(),
                powerhandling.batteryMonitor.cellVoltage()
            );
            out.println();

            // print time
            // this output needs about ~1.7ms on ESP32 with 240MHz clock..
            // uint16_t ms = millis() % 1000;
            // uint16_t s = (millis() / 1000) % 60;
            // uint16_t m = (millis() / 1000 * 60) % 60;
            // uint16_t h = (millis() / 1000 * 60 * 60) % 60;
            // Serial.printf("%02u:%02u:%02u:%03u", h, m, s, ms);
            // Serial.println();
        }

        if (debugOut_LED_Enabled) {
            infoled_state = !infoled_state;
            if (infoled_state) {
                digitalWrite(infoled_pin, HIGH);
            } else {
                digitalWrite(infoled_pin, LOW);
            }
        }
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// menu

void MyMenu::menu__test_xxx(Print& out) {
    out.println(F("TODO"));
    out.println();
}

void MyMenu::menu__set_yyy(Print& out, char* command) {
    out.println(F("Set yyy "));
    out.println(F("TODO"));

    out.print(F("Set yyy "));
    uint8_t command_offset = 1;
    uint8_t index = atoi(&command[command_offset]);
    // a better way than this would be to search for the ':'
    // i have used this a long time ago for MAC address format parsing
    // was something with 'tokenize' or similar..
    command_offset = 3;
    if (index > 9) {
        command_offset = command_offset + 1;
    }
    out.print(index);
    out.print(F(" to "));
    uint16_t value = atoi(&command[command_offset]);
    out.print(value);
    // tlc.set_pixel_16bit_value(index, value, value, value);
    out.println();
}

void MyMenu::menu__set_pixel(Print& out, char* command) {
    out.print(F("Set pixel "));
    uint8_t command_offset = 1;
    uint8_t index = atoi(&command[command_offset]);
    // a better way than this would be to search for the ':'
    // i have used this a long time ago for MAC address format parsing
    // was something with 'tokenize' or similar..
    command_offset = 3;
    if (index > 9) {
        command_offset = command_offset + 1;
    }
    out.print(index);
    out.print(F(" to "));
    uint16_t value = atoi(&command[command_offset]);
    out.print(value);
    // animation.pixels.set_pixel_16bit_value(index, value, value, value);
    out.println();
}

void MyMenu::menu__set_board_dotstar(Print& out, char* command) {
    out.print(F("Set board_dotstar: "));
    char* command_offset = command;
    uint8_t red = atoi(command_offset);
    command_offset = strchr(command_offset, ',');
    uint8_t green = atoi(command_offset + 1);
    command_offset = strchr(command_offset + 1, ',');
    uint8_t blue = atoi(command_offset + 1);
    // out.print(F(" red:"));
    // out.print(red);
    // out.print(F(" green:"));
    // out.print(green);
    // out.print(F(" blue:"));
    // out.print(blue);
    out.printf("red %03d, green %03d, blue %03d", red, green, blue);
    myinput.board_dotstar.setPixelColor(0, red, green, blue);
    myinput.board_dotstar.show();
    out.println();
}

void MyMenu::menu__reboot_to_uf2(Print& out) {
    out.println(F("reset to uf2.."));
    reboot_to_uf2();
}

void MyMenu::menu__time_meassurements(Print& out) {
    out.println(F("time_meassurements:"));

    uint32_t tm_start = 0;
    uint32_t tm_end = 0;
    uint32_t tm_duration = 0;
    uint32_t tm_loop_count = 10;

    animation.animation_run = true;
    // animation.fx_current = &animation.fx_rainbow;
    for (size_t i = 0; i < tm_loop_count; i++) {
        tm_start = millis();
        // fx_current.calculate_effect_position();
        // fx_current.update()
        // // for now just copy pixel data from effect to master
        // pixels = fx_current.pixels;
        // overwrite_black();
        // animation.show();
        animation.update();

        tm_end = millis();
        tm_duration += (tm_end - tm_start);
    }

    out.print(tm_duration / static_cast<float>(tm_loop_count));
    out.print(F("ms / call"));
    out.println();
}

// TODO add TARGET IFDEF guard...
void MyMenu::reboot_to_uf2(void) {
    // https://github.com/adafruit/tinyuf2/blob/8a54e0ed4c3373765903ca6d80eeb54dbcfc54d5/ports/esp32s2/README.md#usage
    // Check out esp_reset_reason_t for other Espressif pre-defined values
    enum {
        APP_REQUEST_UF2_RESET_HINT = 0x11F2
    };

    // call esp_reset_reason() is required for idf.py to properly links
    // esp_reset_reason_set_hint()
    (void)esp_reset_reason();
    esp_reset_reason_set_hint(static_cast<esp_reset_reason_t>(APP_REQUEST_UF2_RESET_HINT
    ));
    esp_restart();
}

//
// void MyMenu::menu__set_overwrite_black(Print &out, char *command) {
//     out.print(F("set overwrite_black: "));
//     char *command_offset = command +1;
//     uint16_t start = atoi(command_offset);
//     command_offset = strchr(command_offset, ',');
//     uint16_t end = atoi(command_offset+1);
//     out.printf("(%3d, %3d) >", start, end);
//     animation.overwrite_set(start, end);
//     out.printf(
//         " (%3d, %3d)",
//         animation.overwrite_start_get(),
//         animation.overwrite_end_get());
//     out.println();
//     out.println();
// }
//
// void MyMenu::menu__set_overwrite_black_relative(Print &out, char *command) {
//     out.print(F("set overwrite_black_relative: "));
//     char *command_offset = command +1;
//     int16_t value = atoi(command_offset);
//     out.print(value);
//     // out.println();
//     out.printf(
//         " (%3d, %3d) --> ",
//         animation.overwrite_start_get(),
//         animation.overwrite_end_get());
//     animation.global_overwrite.set_relative(value);
//     out.printf(
//         " (%3d, %3d)",
//         animation.overwrite_start_get(),
//         animation.overwrite_end_get());
//     out.println();
//     out.println();
// }

//
// void MyMenu::menu__set_hue(Print &out, char *command) {
//     out.print(F("Set hue "));
//     uint8_t command_offset = 1;
//     float value = atof(&command[command_offset]);
//     out.print(value);
//     animation.hue = value;
//     out.println();
// }
//
// void MyMenu::menu__set_saturation(Print &out, char *command) {
//     out.print(F("Set saturation "));
//     uint8_t command_offset = 1;
//     float value = atof(&command[command_offset]);
//     out.print(value);
//     animation.saturation = value;
//     out.println();
// }
//
// void MyMenu::menu__set_brightness(Print &out, char *command) {
//     out.print(F("Set brightness "));
//     uint8_t command_offset = 1;
//     float value = atof(&command[command_offset]);
//     out.print(value);
//     animation.brightness = value;
//     out.println();
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Menu System

void MyMenu::tests(Print& out) {
    out.println(F("__________"));
    out.println(F("Tests:"));
    // out.println(F("nothing to do."));

    animation.menu__test_colors(out);
    out.println(F("__________"));
}

void MyMenu::menu__print_help(Print& out) {
    // help
    out.println(F("__________________________________________________"));
    out.println();
    out.println(F("Help for Commands:"));
    out.println();
    out.println(F("\t '?': this help"));
    out.println(F("\t '!': sketch info"));
    out.println(F("\t 'y': toggle DebugOut livesign print"));
    out.println(F("\t 'Y': toggle DebugOut livesign LED"));
    out.println(F("\t 'x': tests"));
    out.println();
    out.print(F("\t 'r': toggle animation_run 'r' ("));
    out.print(animation.animation_run);
    out.println(F(")"));
    out.print(F("\t 'L': start loop n times 'L3' ("));
    out.print(animation.animation_loopcount);
    out.println(F(")"));
    out.print(F("\t 'j': fx_wave start singleshot 'j'"));
    out.println();
    out.print(F("\t 'J': fx_wave start loop n times 'l3' ("));
    out.print(animation.fx_wave->get_loopcount());
    out.println(F(")"));
    out.print(F("\t 'k': set fx_points point_count 'k10' ("));
    out.print(static_cast<FXPoints*>(animation.fx_points)->point_count);
    out.println(F(")"));
    out.print(F("\t 'd': set effect_duration 'd1000' ("));
    out.print(animation.effect_duration);
    out.println(F("ms)"));
    out.print(F("\t 'h': set hue 'h1.0' ("));
    out.print(animation.hue, 4);
    out.println(F(")"));
    out.print(F("\t 'h': set saturation 's1.0' ("));
    out.print(animation.saturation, 4);
    out.println(F(")"));
    out.print(F("\t 'c': set contrast 'c1.0' ("));
    out.print(animation.contrast, 4);
    out.println(F(")"));
    out.print(F("\t 'b': set brightness 'b1.0' ("));
    out.print(animation.brightness, 5);
    out.println(F(")"));
    out.print(F("\t 'B': fade brightness 'B1.0' ("));
    out.print(animation.brightness, 5);
    out.println(F(")"));
    out.print(F("\t 'f': set fps 'f500' ("));
    out.print(animation.get_fps());
    out.println(F(")"));
    out.println();
    // out.println(F("\t 'f': test fc 'f'"));
    out.println(F("\t 'u': tlc.write() 'u'"));
    // out.print(F("\t 'e': toggle ESPWM 'e' ("));
    // out.print(animation.matrix.tlc.get_fc_ESPWM());
    // out.println(F(")"));
    // out.print(F("\t 'g': set grayscale frequency in MHz 'g1.0' ("));
    // out.print(animation.gsclock_get_frequency_MHz(), 4);
    // out.println(F("MHz)"));
    // out.print(F("\t 's': set spi baudrate in MHz 's1.0' ("));
    // out.print(animation.matrix.tlc.spi_baudrate / (1000.0 * 1000), 4);
    // out.println(F("MHz)"));
    out.println(F("\t 't': set buffer to test values 't'"));
    out.println(F("\t 'T': time_meassurements 'T'"));
    // out.println(F("\t 'p': set pixel 'pX,Y:65535'"));
    // out.println(F("\t 'P': set pixel 'p0:65535'"));
    out.println(F("\t 'l': set layer to 2000 'lY'"));
    out.println(F("\t 'p': set pixel to 2000 'pX,Y'"));
    out.println(F("\t 'P': set pixel to 2000 'p0'"));
    out.println(F("\t 'z': set all pixel to black 'z'"));
    out.println(F("\t 'Z': set all pixel 'Z65535,65535,65535'"));
    // out.println(F("\t 'Z': set all pixel to 21845 'z'"));
    // out.println(F("\t 'B': print Buffer 'B'"));
    out.println(F("\t 'q': print pmap 'q'"));
    out.println(F("\t 'Q': print pmap_layer 'Q'"));
    // out.println(F("\t 'F': print buffer_fc 'F'"));
    out.println();
    // out.println(F("\t 'a': print ambient light sensor 'a'"));
    // out.println(F("\t 'A': toggle als_debugout_enabled 'A'"));
    // out.print(F("\t 'B': toggle als_sets_brightness 'B' ("));
    // out.print(myinput.als_sets_brightness);
    // out.print(F(" | "));
    // out.print(myinput.get_als_brightness_automatic(), 5);
    // out.println(F(")"));
    // out.println(F("\t 'q': test light_map 'q1.0"));
    out.println();
    out.println(F("\t '#': powerhandling: deep sleep '#'"));
    out.println(F("\t '$': reboot_to_uf2 '$'"));
    out.println(F("\t '<': print battery state '<'"));
    out.println(F("\t '+': print wakeup reason '+'"));
    out.println();
    out.println(F("__________________________________________________"));
}

// Main Menu
void MyMenu::handleMenu_Main(slight_DebugMenu* instance) {
    Print& out = instance->get_stream_out_ref();
    char* command = instance->get_command_current_pointer();
    // out.print("command: '");
    // out.print(command);
    // out.println("'");
    switch (command[0]) {
        // case 'h':
        // case 'H':
        case '?': {
            menu__print_help(out);
        } break;
        case '!': {
            sketchinfo_print(out);
        } break;
        case 'y': {
            out.println(F("\t toggle DebugOut livesign Serial:"));
            debugOut_Serial_Enabled = !debugOut_Serial_Enabled;
            out.print(F("\t debugOut_Serial_Enabled:"));
            out.println(debugOut_Serial_Enabled);
        } break;
        case 'Y': {
            out.println(F("\t toggle DebugOut livesign LED:"));
            debugOut_LED_Enabled = !debugOut_LED_Enabled;
            out.print(F("\t debugOut_LED_Enabled:"));
            out.println(debugOut_LED_Enabled);
        } break;
        case 'x': {
            tests(out);
        } break;
        // ---------------------
        case 'r': {
            out.println(F("toggle animation_run"));
            animation.animation_run = !animation.animation_run;
        } break;
        case 'L': {
            animation.menu__start_loop_n_times(out, command);
        } break;
        case 'k': {
            out.println(F("set point_count:"));
            uint8_t value = atoi(&command[1]);
            out.print(value);
            static_cast<FXPoints*>(animation.fx_points)->point_count = value;
            out.println();
        } break;
        case 'j': {
            out.println(F("fx_wave start_singleshot"));
            // set to 1s
            animation.fx_wave->duration = 1000;
            animation.fx_wave->start_singleshot();
        } break;
        case 'J': {
            out.println(F("fx_wave start loop n-times:"));
            uint8_t value = atoi(&command[1]);
            out.print(value);
            animation.fx_wave->start_loop_n_times(value);
            out.println();
        } break;
        case 'd': {
            out.println(F("set effect_duration:"));
            uint16_t value = atoi(&command[1]);
            out.print(value);
            animation.effect_duration = value;
            out.println();
        } break;
        case 'h': {
            animation.menu__set_hue(out, command);
        } break;
        case 's': {
            animation.menu__set_saturation(out, command);
        } break;
        case 'c': {
            animation.menu__set_contrast(out, command);
        } break;
        case 'b': {
            animation.menu__set_brightness(out, command);
        } break;
        case 'B': {
            animation.menu__fade_brightness(out, command);
        } break;
        case 'f': {
            animation.menu__set_fps(out, command);
        } break;
        // ---------------------
        case 'u': {
            out.println(F("write buffer to chips"));
            animation.matrix.tlc.write();
        } break;
        // case 'e': {
        //     out.println(F("toggle ESPWM"));
        //     animation.matrix.tlc.set_fc_ESPWM_all(!animation.matrix.tlc.get_fc_ESPWM());
        //     animation.matrix.tlc.update_fc();
        // } break;
        // case 'g': {
        //     out.print(F("set grayscale frequency - new value:"));
        //     float value = atof(&command[1]);
        //     out.print(value);
        //     value = animation.gsclock_set_frequency_MHz(value);
        //     out.print(F(" → "));
        //     out.print(value, 4);
        //     out.println(F("MHz"));
        // } break;
        // case 's': {
        //     out.print(F("set spi baudrate in MHz - new value:"));
        //     float value = atof(&command[1]);
        //     out.print(value, 4);
        //     out.println(F("MHz"));
        //     animation.matrix.tlc.spi_baudrate = value * 1000 * 1000;
        //     // out.print(F(" → "));
        //     // out.print(animation.matrix.tlc.spi_baudrate);
        //     // out.println();
        // } break;
        // case 't': {
        //     animation.menu__test_buffer(out);
        // } break;
        case 'T': {
            animation.menu__time_meassurements(out);
        } break;
        case 'l': {
            animation.menu__set_layer(out, command);
            animation.matrix.tlc.write();
        } break;
        case 'p': {
            animation.menu__set_pixel_layer(out, command);
            animation.matrix.tlc.write();
        } break;
        case 'P': {
            animation.menu__set_pixel(out, command);
            // animation.menu__set_pixel_index(out, command);
            animation.matrix.tlc.write();
        } break;
        case 'z': {
            out.println(F("Set all Pixel to black."));
            animation.matrix.setRGB(0, 0, 0);
            animation.matrix.tlc.write();
            out.println();
        } break;
        case 'Z': {
            animation.menu__set_all_pixel(out, command);
            animation.matrix.tlc.write();
        } break;
        case 'q': {
            animation.matrix.print_pmap(out);
        } break;
        case 'Q': {
            // animation.matrix.print_2Dmatrix(out);
            animation.matrix.print_layermap(out);
        } break;
        // case 'Z': {
        //     out.println(F("Set all Pixel to 21845."));
        //     animation.matrix.setRGB(21845, 21845, 21845);
        //     out.println();
        // } break;
        // case 'B': {
        //     out.println(F("Print Buffer:"));
        //     animation.matrix.print_tlc_buffer(out);
        //     out.println();
        // } break;
        // case 'F': {
        //     out.println(F("Print buffer_fc:"));
        //     animation.matrix.tlc.print_buffer_fc(out);
        //     out.println();
        // } break;
        //---------------------------------------------------------------------
        // case 'a': {
        //     out.println(F("Print ambient light sensor:"));
        //     // myinput.als.print_status(out);
        //     myinput.als_debugout(out);
        //     out.println();
        // } break;
        // case 'A': {
        //     out.println(F("toggle als_debugout_enabled:"));
        //     myinput.als_debugout_enabled = !myinput.als_debugout_enabled;
        // } break;
        // case 'B': {
        //     out.println(F("toggle als_sets_brightness:"));
        //     myinput.als_sets_brightness = !myinput.als_sets_brightness;
        // } break;
        // case 'q': {
        //     out.println(F("test light_map:"));
        //     uint8_t command_offset = 1;
        //     float value = atof(&command[command_offset]);
        //     out.print(value, 5);
        //     out.print(" -> ");
        //     out.print(myinput.light_map.mapit(value), 5);
        //     out.println();
        // } break;
        //---------------------------------------------------------------------
        case '#': {
            powerhandling.enter_sleep_mode();
        } break;
        case '$': {
            menu__reboot_to_uf2(out);
        } break;
        case '<': {
            powerhandling.print_battery_state();
        } break;
        case '>': {
            powerhandling.print_wakeup_reason();
        } break;
        case '+': {
            powerhandling.print_wakeup_reason();
        } break;
        //---------------------------------------------------------------------
        default: {
            if (strlen(command) > 0) {
                out.print(F("command '"));
                out.print(command);
                out.println(F("' not recognized. try again."));
            }
            instance->get_command_input_pointer()[0] = '?';
            instance->set_flag_EOC(true);
        }
    }  // end switch

    // end Command Parser
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
