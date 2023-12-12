// NOLINT(legal/copyright)

/******************************************************************************

    __doc__ = """
    power handling
    """

    libraries used:
        ~ Adafruit_
            written by Adafruit
        ~
            written by Stefan Krüger (s-light),
                github@s-light.eu, http://s-light.eu,
https://github.com/s-light/ license: MIT

    written by Stefan Krüger (s-light),
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

#include "./powerhandling.h"
#include <cstdint>
#include "Arduino.h"

#include "esp32-hal-gpio.h"
#include "driver/rtc_io.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// class base

RTC_DATA_ATTR int wakeup_failed_count = 0;

PowerHandling::PowerHandling(Stream& out_) : out(out_) {
    ready = false;
}

PowerHandling::~PowerHandling() {
    end();
}

void PowerHandling::begin() {
    end();
    if (ready == false) {
        if (batteryMonitor.begin()) {
            out.print(F("Found MAX17048"));
            out.print(F(" with Chip ID: 0x"));
            out.println(batteryMonitor.getChipID(), HEX);
        } else {
            out.println(F("Couldnt find Adafruit MAX17048."));
        }

        out.println("PowerHandling");
        out.println("    setup TFT_I2C_POWER pin.");
        pinMode(TFT_I2C_POWER, OUTPUT);
        out.println("    setup NEOPIXEL_POWER pin.");
        pinMode(NEOPIXEL_POWER, OUTPUT);


        print_wakeup_reason();

        // you have to hold the power button for about 2sec to start system..
        // if power button is not pushed go back to sleep..
        // and we are woken up from ext1
        pinMode(A0, INPUT_PULLUP);
        if ((digitalRead(A0) == HIGH)
            and (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1)) {
            ++wakeup_failed_count;
             enter_sleep_mode();
        } else {
            out.println("    setup psu_enable_pin pin.");
            pinMode(psu_enable_pin, OUTPUT);
            out.println("    psu enable.");
            digitalWrite(psu_enable_pin, HIGH);
        }
        out.println("done.");
        ready = true;
    }
}

void PowerHandling::end() {
    if (ready) {
        // nothing to do..
    }
}

void PowerHandling::update() {
    if (ready) {
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// public functions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void PowerHandling::print_battery_state() {
    out.print(F("Batt Voltage: "));
    out.print(batteryMonitor.cellVoltage(), 3);
    out.println(" V");
    out.print(F("Batt Percent: "));
    out.print(batteryMonitor.cellPercent(), 1);
    out.println(" %");
    out.println();
}

void PowerHandling::enter_sleep_mode() {
    out.println("prepare sleep mode...");

    out.println("disable power to TFT 12C NeoPixel");
    digitalWrite(TFT_I2C_POWER, LOW);
    digitalWrite(NEOPIXEL_POWER, LOW);

    out.println("shutdown main power. (PSU disable)");
    digitalWrite(psu_enable_pin, LOW);
    pinMode(psu_enable_pin, INPUT_PULLDOWN);
    // make sure the psu_enable_pin is low during deep sleep
    rtc_gpio_pullup_dis(GPIO_NUM_15);
    rtc_gpio_pulldown_en(GPIO_NUM_15);

    out.println("configure wake up pins..");
    // if we only need one gpio:
    // rtc_gpio_pullup_en(GPIO_NUM_18);
    // rtc_gpio_pulldown_dis(GPIO_NUM_18);
    // esp_sleep_enable_ext0_wakeup(GPIO_NUM_18, 0);

    // if we only need multiple gpio:
    // rtc_gpio_pullup_en(GPIO_NUM_16);
    // rtc_gpio_pulldown_dis(GPIO_NUM_16);
    // rtc_gpio_pullup_en(GPIO_NUM_17);
    // rtc_gpio_pulldown_dis(GPIO_NUM_17);
    rtc_gpio_pullup_en(GPIO_NUM_18);
    rtc_gpio_pulldown_dis(GPIO_NUM_18);

    // enable RTC for pullup to work.
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

    // uint64_t gpio_bitmask = std::pow(2, 16) + std::pow(2, 17) + std::pow(2, 18);
    uint64_t gpio_bitmask = std::pow(2, 18);
    esp_sleep_enable_ext1_wakeup(gpio_bitmask, ESP_EXT1_WAKEUP_ANY_LOW);

    out.println("Entering in DEEP Sleep...");
    out.flush();
    esp_deep_sleep_start();
    // this function never returns.
    // on wakeup the full sketch is restarted.

    // out.println("Entering in LIGHT Sleep...");
    // out.flush();
    // esp_light_sleep_start();
}

void PowerHandling::print_wakeup_reason_cause(Stream& out_) {
    // https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:
            out_.println("Wakeup caused by external signal using RTC_IO");
            break;
        case ESP_SLEEP_WAKEUP_EXT1:
            out_.println("Wakeup caused by external signal using RTC_CNTL");
            break;
        case ESP_SLEEP_WAKEUP_TIMER:
            out_.println("Wakeup caused by timer");
            break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            out_.println("Wakeup caused by touchpad");
            break;
        case ESP_SLEEP_WAKEUP_ULP:
            out_.println("Wakeup caused by ULP program");
            break;
        default:
            out_.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
            break;
    }
}
void PowerHandling::print_wakeup_reason_cause() {
    print_wakeup_reason_cause(out);
}

uint8_t PowerHandling::get_wakeup_gpio_num() {
    uint64_t gpio_bitmask = esp_sleep_get_ext1_wakeup_status();
    uint8_t gpio_num = log(gpio_bitmask) / log(2);
    return gpio_num;
}

void PowerHandling::print_wakeup_gpio_num(Stream& out_) {
    out_.printf("GPIO%d\n", get_wakeup_gpio_num());
}
void PowerHandling::print_wakeup_gpio_num() {
    print_wakeup_gpio_num(out);
}

void PowerHandling::print_wakeup_reason() {
    print_wakeup_reason_cause();
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1) {
        print_wakeup_gpio_num();
    }
    out.print("    wakeup_failed_count: ");
    out.println(wakeup_failed_count);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
