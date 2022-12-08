/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "main.h"
#include "DigitalOut.h"
#include "Thread.h"
#include "mbed.h"
#include "measure_thread.h"
#include <cstdio>

int system_mode = TEST_MODE;
InterruptIn mode_button(PB_2);

Thread thread;

bool on_measure = false;
Ticker ticker;
bool on_resume = false;
Ticker resumeTicker;

DigitalOut led1(PB_5);
DigitalOut led2(PA_5);
DigitalOut led3(PB_6);

void activate_measure() { on_measure = true; }

void send_resume() { on_resume = true; }

void change_mode(void) {
  system_mode += 1;
  if (system_mode > ADVANCE_MODE) {
    system_mode = 0;
  }
  on_measure = false;
  on_resume = false;
  ticker.detach();
  resumeTicker.detach();
  switch (system_mode) {
  case TEST_MODE:
    led1.write(1);
    led2.write(0);
    led3.write(0);
    ticker.attach(&activate_measure, 2s);
    break;
  case NORMAL_MODE:
    led1.write(0);
    led2.write(1);
    led3.write(0);
    // ticker.attach(&activate_measure, 2s);
    // resumeTicker.attach(&send_resume, 30s);
    ticker.attach(&activate_measure, 30s);
    resumeTicker.attach(&send_resume, 1h);
    break;
  case ADVANCE_MODE:
    led1.write(0);
    led2.write(0);
    led3.write(1);
    // ticker.attach(&activate_measure, 2s);
    // resumeTicker.attach(&send_resume, 30s);
    ticker.attach(&activate_measure, 30s);
    resumeTicker.attach(&send_resume, 1h);
    break;
  default:
    break;
  }
}

int main() {
  mode_button.mode(PullUp);
  mode_button.fall(&change_mode);

  thread.start(measure_thread);

  led1.write(1);
  led2.write(0);
  led3.write(0);
  ticker.attach(&activate_measure, 2s);

  while (true) {
    wait_us(2000);
  }
}
