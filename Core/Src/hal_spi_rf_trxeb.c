/******************************************************************************
*  Filename: hal_spi_rf_trxeb.c
*
*  Description: Implementation file for common spi access with the CCxxxx
*               tranceiver radios using trxeb. Supports CC1101/CC112X radios
*
*  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/


/******************************************************************************
 * INCLUDES
 */
#include "hal_spi_rf_trxeb.h"
#include "cc112x_def.h"


 /******************************************************************************
  * LOCAL FUNCTIONS
  */
static RfSerialStatus_t trxReadWriteBurstSingle(uint8 addr, uint8* pData, uint16 len);


/******************************************************************************
 * FUNCTIONS
 */


 /*******************************************************************************
  * @fn          trx8BitRegAccess
  *
  * @brief       This function performs a read or write from/to a 8bit register
  *              address space. The function handles burst and single read/write
  *              as specfied in addrByte. Function assumes that chip is ready.
  *
  * input parameters
  *
  * @param       accessType - Specifies if this is a read or write and if it's
  *                           a single or burst access. Bitmask made up of
  *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
  *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
  * @param       addrByte - address byte of register.
  * @param       pData    - data array
  * @param       len      - Length of array to be read(TX)/written(RX)
  *
  * output parameters
  *
  * @return      chip status
  */
RfChipStatus_t trx8BitRegAccess(uint8 accessType, uint8 addrByte, uint8* pData, uint16 len)
{
  RfChipStatus_t chipstatus;
  RfSerialStatus_t serailstatus;
  uint8_t txbuffer = accessType | addrByte;

  /* Pull CS_N low and wait for SO to go low before communication starts */

  cc1120_startTransaction();

  /*TRXEM_SPI_BEGIN();
  while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN); // TO:DO SO pinin low olduğunu nasıl anlayabilirim
  */

  /* send register address byte */
  /*TRXEM_SPI_TX(accessType|addrByte);
  TRXEM_SPI_WAIT_DONE();
  */


  serailstatus = cc1120SpiTRx(&txbuffer, &chipstatus); //rx will be chip status
  while (hspi1.State != HAL_SPI_STATE_READY);

  /* Storing chip status */
  serailstatus = trxReadWriteBurstSingle(accessType | addrByte, pData, len);
  cc1120_endTransaction();
  /* return the status byte value */
  return(chipstatus);
}

/******************************************************************************
 * @fn          trx16BitRegAccess
 *
 * @brief       This function performs a read or write in the extended adress
 *              space of CC112X.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       extAddr - Extended register space address = 0x2F.
 * @param       regAddr - Register address in the extended address space.
 * @param       *pData  - Pointer to data array for communication
 * @param       len     - Length of bytes to be read/written from/to radio
 *
 * output parameters
 *
 * @return      RfChipStatus_t
 */
RfChipStatus_t trx16BitRegAccess(uint8 accessType, uint8 extAddr, uint8 regAddr, uint8* pData, uint8 len)
{
  RfChipStatus_t chipstatus;
  RfSerialStatus_t serailstatus;
  uint8_t rxbuffer;
  uint8_t txbuffer = accessType | extAddr;

  cc1120_startTransaction();
  //TRXEM_SPI_BEGIN();
  //while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);

  /* send extended address byte with access type bits set */
  serailstatus = cc1120SpiTRx(&txbuffer, &chipstatus); //rxbuffer will be chip status
  // extended command
  while (hspi1.State != HAL_SPI_STATE_READY);
  /* Storing chip status */
  //readValue = TRXEM_SPI_RX();
  //TRXEM_SPI_TX(regAddr);
  //TRXEM_SPI_WAIT_DONE();
  serailstatus = cc1120SpiTRx(&regAddr, &rxbuffer);
  while (hspi1.State != HAL_SPI_STATE_READY);

  /* Communicate len number of bytes */
  serailstatus = trxReadWriteBurstSingle(accessType | extAddr, pData, len);
  //TRXEM_SPI_END();
  cc1120_endTransaction();
  /* return the status byte value */
  return(chipstatus);
}

