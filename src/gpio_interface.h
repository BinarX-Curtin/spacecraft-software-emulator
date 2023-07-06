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
  /**
   * @brief Wait for the pin to go high
   *
   * @param gpio The Gpio to check if it goes high
   * @param timeout The ammount of time to wait for the pin to go high
   * @return GpioStatus Is the return status of the function
   */
  virtual GpioStatus WaitForInterrupt(GpioSelector gpio, uint32_t timeout) = 0;
};
}  // namespace binarx_gpio_interface