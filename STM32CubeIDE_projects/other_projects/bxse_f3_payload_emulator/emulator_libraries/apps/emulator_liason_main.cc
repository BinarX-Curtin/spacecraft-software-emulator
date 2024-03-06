#include <stdlib.h>

#include "abstraction_layer/gpio/public/emulator_liason_gpi.h"
#include "abstraction_layer/gpio/public/emulator_liason_gpo.h"
#include "abstraction_layer/inc/serial_impl.h"
#include "emulator_liason/public/emulator_liason.h"
#include "external_libraries/ArduinoJson-v6.21.3.h"
#include "main.h"

bsf::hal::gpio::Gpo<bsf::hal::gpio::GpoPin::kPayloadReady> gpo_payload_ready;
bsf::hal::gpio::Gpi<bsf::hal::gpio::GpiPin::kPayloadChipSelect>
    gpi_payload_chip_select;
binarx_serial_impl::SpiImpl spi_controller = binarx_serial_impl::SpiImpl();
binarx::emulator_liason::EmulatorLiason emulator_liason =
    binarx::emulator_liason::EmulatorLiason(spi_controller, gpo_payload_ready);

 extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
   switch (GPIO_Pin) {
     case Payload_Chip_Select_Pin:
       emulator_liason.ChipSelectInterrupt();
       break;
     default:
       break;
   }
 }

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  emulator_liason.TransmitCallBackInterrupt();
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  uint16_t kDataSize =
          200;
  std::array<uint8_t, 500> buffer;
    buffer.fill(65);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */
	  emulator_liason.Transmit(buffer.data(), kDataSize);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

