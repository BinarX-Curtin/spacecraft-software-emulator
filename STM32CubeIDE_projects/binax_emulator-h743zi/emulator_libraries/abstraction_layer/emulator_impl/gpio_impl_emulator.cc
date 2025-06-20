/**
 * @file gpio_impl.cc
 * @author Tristan Ward
 * @brief gpio implementation file for the Stm32CubeIDE project
 *
 */

#include "abstraction_layer/inc/gpio_impl.h"
#include "main.h"

namespace binarx_gpio_impl {

uint16_t GetPin(binarx_gpio_interface::GpioSelector gpio) {
  switch (gpio) {
    case binarx_gpio_interface::GpioSelector::YellowLed:
      return LD2_Pin;
    case binarx_gpio_interface::GpioSelector::GreenLed:
      return LD1_Pin;
    case binarx_gpio_interface::GpioSelector::RedLed:
      return LD3_Pin;
  }
};
GPIO_TypeDef* GetPort(binarx_gpio_interface::GpioSelector gpio) {
  switch (gpio) {
    case binarx_gpio_interface::GpioSelector::YellowLed:
      return LD2_GPIO_Port;
    case binarx_gpio_interface::GpioSelector::GreenLed:
      return LD1_GPIO_Port;
    case binarx_gpio_interface::GpioSelector::RedLed:
      return LD3_GPIO_Port;
  }
};

void GpioImpl::SetHigh(binarx_gpio_interface::GpioSelector gpio) {
  HAL_GPIO_WritePin(GetPort(gpio), GetPin(gpio), GPIO_PIN_SET);
}
void GpioImpl::SetLow(binarx_gpio_interface::GpioSelector gpio) {
  HAL_GPIO_WritePin(GetPort(gpio), GetPin(gpio), GPIO_PIN_RESET);
}
void GpioImpl::TogglePin(binarx_gpio_interface::GpioSelector gpio) {
  HAL_GPIO_TogglePin(GetPort(gpio), GetPin(gpio));
}

}  // namespace binarx_gpio_impl
