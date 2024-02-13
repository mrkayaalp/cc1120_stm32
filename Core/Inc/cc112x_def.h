#ifndef __CC112X_DEF_H__
#define __CC112X_DEF_H__
#include "spi.h"

#define CC1120_CS_Pin SPI1_CS_Pin
#define CC1120_CS_GPIO_Port SPI1_CS_GPIO_Port

#define CC1120_INTF &hspi1
#define CC1120_TIMOUT_ms 250

typedef enum
{
    CC1120_OK = 0x00U,
    CC1120_ERROR = 0x01U,
    CC1120_BUSY = 0x02U,
    CC1120_TIMEOUT = 0x03U
} CC1120_StatusTypeDef;

typedef signed   char   int8;
typedef unsigned char   uint8;


typedef signed   short  int16;
typedef unsigned short  uint16;

typedef signed   long   int32;
typedef unsigned long   uint32;

typedef void (*ISR_FUNC_PTR)(void);
typedef void (*VFPTR)(void);


#endif