/*******************************************************************************
 * @fn          trxSpiCmdStrobe
 *
 * @brief       Send command strobe to the radio. Returns status byte read
 *              during transfer of command strobe. Validation of provided
 *              is not done. Function assumes chip is ready.
 *
 * input parameters
 *
 * @param       cmd - command strobe
 *
 * output parameters
 *
 * @return      status byte
 */
RfChipStatus_t trxSpiCmdStrobe(uint8 cmd)
{
  RfChipStatus_t chipstatus;
  RfSerialStatus_t serailstatus;
  uint8_t txbuffer = cmd;

  cc1120_startTransaction();
  while (hspi1.State != HAL_SPI_STATE_READY);
  //TRXEM_SPI_BEGIN();
  //while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);
  serailstatus = cc1120SpiTRx(&txbuffer, &chipstatus);
  while (hspi1.State != HAL_SPI_STATE_READY);
  //TRXEM_SPI_TX(cmd);
  //TRXEM_SPI_WAIT_DONE();
  //rc = TRXEM_SPI_RX();
  //TRXEM_SPI_END();
  cc1120_endTransaction();

  return(chipstatus);
}

/*******************************************************************************
 * @fn          trxReadWriteBurstSingle
 *
 * @brief       When the address byte is sent to the SPI slave, the next byte
 *              communicated is the data to be written or read. The address
 *              byte that holds information about read/write -and single/
 *              burst-access is provided to this function.
 *
 *              Depending on these two bits this function will write len bytes to
 *              the radio in burst mode or read len bytes from the radio in burst
 *              mode if the burst bit is set. If the burst bit is not set, only
 *              one data byte is communicated.
 *
 *              NOTE: This function is used in the following way:
 *
 *              TRXEM_SPI_BEGIN();
 *              while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);
 *              ...[Depending on type of register access]
 *              trxReadWriteBurstSingle(uint8 addr,uint8 *pData,uint16 len);
 *              TRXEM_SPI_END();
 *
 * input parameters
 *
 * @param       none
 *
 * output parameters
 *
 * @return      RfSerialStatus_t
 */
static RfSerialStatus_t trxReadWriteBurstSingle(uint8 addr, uint8* pData, uint16 len)
{
  uint16 i;
  uint8_t buffer = 0;


  RfSerialStatus_t serailstatus;
  /* Communicate len number of bytes: if RX - the procedure sends 0x00 to push bytes from slave*/
  if (addr & RADIO_READ_ACCESS)
  {
    if (addr & RADIO_BURST_ACCESS)
    {
      for (i = 0; i < len; i++)
      {
        //TRXEM_SPI_TX(0);            /* Possible to combining read and write as one access type */
        //TRXEM_SPI_WAIT_DONE();
        //*pData = TRXEM_SPI_RX();     /* Store pData from last pData RX */
        serailstatus = cc1120SpiTRx(buffer, pData);
        while (hspi1.State != HAL_SPI_STATE_READY);
        pData++;
      }
    }
    else
    {
      serailstatus = cc1120SpiTRx(buffer, pData);
      while (hspi1.State != HAL_SPI_STATE_READY);
    }
  }
  else
  {
    if (addr & RADIO_BURST_ACCESS)
    {
      /* Communicate len number of bytes: if TX - the procedure doesn't overwrite pData */
      for (i = 0; i < len; i++)
      {
        serailstatus = cc1120SpiTRx(pData, buffer);
        while (hspi1.State != HAL_SPI_STATE_READY);
        pData++;
      }
    }
    else
    {
      serailstatus = cc1120SpiTRx(pData, buffer); // txbuffer'ı öylesine koydum buraya
      while (hspi1.State != HAL_SPI_STATE_READY);
    }
  }
  return serailstatus;
}
