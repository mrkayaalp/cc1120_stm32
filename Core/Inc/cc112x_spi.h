/*****************************************************************************/
//  @file   CC112x_spi.h  
//  
//  @brief  header file that defines a minimum set of neccessary functions
//          to communicate with CC112X over SPI as well as the regsister 
//          mapping. 
//				 
//  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/****************************************************************************/

#ifndef CC112x_SPI_H
#define CC112x_SPI_H

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 * INCLUDES
 */
#ifdef MSP_EXP430G2
  #include "hal_msp_exp430g2_spi.h"
#else
  #include "hal_spi_rf_trxeb.h"  

#endif

/******************************************************************************
 * CONSTANTS
 */







/******************************************************************************
 * PROTPTYPES
 */ 
CC1120_StatusTypeDef cc1120SpiTRx(uint8_t *txbuffer, uint8_t *rxbuffer);
void  cc1120_startTransaction();
void cc1120_endTransaction();
/* basic set of access functions */
rfStatus_t cc112xSpiReadReg(uint16 addr, uint8 *data, uint8 len);
rfStatus_t cc112xGetTxStatus(void);
rfStatus_t cc112xGetRxStatus(void);  
rfStatus_t cc112xSpiWriteReg(uint16 addr, uint8 *data, uint8 len);
rfStatus_t cc112xSpiWriteTxFifo(uint8 *pWriteData, uint8 len);
rfStatus_t cc112xSpiReadRxFifo(uint8 *pReadData, uint8 len);

#ifdef  __cplusplus
}
#endif
#endif// CC112x_SPI_H
