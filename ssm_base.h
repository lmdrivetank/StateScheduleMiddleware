#ifndef SSM_BASE_H
#define SSM_BASE_H

#include "datadomain.h"

void Get_UserFunctionInput(uint8_t input);

void Get_ACCStatus(uint8_t status);
void Get_SpeedManagerStatus(uint8_t status);
void Get_Adv2Status(uint8_t status);
void Get_CruiseManager_Status(uint32_t status);
void Get_CruiseManager_HeartCnt(uint8_t cnt);
void Set_CruiseManager_State(uint8_t state);

void SetModuleState(uint8_t module_id, uint8_t main_state, uint8_t sub_state);
void GetModuleState(uint8_t module_id, uint8_t *pmain_state, uint8_t *psub_state);

#endif