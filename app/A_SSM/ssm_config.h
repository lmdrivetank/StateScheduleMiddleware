#ifndef SSM_CONFIG_H
#define SSM_CONFIG_H

#include "ssm_type.h"
#define __DEBUG_MODE__   1
#define         UpperLimit_HeartBeatStopTime            128
#define         ModuleNoDepend                          0xff
#define         StateNoDepend                           0xff
#define         Te_MainState_All_Idle                   0
#define         Te_MainState_All_End                    0xff
#define         Te_MainStateType_End                    0xff
#define         Te_ModuleId_End                         0xff
#define         ModuleDependInitLevel                   0

extern Ts_SsmInfo g_SsmInfo;

char* GetModuleName(uint8_t module_id);
char* GetStateName(uint8_t module_id, uint8_t state_id);

#endif