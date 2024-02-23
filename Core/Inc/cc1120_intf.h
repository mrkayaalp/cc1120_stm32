#ifndef __CC1120_INTF_H__
#define __CC1120_INTF_H__

#include "cc112x_spi.h"
#include "cc112x_def.h"



/*!
* @brief This API confiures the CC1120 radio with the given RF configuration.
* @param[in] cc1120_devTypeDef - CC1120 pointer device structure
* @return RfChipStatus_t
*/
RfChipStatus_t registerConfig(cc1120_devTypeDef* cc1120_dev);

/*!
* @brief This API initialize the CC1120 radio with the given RF configuration.
* @param[in] cc1120_devTypeDef - CC1120 pointer device structure
* @return RfChipStatus_t
*/
RfChipStatus_t rfinit(cc1120_devTypeDef* cc1120_dev);

/*!
* @brief This API sends the given sensor data to the CC1120 radio.
* @param[in] cc1120_devTypeDef - CC1120 pointer device structure
* @param[in] sensorData - Sensor data
* @return RfChipStatus_t
*/
RfChipStatus_t rfSendTxPacket(cc1120_devTypeDef* cc1120_dev, uint8_t sensorData[]);

/*!
* @brief This API sets the CC1120 radio for RX mode.
* @return RfChipStatus_t
*/
RfChipStatus_t rfRunRx();

/*!
* @brief This API recieve the  RX buffer.
* @return RfChipStatus_t
*/
RfChipStatus_t rfRecieveRxPacket();

/*!
* @brief This API configures the  CC1190 amplifier.
* @param[in] cc1120_dev Pointer to the device structure.
* @return void
*/
void configAmplifier(cc1120_devTypeDef* cc1120_dev);


#endif
