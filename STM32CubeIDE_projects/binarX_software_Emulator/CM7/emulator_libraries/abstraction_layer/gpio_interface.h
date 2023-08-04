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
 * @brief The GPIO interface to allow for dependency injection and testing
 *
 */
class GpioInterface {
 public:
  /**
   * @brief Set the Pin High to high
   *
   * @param GpioSelector gpio is the pin to set high
   *
   */
  virtual void SetHigh(GpioSelector gpio) = 0;
  /**
   * @brief Set the Pin Low to high
   *
   * @param GpioSelector gpio is the pin to set low
   *
   */
  virtual void SetLow(GpioSelector gpio) = 0;
  /**
   * @brief Toggle the pin between low and High
   *
   * @param GpioSelector gpio is the pin to toggle
   *
   */
  virtual void TogglePin(GpioSelector gpio) = 0;
};
}  // namespace binarx_gpio_interface