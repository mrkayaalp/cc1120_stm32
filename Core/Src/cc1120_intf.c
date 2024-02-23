#include "cc1120_intf.h"

cc1120_devTypeDef cc1120_dev;

uint16_t rxPacketCounter = 0;
uint16_t txPacketCounter = 0;

uint8_t rxBuffer[128] = { 0 };
uint8_t rxBytes;
uint8_t marcState;

static uint8_t rssi2compl, rssiValid;
static uint8_t rssiOffset = 102;
static int8_t rssiConverted;

uint8_t sensorData[PKTLEN - 3] = { 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB }; //Dummy datas we won't use it in real application
uint8_t rfTxPacket[PKTLEN] = { 0x00 };


/*---------------Static Function Declarations------------------*/

/*!
* @brief This API creates TX packet with the given sensor data with configured length.
* @param[in] cc1120_dev Pointer to the device structure.
* @param[in] sensorData Pointer to the sensor data.
* @return void
*/
static void createRfTxPacket(cc1120_devTypeDef* cc1120_dev, uint8_t sensorData[]);

/*!
* @brief This API sets the mode of the amplifier.
* @param[in] cc1120_dev Pointer to the device structure.
* @return void
*/
static void setAmpMode(cc1120_devTypeDef* cc1120_dev);

/*!
* @brief This API sets the direction of the amplifier RX/TX.
* @param[in] cc1120_dev Pointer to the device structure.
* @return void
*/
static void setAmpDirection(cc1120_devTypeDef* cc1120_dev);


/*---------------------Public Functions------------------------*/




/*!
* @brief This API confiures the CC1120 radio with the given RF configuration.
*/
RfChipStatus_t registerConfig(cc1120_devTypeDef* cc1120_dev) {
  uint8 writeByte;
  RfChipStatus_t chipstatus;

  chipstatus = trxSpiCmdStrobe(CC112X_SRES);

  if (cc1120_dev->rfconfig == _4GFSK_200kbps) {
    for (uint16 i = 0;
      i < (sizeof(preferredSettingsMaxDR) / sizeof(registerSetting_t)); i++) {
      writeByte = preferredSettingsMaxDR[i].data;
      chipstatus = cc112xSpiWriteReg(preferredSettingsMaxDR[i].addr, &writeByte, 1);
    }
  }
  else {
    for (uint16 i = 0;
      i < (sizeof(preferredSettingsSens) / sizeof(registerSetting_t)); i++) {
      writeByte = preferredSettingsSens[i].data;
      chipstatus = cc112xSpiWriteReg(preferredSettingsSens[i].addr, &writeByte, 1);
    }
  }
  return chipstatus;
}
/*!
* @brief This API initialize the CC1120 radio with the given RF configuration.
*/
RfChipStatus_t rfinit(cc1120_devTypeDef* cc1120_dev) {
  RfChipStatus_t chipstatus;
  //Set configuration settings in device
  cc1120_dev->intf = CC1120_INTF;
  cc1120_dev->packetLength = PKTLEN;
  cc1120_dev->rfconfig = _4GFSK_200kbps;
  cc1120_dev->rfmode = RF_MODE_TX;
  cc1120_dev->amp_mode = AMP_HGM_ON;

  //Set GPIOs for amplifier
  configAmplifier(cc1120_dev);

  //Configure register settings
  chipstatus = registerConfig(cc1120_dev);

  if (cc1120_dev->rfmode == RF_MODE_TX) {
    chipstatus = rfSendTxPacket(cc1120_dev, sensorData); //Wait for tx interrupt and send data again
  }
  else {
    chipstatus = rfInitRx(); // Wait for rx interrupt and receive data
  }
  return chipstatus;
}

/*---------------------TX------------------------------------*/

/*!
* @brief This API sends the given sensor data to the CC1120 radio.
*/
RfChipStatus_t rfSendTxPacket(cc1120_devTypeDef* cc1120_dev, uint8_t sensorData[]) {
  RfChipStatus_t chipstatus;
  // Create packet
  createRfTxPacket(cc1120_dev, sensorData);

  // Write packet to TX FIFO
  chipstatus = cc112xSpiWriteTxFifo(rfTxPacket, sizeof(rfTxPacket));


  // Strobe TX to Send packet
  chipstatus = trxSpiCmdStrobe(CC112X_STX);
  return chipstatus;
}

