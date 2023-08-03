/**
 * @file gpio_impl.h
 * @author Tristan Ward
 * @brief GPIO implementation of the GPIO interface within the stm32CubeIDE
 * project
 *
 */

#include "gpio_interface.h"
#include "main.h"

namespace binarx_gpio_impl {

class GpioImpl : public binarx_gpio_interface::GpioInterface {
 public:
  GpioImpl(){};
  void SetHigh(binarx_gpio_interface::GpioSelector gpio);
  void SetLow(binarx_gpio_interface::GpioSelector gpio);
  void TogglePin(binarx_gpio_interface::GpioSelector gpio);

 private:
  uint16_t GetPin(binarx_gpio_interface::GpioSelector gpio);
  GPIO_TypeDef* GetPort(binarx_gpio_interface::GpioSelector gpio);
};
}  // namespace binarx_gpio_impl
