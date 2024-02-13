#include "cc1120_intf.h"
#include "cc1120_config.h"

/*******************************************************************************
*   @fn	        registerConfig
*
*   @brief      Reset and configure the CC1120 based on selected freq. band
*
*   @param      freq
*                 FREQ_470 or FREQ_868
*
*   @return     none
*/
void registerConfig(RfConfig config) {
  uint8 writeByte;

  trxSpiCmdStrobe(CC112X_SRES);

  if (config == _4GFSK_200kbps) {
    for(uint16 i = 0;
        i < (sizeof(preferredSettingsMaxDR)/sizeof(registerSetting_t)); i++) {
      writeByte = preferredSettingsMaxDR[i].data;
      cc112xSpiWriteReg(preferredSettingsMaxDR[i].addr, &writeByte, 1);
    }
  } else {
    for(uint16 i = 0;
        i < (sizeof(preferredSettingsSens)/sizeof(registerSetting_t)); i++) {
      writeByte = preferredSettingsSens[i].data;
      cc112xSpiWriteReg(preferredSettingsSens[i].addr, &writeByte, 1);
    }
  }
}
