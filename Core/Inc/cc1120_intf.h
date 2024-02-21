#ifndef __CC1120_INTF_H__
#define __CC1120_INTF_H__

#include "cc112x_spi.h"
#include "cc112x_def.h"



/*!
* @brief This API confiures the CC1120 radio with the given RF configuration.
* @param cc1120_devTypeDef - CC1120 pointer device structure
*/
CC1120_StatusTypeDef registerConfig(cc1120_devTypeDef *cc1120_dev);

/*!
* @brief This API configures the  CC1190 amplifier.
* @param[in] cc1120_dev Pointer to the device structure.
* @return void
*/
void configAmplifier(cc1120_devTypeDef* cc1120_dev);

/*!
* @brief This API sets the CC1120 radio for RX mode.
* @return void
*/
void rfRunRx();

#endif
