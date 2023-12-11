// NOLINT(legal/copyright)

/******************************************************************************

    __doc__ = """
    sensor input and pre-conditioning
    """

    libraries used:
        ~ Adafruit_Sensor
        ~ Adafruit_TSL2591
            written by Adafruit
        ~ slight_ButtonInput
        ~ slight_DebugMenu
            written by stefan krueger (s-light),
                github@s-light.eu, http://s-light.eu,
https://github.com/s-light/ license: MIT

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

// cpplint
// disable linting for next line:
// NOLINTNEXTLINE(readability/nolint)
// https://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py

// include own headerfile
// NOLINTNEXTLINE(build/include)
#include "./myinput.h"

#include <algorithm>
#include <ratio>

// // include Core Arduino functionality
// #include <Arduino.h>
//
// #include <slight_DebugMenu.h>
//
// #include <Wire.h>
// #include <Adafruit_Sensor.h>
// #include "Adafruit_TSL2591.h"
//
// #include <slight_ButtonInput.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions

// MyInput::MyInput(button):
// ) {

MyInput::MyInput(Stream& out_, MyAnimation& animation, PowerHandling& powerhandling)
    : out(out_),
      animation(animation),
      powerhandling(powerhandling),
      //       ^ '(' needed. its a bug in gcc..
      // https://stackoverflow.com/questions/10509603/why-cant-i-initialize-a-reference-in-an-initializer-list-with-uniform-initializ
      ready{false}  // NOLINTNEXTLINE(whitespace/braces)
{
    // nothing to do right now..
}

MyInput::~MyInput() {
    end();
}

void MyInput::begin(slight_RotaryEncoder::tCallbackFunctionISR func_ISR) {
    // clean up..
    end();

    // find board definition
    // https://arduino.stackexchange.com/questions/19892/list-of-arduino-board-preprocessor-defines
    // grep board= `find ~/.arduino15 -name boards.txt` | cut -f2 -d= | sort -u
    // usage:
    // #if defined(ITSYBITSY_M4)
    // special code for this board here..
    // #endif

    // start up...
    if (ready == false) {
        // setup
        out.println("SettingsUI begin:");
        // sleepmode_init(out);
        out.println("  board_dotstar");
        board_dotstar.begin();
        // board_dotstar.setPixelColor(0, board_dotstar_active_color);
        board_dotstar.setPixelColor(0, board_dotstar_standby_color);
        board_dotstar.show();
        // als.begin(out);
        button_init(out);
        encoder_init(out, func_ISR);
        // out.println("  myencoder.begin");
        // myencoder.begin(funcISR);
        out.println("done.");
        ready = true;
        out.println("MyInput.begin ready = true");
    }
}

void MyInput::end() {
    if (ready) {
        // nothing to do..
    }
}

void MyInput::update() {
    if (ready) {
        // do it :-)
        // als_update(out);
        // out.println("button.");
        for (size_t i = 0; i < UIButton_count; i++) {
            mybuttons[i].update();
        }
        myencoder.update();
        // if (counter != counter_last) {
        //     counter_last = counter;
        //     out.print("counter changed: ");
        //     out.print(counter);
        //     out.println();
        //     animation.brightness = map_range(counter, 0, 500, 0.0, 1.0);
        // }

        // out.println("update.");
        // delay(100);
    }
}
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// menu

void MyInput::menu__test_xxx(Print& out) {
    out.println(F("TODO"));
    out.println();
}

void MyInput::menu__set_yyy(Print& out, char* command) {
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

void MyInput::menu__set_test_token(Print& out, char* command) {
    out.println(F("Set yyy "));
    out.println(F("TODO"));

    // char * pCh;
    // pCh = strtok_r(&caCommand[1], " :");
    // byte bIndex = (byte)atoi(pCh);
    // pCh = strtok_r(NULL, " :");
    // byte bPSC = (byte)atoi(pCh);
    // pCh = strtok_r(NULL, " :");
    // byte bPWM = (byte)atoi(pCh);
    //
    // out.print(F("\t bIndex: "));
    // out.println(bIndex);
    // out.print(F("\t bPSC: "));
    // out.println(bPSC);
    // out.print(F("\t bPWM: "));
    // out.println(bPWM);

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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ambientlight sensor

// void MyInput::als_setup(Print &out) {
//     out.println("Ambient Light Sensor:");
//     if (als.begin(out)) {
//         out.println(F("found TSL2591 sensor"));
//         out.println(F("------------------------------------------"));
//         als.tsl.setConfig(&als.sens_conf[0]);
//         out.println(F("------------------------------------------"));
//         als.sensor_print_details(out);
//         out.println(F("------------------------------------------"));
//         als.tsl.printConfig(out);
//         out.println(F("------------------------------------------"));
//         for (size_t i = 0; i < 10; i++) {
//             als.update();
//             // delay(110);
//         }
//     } else {
//         out.println("No sensor found. → please check your wiring..");
//     }
//     out.println();
// }
//
// void MyInput::als_update(Print &out) {
//     als.update();
//     als_handle_sens_conf_change(out);
//     if (als_debugout_enabled) {
//         if ((millis() - als_debugout_timeStamp) > 1000) {
//             als_debugout(out);
//             als_debugout_timeStamp = millis();
//         }
//     }
//     als_handle_lux_change(out);
// }
//
// void MyInput::als_handle_sens_conf_change(Print &out) {
//     if (als.sensitivity_config_changed()) {
//         als_debugout_sens_conf_change(out);
//         als.sensitivity_config_changed_clear();
//     }
// }
//
// void MyInput::als_handle_lux_change(Print &out) {
//     if (als.lux_filtered_changed()) {
//         out.print("update: ");
//         out.print(als.get_lux_filtered(), 4);
//         out.print("LUX");
//         // out.println();
//
//         // als_brightness_automatic = map_range_clamped__double(
//         //     als.get_lux_filtered(),
//         //     // 0.0, 88000.0,
//         //     0.0001, 2000.0,
//         //     0.0005, 1.0);
//         //     // 0.00002, 1.0);
//
//         als_brightness_automatic = light_map.mapit(als.get_lux_filtered());
//         out.print(" -> ");
//         out.print(als_brightness_automatic, 5);
//         out.println();
//         if (als_sets_brightness) {
//             animation.brightness = als_brightness_automatic;
//             // TODO(s-light): implement fading
//         }
//         als.lux_filtered_changed_clear();
//     }
// }
//
// void MyInput::als_debugout(Print &out) {
//     print_runtime(out);
//
//     out.print("  ");
//     out.print(als.get_lux_filtered(), 4);
//     out.print(" lux");
//
//     out.print("    id:");
//     out.print(als.get_sensitivity_config_id());
//     out.print("    full:");
//     out.print(als.get_full_raw());
//     out.print("    lux_raw:");
//     out.print(als.get_lux_raw(), 4);
//
//     // als.print_status(out);
//     out.println();
// }
//
// void MyInput::als_debugout_sens_conf_change(Print &out) {
//     out.println("******************************************");
//     print_runtime(out);
//     out.println();
//
//     out.print("");
//     out.print("sens_conf_current_id:");
//     out.print(als.get_sensitivity_config_id());
//     out.println();
//
//     out.print("sens_conf_changed:");
//     out.print(als.sensitivity_config_changed());
//     out.println();
//     out.print("full_raw:");
//     out.print(als.get_full_raw());
//     out.println();
//     out.print("lux_raw:");
//     out.print(als.get_lux_raw(), 4);
//     out.println();
//     out.println();
//
//     als.tsl.printConfig(out);
//     out.println();
//     out.println("******************************************");
// }
//
// double MyInput::get_als_brightness_automatic(void) {
//     return als_brightness_automatic;
// }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// button

void MyInput::button_init(Stream& out) {
    out.println(F("setup button input:"));
    for (size_t i = 0; i < UIButton_count; i++) {
        out.print(F("  button "));
        out.print(UIButton_names(static_cast<UIButton>(mybuttons[i].id)));
        out.print(F(" on pin "));
        out.println(mybuttons[i].pin);
        pinMode(mybuttons[i].pin, INPUT_PULLUP);
        mybuttons[i].begin();
        mybuttons[i].flag_filter_multi_click_events = true;
    }
    out.println(F("  finished."));
}

boolean MyInput::mybutton_get_input(slight_ButtonInput* instance) {
    // read input + invert: button closes to GND.
    return !digitalRead((*instance).pin);
}

void MyInput::mybutton_event(slight_ButtonInput* instance) {
    // out.print(F("instance:"));
    // out.print((*instance).id);
    // out.print(F(" - event: "));
    // (*instance).printEventLast(out);
    // out.println();

    switch ((*instance).getEventLast()) {
        case slight_ButtonInput::event_down: {
            // out.println(F("down"));
            switch ((*instance).id) {
                case UIButton::up: {
                    out.println("fadeUp");
                    animation.brightnessFader.fadeUp();
                } break;
                case UIButton::down: {
                    out.println("fadeDown");
                    animation.brightnessFader.fadeDown();
                } break;
            }
        } break;
        case slight_ButtonInput::event_holddown: {
            // out.print(F("duration active: "));
            // out.println((*instance).getDurationActive());
        } break;
        case slight_ButtonInput::event_up: {
            // out.println(F("up"));
            switch ((*instance).id) {
                case UIButton::up:
                case UIButton::down: {
                    out.println("fadePause");
                    animation.brightnessFader.fadePause();
                } break;
            }
        } break;
        case slight_ButtonInput::event_click: {
            // out.println(F("click"));
            // switch ((*instance).id) {
            //     case UIButton::up: {
            //         1;
            //     } break;
            //     case UIButton::down: {
            //         1;
            //     } break;
            //     case UIButton::power: {
            //         // animation.brightnessFader.fadeTo(animation.brightness_min);
            //     } break;
            // }
        } break;
        case slight_ButtonInput::event_click_long : {
            // out.print(F("click long "));
            // out.println((*instance).getDurationActive());
            // encoder_mode_brightness = !encoder_mode_brightness;
            // out.print(F("  encoder_mode_brightness: "));
            // out.println(encoder_mode_brightness);
            // powerhandling.get_wakeup_gpio_num()
            switch ((*instance).id) {
                case UIButton::up: {
                    1;
                } break;
                case UIButton::down: {
                    1;
                } break;
                case UIButton::power: {
                    // out.print("power click long");
                    powerhandling.enter_sleep_mode();
                } break;
            }
        } break;
        case slight_ButtonInput::event_click_double: {
            out.println(F("click double"));
            // if (animation.brightness > 0.1) {
            //     animation.set_brightness(0.001);
            // } else {
            //     animation.set_brightness(1.0);
            // }
            // out.print(F("  animation.brightness: "));
            // out.println(animation.brightness, 4);
            switch ((*instance).id) {
                case UIButton::up: {
                    1;
                } break;
                case UIButton::down: {
                    1;
                } break;
                case UIButton::power: {
                    out.println("fade to min..");
                    animation.brightnessFader.fadeTo(animation.brightness_min, 1500);
                    // wait till we are done fading down..
                    while (animation.brightnessFader.getState()
                           == animation.brightnessFader.state_FadingDown) {
                        animation.update();
                        delay(1);
                    }
                    animation.update();
                    powerhandling.enter_sleep_mode();
                } break;
            }
        } break;
            // case slight_ButtonInput::event_click_triple : {
            //     out.println(F("click triple"));
            // } break;
            // case slight_ButtonInput::event_click_multi : {
            //     out.print(F("click multi - count: "));
            //     out.println((*instance).getClickCount());
            // } break;
    }  // end switch
}

// ------------------------------------------
// slight_RotaryEncoder things

void MyInput::encoder_init(
    Stream& out, slight_RotaryEncoder::tCallbackFunctionISR func_ISR
) {
    out.println(F("setup encoder input:"));
    out.println(F("  myencoder.begin()"));
    // myencoder.begin(myencoder_pin_changed_ISR__helper);
    myencoder.begin(func_ISR);
    out.println(F("  finished."));
}

// void myencoder_pin_changed_ISR__helper(slight_RotaryEncoder * instance) {
//   // instance->set(15);
//   instance->updateClassic();
//   // instance->updateGray();
// }
// void MyInput::myencoder_pin_changed_ISR() {
// moved to main .ino file.

void MyInput::myencoder_event(slight_RotaryEncoder* instance) {
    // react on event
    switch ((*instance).getEventLast()) {
        case slight_RotaryEncoder::event_Rotated: {
            // get current data
            // int16_t temp_steps = (*instance).getSteps();
            int16_t temp_stepsAccel = (*instance).getStepsAccelerated();
            // clear data
            (*instance).clearSteps();

            // out.print(F("  steps: "));
            // out.println(temp_steps);
            // out.print(F("  steps accelerated: "));
            // out.println(temp_stepsAccel);
            // counter += temp_stepsAccel;
            if (encoder_mode_brightness) {
                animation.set_brightness(
                    animation.brightness + temp_stepsAccel * 0.0002
                );
                out.print(F("  animation.brightness: "));
                out.println(animation.brightness, 4);
            } else {
                animation.set_hue(animation.hue + temp_stepsAccel * 0.0002);
                out.print(F("  animation.hue: "));
                out.println(animation.hue, 4);
            }
        } break;
            // currently there are no other events fired.
    }  // end switch
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// helper

void MyInput::print_runtime(Print& out) {
    char buffer[] = "[1234567890ms]   \0";
    snprintf(buffer, sizeof(buffer), "[%8lums]", millis());
    out.print(buffer);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// helper

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
