#include "ssm_sm.h"
#include "ssm_config.h"
#include "ssm_base.h"
#include "string.h"
#include "app_print.h"
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
static void SetModuleState(uint8_t module_id, Ts_ModuleState state)
{
  Te_MoudleId_u8        module_index    = g_SsmInfo.ModuleIndex[module_id];
  Ts_StateManager*      pstate_manager  = &(g_SsmInfo.module_info[module_index].state_manager);

  pstate_manager->main_state = state.main_state;
  pstate_manager->sub_state = state.sub_state;
#if 0
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
#endif
}
Ts_ModuleState GetModuleState(uint8_t module_id)
{
  Te_MoudleId_u8        module_index    = g_SsmInfo.ModuleIndex[module_id];
  Ts_StateManager*      pstate_manager  = &(g_SsmInfo.module_info[module_index].state_manager);
  Ts_ModuleState state;
  
  state.main_state = pstate_manager->main_state;
  state.sub_state = pstate_manager->sub_state;
  
  return state;
#if 0
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
#endif
}
Te_ModuleMainState_u8 GetMainState(Te_MoudleId_u8 module_id)
{
  Ts_ModuleState state = GetModuleState(module_id);
  
  return state.main_state;
}
Te_ModuleSubState_u8 GetSubState(Te_MoudleId_u8 module_id)
{
  Ts_ModuleState state = GetModuleState(module_id);
  
  return state.sub_state;
}
void SetMainState(Te_MoudleId_u8 module_id, Te_ModuleMainState_u8 main_state, BOOL force)
{
  Ts_ModuleState        state           = GetModuleState(module_id);
  Te_MainStateType_u8   mainStateType   = GetMainStateType(module_id, main_state);
  char					module_name[15] = "";


	  char* name = GetModuleName(module_id);
	  uint8_t len = strlen(name);
	  memcpy(module_name, name, len);
	  module_name[len] = '\0';

	  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "-------切换工作状态-------\n");
  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "切换工作状态至 %s\n", GetStateName(module_id, main_state));
  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "当前主状态 %s\n", GetStateName(module_id, state.main_state));
  if (main_state == state.main_state)
	  return; 
  
  //LOG_PRINTF(Te_AppLogLevel_INFO, "s_state %d\n", state.sub_state);
  if(0 != CheckRightful_StateChange(module_id, main_state))
    return;
  
  /**maybe state transfer path should support a context**/
  if (state.sub_state == Te_SubState_StopSucceed || force == TRUE)
  {
	LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "当前子状态StopSucceed或者被强制，可切换主状态\n");
    state.main_state = main_state;
    
    if ( ((mainStateType == Te_MainStateType_Idle || mainStateType == Te_MainStateType_RunOnce) 
          && GetDependModuleCount(module_id) == 0) 
      || mainStateType == Te_MainStateType_RunCycle )
    {      
      state.sub_state = Te_SubState_Ready;
	  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "子状态切换至Ready\n");
    }
    else if (mainStateType == Te_MainStateType_RunOnce && GetDependModuleCount(module_id) > 0)
    {
      state.sub_state = Te_SubState_Prepare;   
	  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "子状态切换至Prepare\n");
    }
    else if (mainStateType == Te_MainStateType_Idle)
    {
      state.sub_state = Te_SubState_Prepare;
	  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "子状态切换至Prepare\n");
    }    
    SetModuleState(module_id, state);
  }
  else if (state.sub_state != Te_SubState_StopFailed && state.sub_state != Te_SubState_HeartBeatError)
  {
    //no error ,but not stop succeed while change state, should request stop by hold
	  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "当前子状态未处于StopSucceed，先请求子状态停止，主状态不切换\n");
    state.sub_state = Te_SubState_Hold;
    SetModuleState(module_id, state);
  }
  //SSM_LOG(Te_AppLogLevel_INFO, "module id %d result\n", module_id);
  //SSM_LOG(Te_AppLogLevel_INFO, "m_state %d\n", state.main_state);
  //SSM_LOG(Te_AppLogLevel_INFO, "s_state %d\n", state.sub_state);
}
void SetSubState(Te_MoudleId_u8 module_id, Te_ModuleSubState_u8 sub_state)
{
  Ts_ModuleState state = GetModuleState(module_id);
  
  state.sub_state = sub_state;
  
  SetModuleState(module_id, state);
}
/*********************************************/
/*********************************************/
int ModuleStateCheck_SSM(uint8_t this_module_id, BOOL self_check)
{
  Ts_ModuleState                self_state;  
  Te_MainStateType_u8           mainStateType;
  uint8_t                       depend_cur_mainstate;
  uint8_t                       depend_dst_mainState;
  uint8_t                       depend_module_id;
  uint8_t                       errorId = 0;
  BOOL                          depend_ok = TRUE;
  char							module_name[15] = "module ssm: ";

  if (self_check == TRUE)
  {
	  char* name = GetModuleName(this_module_id);
	  uint8_t len = strlen(name);
	memcpy(module_name, name, len);
	module_name[len] = '\0';
  }
  else
  {
	  memcpy(module_name, "module ssm: ", sizeof("module ssm: "));
  }
  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "依赖分析检查\n");
  self_state = GetModuleState(this_module_id);
  
  if (self_state.sub_state == Te_SubState_HeartBeatError)
  {
    ReportModuleError_SSM(this_module_id, errorId); 
	LOG_PRINTF(Te_AppLogLevel_ERROR, module_name, "heart beat error\n");
    return -1;
  }
  
  if (self_state.sub_state == Te_SubState_StopFailed)
  {
    ReportModuleError_SSM(this_module_id, errorId);
	LOG_PRINTF(Te_AppLogLevel_ERROR, module_name, "module work stop failed\n");
    SetSubState(this_module_id, Te_SubState_Ready);  
    //tb add fail count
    return 0;
  }
  
  mainStateType = GetMainStateType(this_module_id, self_state.main_state);
  LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "当前状态%s\n", GetStateName(this_module_id, self_state.main_state));
  for (depend_module_id = 0; depend_module_id < Te_ModuleId_Count; depend_module_id++)
  {
    depend_dst_mainState = GetModuleStateDepend(this_module_id, depend_module_id, self_state.main_state);
    if (depend_dst_mainState == StateNoDepend)
      continue;
    depend_cur_mainstate = GetMainState(depend_module_id);
	LOG_PRINTF(Te_AppLogLevel_INFO, module_name, "依赖 %s 工作在状态 %s，当前状态 %s\n", GetModuleName(depend_module_id), GetStateName(depend_module_id, depend_dst_mainState), GetStateName(depend_module_id, depend_cur_mainstate));
    if (depend_dst_mainState != depend_cur_mainstate)
    {      
      depend_ok = FALSE;
      if (mainStateType == Te_MainStateType_RunOnce || 
          (mainStateType == Te_MainStateType_RunCycle && self_state.sub_state == Te_SubState_Ready))
      {
        //self work is once, depend module main state not match, so self substate is prepare, wait depend module run once succeed
        //SSM_LOG(Te_AppLogLevel_INFO, "self state not steady, depend module should change state\n");
        SetMainState(depend_module_id, depend_dst_mainState, FALSE);
      }
      else if ((mainStateType == Te_MainStateType_RunCycle && self_state.sub_state == Te_SubState_Run) 
               || mainStateType == Te_MainStateType_Idle)
      { 
        ReportModuleError_SSM(this_module_id, errorId);
		LOG_PRINTF(Te_AppLogLevel_ERROR, module_name, "self state is steady, depend module may be fail in work\n");
        SetSubState(this_module_id, Te_SubState_Hold);
        //ReportModuleError_SSM(uint8_t moduleId, uint8_t errorId);
        //add module to graceful degradation
        break;
      }
    }
    /*if state current match dst while cycle run, should do nothing*/
    else if (mainStateType == Te_MainStateType_RunOnce && GetSubState(depend_module_id) != Te_SubState_StopSucceed)
    {
      depend_ok = FALSE;
    }
  }
  if (depend_ok == TRUE && mainStateType == Te_MainStateType_RunOnce && self_state.sub_state == Te_SubState_Prepare)
  {
    // report run once complete ,ready to run next state
    SetSubState(this_module_id, Te_SubState_Ready);
  }
  
  return 0;
}