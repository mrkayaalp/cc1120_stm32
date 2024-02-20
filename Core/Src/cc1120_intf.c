#include "cc1120_intf.h"

cc1120_devTypeDef cc1120_dev;

uint16_t rxPacketCounter = 0;
uint16_t txPacketCounter = 0;

uint8_t rxBuffer[128] = {0};
uint8_t rxBytes;
uint8_t marcState;

static uint8_t rssi2compl,rssiValid;
static uint8_t rssiOffset = 102;
static int8_t rssiConverted;

uint8_t sensorData[PKTLEN - 3] = { 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB }; //Dummy datas we won't use it in real application
uint8_t rfTxPacket[PKTLEN] = { 0x00 };


/***** Static Function Declarations****/

/*!
* @brief This API creates TX packet with the given sensor data with configured length.
* @param[in] cc1120_dev Pointer to the device structure.
* @param[in] sensorData Pointer to the sensor data.
* @return void
*/
static void createRfTxPacket(cc1120_devTypeDef *cc1120_dev, uint8_t sensorData[]);

/*!
* @brief This API confiures the CC1120 radio with the given RF configuration.
*/
CC1120_StatusTypeDef registerConfig(cc1120_devTypeDef *cc1120_dev) {
  uint8 writeByte;

  trxSpiCmdStrobe(CC112X_SRES);

  if (cc1120_dev->rfconfig == _4GFSK_200kbps) {
    for (uint16 i = 0;
      i < (sizeof(preferredSettingsMaxDR) / sizeof(registerSetting_t)); i++) {
      writeByte = preferredSettingsMaxDR[i].data;
      cc112xSpiWriteReg(preferredSettingsMaxDR[i].addr, &writeByte, 1);
    }
  }
  else {
    for (uint16 i = 0;
      i < (sizeof(preferredSettingsSens) / sizeof(registerSetting_t)); i++) {
      writeByte = preferredSettingsSens[i].data;
      cc112xSpiWriteReg(preferredSettingsSens[i].addr, &writeByte, 1);
    }
  }
}

CC1120_StatusTypeDef rfinit(cc1120_devTypeDef *cc1120_dev) {
  //Set configuration settings in device
  cc1120_dev->intf = CC1120_INTF;
  cc1120_dev->packetLength = PKTLEN;
  cc1120_dev->rfconfig = _4GFSK_200kbps;
  cc1120_dev->rfmode = RF_MODE_TX;
  cc1120_dev->amp_hgm =  Amp_HGM_ON;

  //Configure register settings
  registerConfig(cc1120_dev);

  if(cc1120_dev->rfmode == RF_MODE_TX){
    rfSendTxPacket(cc1120_dev, sensorData); //Wait for tx interrupt and send data again
  }
  else{
    rfInitRx(); // Wait for rx interrupt and receive data
  }

}

/*---------------------TX------------------------------------*/
void rfSendTxPacket(cc1120_devTypeDef *cc1120_dev, uint8_t sensorData[]) {
  // Create packet
  createRfTxPacket(cc1120_dev, sensorData);

  // Write packet to TX FIFO
  cc112xSpiWriteTxFifo(rfTxPacket, sizeof(rfTxPacket));


  // Strobe TX to Send packet
  trxSpiCmdStrobe(CC112X_STX); 
}

static void createRfTxPacket(cc1120_devTypeDef *cc1120_dev, uint8_t sensorData[]) {

  rfTxPacket[0] = cc1120_dev->packetLength;                           // Length byte
  rfTxPacket[1] = (uint8_t)(txPacketCounter >> 8);     // MSB of packetCounter
  rfTxPacket[2] = (uint8_t)txPacketCounter;           // LSB of packetCounter

  // Fill rest of the buffer with the sensor data
  for(uint8_t i = 3; i < (cc1120_dev->packetLength); i++){
    rfTxPacket[i] = sensorData[i - 3];
  }
}
/*-------------------------------RX--------------------------*/
void rfInitRx() {
  // Strobe RX to receive packet
  trxSpiCmdStrobe(CC112X_SRX);

}

void rfRecieveRxPacket() {
  cc112xSpiReadReg(CC112X_NUM_RXBYTES, &rxBytes, 1);

  // Check that we have bytes in FIFO
  if (rxBytes != 0) {

    // Read MARCSTATE to check for RX FIFO error
    cc112xSpiReadReg(CC1120_MARCSTATE, &marcState, 1);

    // Mask out MARCSTATE bits and check if we have a RX FIFO error
    if ((marcState & 0x1F) == RX_FIFO_ERROR) {

      // Flush RX FIFO
      trxSpiCmdStrobe(CC112X_SFRX);
    }
    else {

      // Read n bytes from RX FIFO
      cc112xSpiReadRxFifo(rxBuffer, rxBytes);

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
}

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

  if(rssiValid & 0x01) {
    cc112xSpiReadReg(CC1120_RSSI1, &rssi2compl, 1);
    rssiConverted = (int8_t)rssi2compl - rssiOffset;
  }
}
