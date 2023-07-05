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

/**
 * @brief GPIO Enum that identifies the pins allowed
 *
 */
enum GpioSelector { Button, YellowLed, GreenLed, RedLed, PayloadReady };

/**
 * @brief Gpio enum that identifies the return options.
 *
 */
enum GpioStatus { Success, Timeout };

class GpioInterface {
 public:
  virtual void SetHigh(GpioSelector) = 0;
  virtual void SetLow(GpioSelector) = 0;
  virtual void TogglePin(GpioSelector) = 0;
  virtual GpioStatus WaitForInterrupt(GpioSelector, uint32_t timeout) = 0;
};
}  // namespace binarx_gpio_interface