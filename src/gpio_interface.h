/**
 * @file gpio_interface.h
 * @author Tristan Ward
 * @brief GPIO interface heather file to create selectable
 * dependencies
 *
 */

#pragma once
#include <stdint.h>
namespace binarx_gpio_interface {

enum GpioSelector { Button, YellowLed, GreenLed, RedLed, PayloadReady };

class GpioInterface {
 public:
  virtual void SetHigh(GpioSelector) = 0;
  virtual void SetLow(GpioSelector) = 0;
  virtual void TogglePin(GpioSelector) = 0;
  virtual void WaitForInterrupt(GpioSelector) = 0;
};
}  // namespace binarx_gpio_interface