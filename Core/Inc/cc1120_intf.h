#ifndef __CC1120_INTF_H__
#define __CC1120_INTF_H__

#include "cc112x_spi.h"
#include "cc112x_def.h"







/*!
* @brief This API confiures the CC1120 radio with the given RF configuration.
* @param config RFConfig type configuration values
* 
*/
CC1120_StatusTypeDef registerConfig(RfConfig config);

#endif
