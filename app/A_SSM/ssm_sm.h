#ifndef _SSM_SM_H_
#define _SSM_SM_H_

#include "ssm_type.h"

void InitModuleStateType();
int ModuleStateCheck_SSM(uint8_t this_module_id, BOOL self_check);
Te_ModuleMainState_u8 GetMainState(Te_MoudleId_u8 module_id);
Te_ModuleSubState_u8 GetSubState(Te_MoudleId_u8 module_id);
void SetMainState(Te_MoudleId_u8 module_id, Te_ModuleMainState_u8 main_state, BOOL force);
void SetSubState(Te_MoudleId_u8 module_id, Te_ModuleSubState_u8 sub_state);
#endif