static void createRfTxPacket(cc1120_devTypeDef* cc1120_dev, uint8_t sensorData[]) {

  rfTxPacket[0] = cc1120_dev->packetLength;                           // Length byte
  rfTxPacket[1] = (uint8_t)(txPacketCounter >> 8);     // MSB of packetCounter
  rfTxPacket[2] = (uint8_t)txPacketCounter;           // LSB of packetCounter

  // Fill rest of the buffer with the sensor data
  for (uint8_t i = 3; i < (cc1120_dev->packetLength); i++) {
    rfTxPacket[i] = sensorData[i - 3];
  }
}

/*-------------------------------RX--------------------------*/

/*!
* @brief This API sets the CC1120 radio for RX mode.
*/
RfChipStatus_t rfRunRx() {
  RfChipStatus_t chipstatus;
  // Strobe RX to receive packet
  chipstatus = trxSpiCmdStrobe(CC112X_SRX);
}

/*!
* @brief This API recieve the  RX buffer.
*/
RfChipStatus_t rfRecieveRxPacket() {
  RfChipStatus_t chipstatus;
  chipstatus = cc112xSpiReadReg(CC112X_NUM_RXBYTES, &rxBytes, 1);

  // Check that we have bytes in FIFO
  if (rxBytes != 0) {

    // Read MARCSTATE to check for RX FIFO error
    chipstatus = cc112xSpiReadReg(CC1120_MARCSTATE, &marcState, 1);

    // Mask out MARCSTATE bits and check if we have a RX FIFO error
    if ((marcState & 0x1F) == RX_FIFO_ERROR) {

      // Flush RX FIFO
      chipstatus = trxSpiCmdStrobe(CC112X_SFRX);
    }
    else {

      // Read n bytes from RX FIFO
      chipstatus = cc112xSpiReadRxFifo(rxBuffer, rxBytes);

      // Check CRC ok (CRC_OK: bit7 in second status byte)
      // This assumes status bytes are appended in RX_FIFO
      // (PKT_CFG1.APPEND_STATUS = 1)
      // If CRC is disabled the CRC_OK field will read 1
      if (rxBuffer[rxBytes - 1] & 0x80) {

        // Update packet counter
        rxPacketCounter++;
      }
    }
  }
  return chipstatus;
}


/*----------------------Amplifier Functions------------------------------*/

// Table for PA_EN, LNA_EN, HGM, and Mode of Operation
// PA_EN | LNA_EN | HGM | Mode of Operation
//   0   |   0    |  X  |    Power Down
//   0   |   1    |  0  |    RX LGM
//   0   |   1    |  1  |    RX HGM
//   1   |   0    |  0  |    TX LGM
//   1   |   0    |  1  |    TX HGM



/*!
* @brief This API configures the  CC1190 amplifier.
*/
void configAmplifier(cc1120_devTypeDef* cc1120_dev) {
  setAmpMode(cc1120_dev);
  setAmpDirection(cc1120_dev);

}

/*!
* @brief This API sets the mode of the amplifier.
*/
static void setAmpMode(cc1120_devTypeDef* cc1120_dev) {
  if (cc1120_dev->amp_mode == AMP_POWER_DOWN) {
    HAL_GPIO_WritePin(AMP_PA_PORT, AMP_PA_PIN, 0);
    HAL_GPIO_WritePin(AMP_LNA_PORT, AMP_LNA_PIN, 0);

  }
  else {
    HAL_GPIO_WritePin(AMP_HGM_PORT, AMP_HGM_PORT, cc1120_dev->amp_mode);
  }
}

/*!
* @brief This API sets the direction of the amplifier RX/TX.
*/
static void setAmpDirection(cc1120_devTypeDef* cc1120_dev) {
  if (cc1120_dev->rfmode == RF_MODE_TX) {
    HAL_GPIO_WritePin(AMP_PA_PORT, AMP_PA_PIN, 1);
    HAL_GPIO_WritePin(AMP_LNA_PORT, AMP_LNA_PIN, 0);
  }
  else {
    HAL_GPIO_WritePin(AMP_PA_PORT, AMP_PA_PIN, 0);
    HAL_GPIO_WritePin(AMP_LNA_PORT, AMP_LNA_PIN, 1);
  }
}


/*---------------------RSSI------------------------------------*/

/*******************************************************************************
*   @fn         updateRssi
*
*   @brief      Reads the RSSI register if RSSI_VALID is asserted and do
*               necessary conversions. Calculates the average RSSI based on the
*               last 10 packets
*
*   @param      none
*
*   @return     none
*/
static void updateRssi(void) {
  static uint8 index = 0;

  cc112xSpiReadReg(CC1120_RSSI0, &rssiValid, 1); // Is rssi calculation valid?

  if (rssiValid & 0x01) {
    cc112xSpiReadReg(CC1120_RSSI1, &rssi2compl, 1);
    rssiConverted = (int8_t)rssi2compl - rssiOffset;
  }
}
