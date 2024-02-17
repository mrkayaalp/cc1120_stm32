#include "cc1120_intf.h"

uint16_t rxPacketCounter = 0;
uint16_t txPacketCounter = 0;

uint8_t rxBuffer[128] = {0};
uint8_t rxBytes;
uint8_t marcState;

static uint8_t rssi2compl,rssiValid;
static uint8_t rssiOffset = 102;
static int8_t rssiConverted;

uint8_t sensorData[PKTLEN - 3] = { 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB };
uint8_t rfTxPacket[PKTLEN] = { 0x00 };

/*!
* @brief This API confiures the CC1120 radio with the given RF configuration.
*/
CC1120_StatusTypeDef registerConfig(RfConfig config) {
  uint8 writeByte;

  trxSpiCmdStrobe(CC112X_SRES);

  if (config == _4GFSK_200kbps) {
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

CC1120_StatusTypeDef rfinit() {
  //Configure register settings
  registerConfig(_4GFSK_200kbps);

}

/*---------------------TX------------------------------------*/
void rfSendTxPacket(uint8_t sensorData[]) {
  // Create packet
  createRfTxPacket(sensorData);

  // Write packet to TX FIFO
  cc112xSpiWriteTxFifo(rfTxPacket, sizeof(rfTxPacket));


  // Strobe TX to Send packet
  trxSpiCmdStrobe(CC112X_STX);
}

static void createRfTxPacket(uint8_t sensorData[]) {

  rfTxPacket[0] = PKTLEN;                           // Length byte
  rfTxPacket[1] = (uint8_t)(txPacketCounter >> 8);     // MSB of packetCounter
  rfTxPacket[2] = (uint8_t)txPacketCounter;           // LSB of packetCounter

  // Fill rest of the buffer with the sensor data
  rfTxPacket[3] = sensorData[0];
  rfTxPacket[4] = sensorData[1];
  rfTxPacket[5] = sensorData[2];
  rfTxPacket[6] = sensorData[3];
  rfTxPacket[7] = sensorData[4];
  rfTxPacket[8] = sensorData[5];
  rfTxPacket[9] = sensorData[6];

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