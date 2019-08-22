#include "ssm_sm.h"
#include "ssm_config.h"

uint8_t GetModuleStateCount(uint8_t module_id)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  uint8_t state_count = g_SsmInfo.module_info[module_index].state_manager.state_count;
  
  return state_count;
}
void InitModuleStateType()
{
  uint8_t                       module_index = 0;
  uint8_t                       list_index = 0;
  Ts_MainStateConfig const*     plist_stateConfig;
  Te_MainStateType_u8*          plist_MainStateType;
  uint8_t                       main_state_id;
  
  for (module_index = 0; module_index < Te_ModuleId_Count; module_index++)
  {
    plist_stateConfig = g_SsmInfo.module_info[module_index].state_manager.plist_stateConfig;
    plist_MainStateType = g_SsmInfo.module_info[module_index].state_manager.plist_MainStateType;
    
    list_index = 0;
    while (plist_stateConfig[list_index].MainState_id != Te_MainState_All_End)
    {
      main_state_id = plist_stateConfig[list_index].MainState_id;
      plist_MainStateType[main_state_id] = plist_stateConfig[list_index].state_type;
      
      list_index++;
    }
  }
}
Te_MainStateType_u8 GetMainStateType(uint8_t module_id, uint8_t main_state)
{
  uint8_t                       module_index = 0;
  Te_MainStateType_u8*          plist_MainStateType;
  module_index = g_SsmInfo.ModuleIndex[module_id];
  plist_MainStateType = g_SsmInfo.module_info[module_index].state_manager.plist_MainStateType;
    
  return plist_MainStateType[main_state];
}
int CheckRightful_StateChange(uint8_t module_id, uint8_t dst_main_state)
{
  uint8_t cur_main_state;
  
  /*check state transition rightful, from cur_main_state to dst_main_state*/
  
  return 0;
}
static void SetModuleState(uint8_t module_id, struct Ts_ModuleState state)
{
  //private in this file
  switch (module_id)
  {
  case Te_ModuleId_System:
    break;
  case Te_ModuleId_CruiseManager:
    break;
  case Te_ModuleId_HmiAdapte:
    break;
  case Te_ModuleId_ADV2HR:
    break;
  default:
    break;
  }
}
struct Ts_ModuleState GetModuleState(uint8_t module_id)
{
  struct Ts_ModuleState state;
  switch (module_id)
  {
  case Te_ModuleId_System:
    break;
  case Te_ModuleId_CruiseManager:
    break;
  case Te_ModuleId_HmiAdapte:
    break;
  case Te_ModuleId_ADV2HR:
    break;
  default:
    break;
  }
  return state;
}
uint8_t GetMainState(uint8_t module_id)
{
  struct Ts_ModuleState state = GetModuleState(module_id);
  
  return state.MainState;
}
uint8_t GetSubState(uint8_t module_id)
{
  struct Ts_ModuleState state = GetModuleState(module_id);
  
  return state.SubState;
}
int SetMainState(uint8_t module_id, uint8_t main_state)
{
  struct Ts_ModuleState         state           = GetModuleState(module_id);
  Te_MainStateType_u8           mainStateType   = GetMainStateType(module_id, main_state);

  if (main_state == state.MainState)
    return 0;
  
  if(0 != CheckRightful_StateChange(module_id, main_state))
    return -1;
  
  /**maybe state transfer path should support a context**/
  if (state.SubState == Te_SubState_StopSucceed)
  {
    state.MainState = main_state;
    
    if ( (mainStateType == Te_MainStateType_RunOnce && GetDependModuleCount(module_id) == 0) 
      || mainStateType == Te_MainStateType_RunCycle )
    {      
      state.SubState = Te_SubState_Ready;
    }
    else if (mainStateType == Te_MainStateType_RunOnce && GetDependModuleCount(module_id) > 0)
    {
      state.SubState = Te_SubState_Prepare;      
    }
    else if (mainStateType == Te_MainStateType_Idle)
    {
      state.SubState = Te_SubState_StopSucceed;
    }
    
    SetModuleState(module_id, state);
  }
  else if (state.SubState != Te_SubState_StopFailed && state.SubState != Te_SubState_HeartBeatError)
  {
    //no error ,but not stop succeed while change state, should request stop by hold
    state.SubState = Te_SubState_Hold;
    SetModuleState(module_id, state);
  }
  
  return 0;
}
int SetSubState(uint8_t module_id, uint8_t sub_state)
{
  struct Ts_ModuleState state = GetModuleState(module_id);
  
  state.SubState = sub_state;
  
  SetModuleState(module_id, state);
}
/*********************************************/
/*********************************************/
int ModuleStateCheck_SSM(uint8_t this_module_id)
{
  struct Ts_ModuleState         self_state;  
  Te_MainStateType_u8           mainStateType;
  uint8_t                       depend_cur_mainstate;
  uint8_t                       depend_dst_mainState;
  uint8_t                       depend_module_id;
  uint8_t                       errorId = 0;
  BOOL                        depend_ok = TRUE;
  
  self_state = GetModuleState(this_module_id);
  
  if (self_state.SubState == Te_SubState_HeartBeatError)
  {
    ReportModuleError_SSM(this_module_id, errorId); 
    return -1;
  }
  
  if (self_state.SubState == Te_SubState_StopFailed)
  {
    ReportModuleError_SSM(this_module_id, errorId);
    SetSubState(this_module_id, Te_SubState_Ready);  
    //tb add fail count
    return 0;
  }
  
  mainStateType = GetMainStateType(this_module_id, self_state.MainState);
  for (depend_module_id = 0; depend_module_id < Te_ModuleId_Count; depend_module_id++)
  {
    depend_dst_mainState = GetModuleDepend(this_module_id, depend_module_id, self_state.MainState);
    if (depend_dst_mainState == StateNoDepend)
      continue;
    depend_cur_mainstate = GetMainState(depend_module_id);
    if (depend_dst_mainState != depend_cur_mainstate)
    {      
      depend_ok = FALSE;
      if (mainStateType == Te_MainStateType_RunOnce)
      {
        //self work is once, depend module main state not match, so self substate is prepare, wait depend module run once succeed
        SetMainState(depend_module_id, depend_dst_mainState);
      }
      else if (mainStateType == Te_MainStateType_RunCycle || mainStateType == Te_MainStateType_Idle)
      { 
        ReportModuleError_SSM(this_module_id, errorId);
        SetSubState(this_module_id, Te_SubState_Hold);
        //ReportModuleError_SSM(uint8_t moduleId, uint8_t errorId);
        //add module to graceful degradation
      }
    }
    /*if state current match dst while cycle run, should do nothing*/
    else if (mainStateType == Te_MainStateType_RunOnce && GetSubState(depend_module_id) != Te_SubState_StopSucceed)
    {
      depend_ok = FALSE;
    }
  }
  if (depend_ok == TRUE && mainStateType == Te_MainStateType_RunOnce)
  {
    // report run once complete ,ready to run next state
    SetSubState(this_module_id, Te_SubState_Ready);
  }
  
  return 0;
}