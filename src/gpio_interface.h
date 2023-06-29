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

class GpioInterface {
 public:
  virtual void SetHigh() = 0;
  virtual void SetLow() = 0;
  virtual void TogglePin() = 0;
};
}  // namespace binarx_gpio_interface