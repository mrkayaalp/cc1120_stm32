/*
 * cc1120_config.h
 *
 *  Created on: Feb 13, 2024
 *      Author: BurakKAYAALP
 */

#ifndef INC_CC1120_CONFIG_H_
#define INC_CC1120_CONFIG_H_

#include "cc112x_def.h"

// Address Config = No address check
// Bit Rate = 200
// Carrier Frequency = 868.000000
// Deviation = 82.763672
// Device Address = 0
// Manchester Enable = false
// Modulation Format = 4-GFSK
// PA Ramping = true
// Packet Bit Length = 0
// Packet Length = 255
// Packet Length Mode = Variable
// Performance Mode = High Performance
// RX Filter BW = 200.000000
// Symbol rate = 100
// TX Power = 27
// Whitening = false

static const registerSetting_t preferredSettingsMaxDR[]=
{
  {CC1120_IOCFG3,            0xB0},
  {CC1120_IOCFG2,            0x06},
  {CC1120_IOCFG1,            0xB0},
  {CC1120_IOCFG0,            0x40},
  {CC1120_SYNC_CFG1,         0x07},
  {CC1120_DEVIATION_M,       0x53},
  {CC1120_MODCFG_DEV_E,      0x2F},
  {CC1120_DCFILT_CFG,        0x04},
  {CC1120_PREAMBLE_CFG1,     0x18},
  {CC1120_FREQ_IF_CFG,       0x00},
  {CC1120_IQIC,              0x00},
  {CC1120_CHAN_BW,           0x01},
  {CC1120_MDMCFG0,           0x05},
  {CC1120_SYMBOL_RATE2,      0xA9},
  {CC1120_SYMBOL_RATE1,      0x99},
  {CC1120_SYMBOL_RATE0,      0x99},
  {CC1120_AGC_REF,           0x3C},
  {CC1120_AGC_CS_THR,        0xEC},
  {CC1120_AGC_CFG3,          0x83},
  {CC1120_AGC_CFG2,          0x60},
  {CC1120_AGC_CFG1,          0xA9},
  {CC1120_AGC_CFG0,          0xC0},
  {CC1120_FIFO_CFG,          0x00},
  {CC1120_FS_CFG,            0x12},
  {CC1120_PKT_CFG0,          0x20},
  {CC1120_PA_CFG2,           0x77},
  {CC1120_PA_CFG0,           0x01},
  {CC1120_PKT_LEN,           0xFF},
  {CC1120_IF_MIX_CFG,        0x00},
  {CC1120_TOC_CFG,           0x0A},
  {CC1120_FREQ2,             0x6C},
  {CC1120_FREQ1,             0x80},
  {CC1120_FS_DIG1,           0x00},
  {CC1120_FS_DIG0,           0x5F},
  {CC1120_FS_CAL1,           0x40},
  {CC1120_FS_CAL0,           0x0E},
  {CC1120_FS_DIVTWO,         0x03},
  {CC1120_FS_DSM0,           0x33},
  {CC1120_FS_DVC0,           0x17},
  {CC1120_FS_PFD,            0x50},
  {CC1120_FS_PRE,            0x6E},
  {CC1120_FS_REG_DIV_CML,    0x14},
  {CC1120_FS_SPARE,          0xAC},
  {CC1120_FS_VCO0,           0xB4},
  {CC1120_XOSC5,             0x0E},
  {CC1120_XOSC1,             0x03},
};


static const registerSetting_t preferredSettingsSens[]=
{
  {CC1120_IOCFG3,            0xB0},
  {CC1120_IOCFG2,            0x06},
  {CC1120_IOCFG1,            0xB0},
  {CC1120_IOCFG0,            0x40},
  {CC1120_SYNC_CFG1,         0x0B},
  {CC1120_DCFILT_CFG,        0x1C},
  {CC1120_IQIC,              0xC6},
  {CC1120_MDMCFG0,           0x05},
  {CC1120_AGC_REF,           0x20},
  {CC1120_AGC_CS_THR,        0x19},
  {CC1120_AGC_CFG1,          0xA9},
  {CC1120_AGC_CFG0,          0xCF},
  {CC1120_FIFO_CFG,          0x00},
  {CC1120_FS_CFG,            0x12},
  {CC1120_PKT_CFG0,          0x20},
  {CC1120_PA_CFG2,           0x77},
  {CC1120_PKT_LEN,           0xFF},
  {CC1120_IF_MIX_CFG,        0x00},
  {CC1120_FREQOFF_CFG,       0x22},
  {CC1120_FREQ2,             0x6C},
  {CC1120_FREQ1,             0x80},
  {CC1120_FS_DIG1,           0x00},
  {CC1120_FS_DIG0,           0x5F},
  {CC1120_FS_CAL1,           0x40},
  {CC1120_FS_CAL0,           0x0E},
  {CC1120_FS_DIVTWO,         0x03},
  {CC1120_FS_DSM0,           0x33},
  {CC1120_FS_DVC0,           0x17},
  {CC1120_FS_PFD,            0x50},
  {CC1120_FS_PRE,            0x6E},
  {CC1120_FS_REG_DIV_CML,    0x14},
  {CC1120_FS_SPARE,          0xAC},
  {CC1120_FS_VCO0,           0xB4},
  {CC1120_XOSC5,             0x0E},
  {CC1120_XOSC1,             0x03},
};




#endif /* INC_CC1120_CONFIG_H_ */
