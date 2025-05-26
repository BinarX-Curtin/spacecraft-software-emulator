/**
 * @file fgd02f.c
 * @author Will Blackie
 * @brief This file contains the function implementations for interacting with
 * the fgd02f Dosimeter device.
 *
 */

 #include "fgd02f.h"

fgd02f_t _dosimeter = { NULL, NULL };

uint32_t FGD02FWrite(uint8_t *write_buf,
                         uint16_t len) {
  return _dosimeter.write_command(write_buf, len);
}
 
uint32_t FGD02FRead(uint8_t *read_buf,
                        uint16_t len) {
  return _dosimeter.read_command(read_buf, len);
}
 
void FGD02FInit(const fgd02f_t dosimeter){
  // assign the dosimeter to the global variable
  _dosimeter = dosimeter;

  // Check if the device ID matches the expected value
  uint8_t pID = GetPid();
  if (pID != CHIPID_VAL) {
    // Handle error: incorrect device ID
    return;
  }  

  // Set up the device
  uint8_t write_buf[2] = {WRITE | REFWINDOW, 0b01000000 }; // Set 1 second long measurement window in HS
  FGD02FWrite(write_buf, sizeof(write_buf));

  write_buf[0] = (WRITE | CHARGECFG_1); // Set high sensitivity and force charge off
  write_buf[1] = 0b01111001;
  FGD02FWrite(write_buf, sizeof(write_buf));

  write_buf[0] = (WRITE | CHARGECFG_2);  // turn off charging
  write_buf[1] = 0b00000000;
  FGD02FWrite(write_buf, sizeof(write_buf));

  write_buf[0] = (WRITE | TARGET);  // Set target frequency to current reference
  write_buf[1] = 0b00110011;
  FGD02FWrite(write_buf, sizeof(write_buf));

  write_buf[0] = (WRITE | THRES);  // Set the threshold frequency 30kHz in HS
  write_buf[1] = 0b00011101;
  FGD02FWrite(write_buf, sizeof(write_buf));

  write_buf[0] = (WRITE | CFG);  // enable interrupt on new data
  write_buf[1] = 0b01110000;
  FGD02FWrite(write_buf, sizeof(write_buf));

  write_buf[0] = (WRITE | CHARGECFG_2);  // Set automatic recharge mode and SET ECH high
  write_buf[1] = 0b01000001;
  FGD02FWrite(write_buf, sizeof(write_buf));

}

uint8_t GetPid() {
  uint8_t write_buf[1] = {READ | CHIPID};
  uint8_t read_buf[1] = {0};

  FGD02FWrite(write_buf, sizeof(write_buf));
  FGD02FRead(read_buf, sizeof(read_buf));

  return *read_buf;
}
 
// uint32_t SensorCounter() {
//   uint8_t write_buf[2] = {INTERNAL_CONTROL_0, (INT_MEAS_DONE_EN | TM_M)};
//   return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
// }

// uint32_t ReferenceCounter() {
//   uint8_t write_buf[2] = {INTERNAL_CONTROL_0, (INT_MEAS_DONE_EN | TM_M)};
//   return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
// }
 
// uint32_t TemperatureMeasurement() {
//   uint8_t write_buf[2] = {INTERNAL_CONTROL_0, (INT_MEAS_DONE_EN | TM_T)};
//   return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
// }
 
 uint32_t GetReference() {
   uint8_t write_buf[1] = {READ | F1R_1};

   uint8_t read_buf[3] = {0};

   FGD02FWrite(write_buf, sizeof(write_buf));
   FGD02FRead(read_buf, sizeof(read_buf));

   uint32_t reference = ((read_buf[2] & 0x03) << 16) | (read_buf[1] << 8) | read_buf[0];

   return reference;
 }

 uint32_t GetSensor() {
   uint8_t write_buf[1] = {READ | F1S_1};

   uint8_t read_buf[3] = {0};

   FGD02FWrite(write_buf, sizeof(write_buf));
   FGD02FRead(read_buf, sizeof(read_buf));

   uint32_t sensor = ((read_buf[2] & 0x03) << 16) | (read_buf[1] << 8) | read_buf[0];

   return sensor;
 }

 uint8_t GetTemp() {
    uint8_t write_buf[1] = {READ | TEMP};

    uint8_t read_buf[1] = {0};

    FGD02FWrite(write_buf, sizeof(write_buf));
    FGD02FRead(read_buf, sizeof(read_buf));

    uint32_t temp = read_buf[0];

    return temp;
  }
 
// uint8_t GetTemp(const mmc5983ma_t *magnetometer) {
//   uint8_t write_buf[1] = {TEMP_OUT};
//   uint8_t read_buf[1] = {0};

//   Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
//   Mmc5983maRead(magnetometer, read_buf, sizeof(read_buf));

//   return *read_buf;
// }
 
// uint32_t ClearMagFieldInt(const mmc5983ma_t *magnetometer) {
//    uint8_t write_buf[2] = {STATUS, MEAS_M_DONE};
//    return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
// }
 
// uint32_t ClearTempInt(const mmc5983ma_t *magnetometer) {
//    uint8_t write_buf[2] = {STATUS, MEAS_T_DONE};
//    return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
// }
 
// uint32_t Set(const mmc5983ma_t *magnetometer) {
//    uint8_t write_buf[2] = {INTERNAL_CONTROL_0, MAG_SET};
//    return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
// }
 
// uint32_t Reset(const mmc5983ma_t *magnetometer) {
//    uint8_t write_buf[2] = {INTERNAL_CONTROL_0, MAG_RESET};
//    return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
// }
