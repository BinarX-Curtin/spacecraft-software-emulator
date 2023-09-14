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


/**
 * @brief  The application entry point.
 * @retval int
 */
void emulator_liason_main(void) {

//  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
//   */
//  int kMaxDelayTime = 200;  // miliseconds
//  srand((unsigned)HAL_GetTick());
//
//  StaticJsonDocument<1000> doc;
//
//  // Add values in the document
//  doc["sensor"] = "Test";
//  doc["time"] = 0;
//  doc["memory_doc"] = doc.memoryUsage();
//  doc["memory_buffer"] = 0;
//
//  //
//  //
//  int extra_numbers = 10;
//
//  JsonArray data = doc.createNestedArray("data");
//  for (int i = 0; i < extra_numbers; i++) {
//    if (!data.add(doc.memoryUsage())) {
//      data[0] = i;
//      break;
//    }
//  }
//
//  std::array<uint8_t, 1000> buffer;
//
//  while (1) {
//    // Test different delays to make sure the emulator can handle it
//    int random_number = rand() % kMaxDelayTime;
//    doc["time"] = random_number;
//    doc["memory_doc"] = doc.memoryUsage();
//    HAL_Delay(random_number);
//
//    // Data size as defined by emulator largest packet size (+ 1 to add the
//    // null terminator byte)
//    uint16_t kDataSize = static_cast<uint16_t>(measureJsonPretty(doc) + 1);
//
//    doc["memory_buffer"] = kDataSize;
//
//    serializeJsonPretty(doc, (char *)buffer.data(), kDataSize);
//
//    // To transmit the data we need to call this function
//
//    emulator_liason.Transmit(buffer.data(), kDataSize);
//  }
}
