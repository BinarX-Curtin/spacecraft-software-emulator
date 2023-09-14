/**
 * @file gpio_impl.h
 * @author Tristan Ward
 * @brief GPIO implementation of the GPIO interface within the stm32CubeIDE
 * project
 *
 */
#pragma once
#include "abstraction_layer/gpio_interface.h"

namespace binarx_gpio_impl {

class GpioImpl : public binarx_gpio_interface::GpioInterface {
 public:
  GpioImpl(){};
  void SetHigh(binarx_gpio_interface::GpioSelector gpio) override;
  void SetLow(binarx_gpio_interface::GpioSelector gpio) override;
  void TogglePin(binarx_gpio_interface::GpioSelector gpio) override;
};
}  // namespace binarx_gpio_impl
