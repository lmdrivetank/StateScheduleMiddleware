#include "ssm_type.h"
#include "ssm_config.h"
#include "ssm_rte.h"
/******************************************************/
const Ts_MainStateConfig g_MainStateConfig_HmiAdapte[] = 
{
  {Te_MainState_HmiAdapte_Idle,         Te_MainStateType_Idle},
  {Te_MainState_HmiAdapte_Init,         Te_MainStateType_RunOnce},
  {Te_MainState_HmiAdapte_PreSimple,    Te_MainStateType_RunOnce},
  {Te_MainState_HmiAdapte_RunSimple,    Te_MainStateType_RunCycle},
  {Te_MainState_HmiAdapte_PrePCC,       Te_MainStateType_RunOnce},
  {Te_MainState_HmiAdapte_RunPCC,       Te_MainStateType_RunCycle},
  {Te_MainState_HmiAdapte_Terminate,    Te_MainStateType_RunOnce},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
const Ts_MainStateConfig g_MainStateConfig_CM[] = 
{
  {Te_MainState_CM_Idle,                Te_MainStateType_Idle},
  {Te_MainState_CM_Init,                Te_MainStateType_RunOnce},
  {Te_MainState_CM_PreSimple,           Te_MainStateType_RunOnce},
  {Te_MainState_CM_RunSimple,           Te_MainStateType_RunCycle},
  {Te_MainState_CM_PrePcc,              Te_MainStateType_RunOnce},
  {Te_MainState_CM_RunPcc,              Te_MainStateType_RunCycle},
  {Te_MainState_CM_Terminate,           Te_MainStateType_RunOnce},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
const Ts_MainStateConfig g_MainStateConfig_ADV2HR[] = 
{
  {Te_MainState_ADV2HR_Idle,            Te_MainStateType_Idle},
  {Te_MainState_ADV2HR_Init,            Te_MainStateType_RunOnce},
  {Te_MainState_ADV2HR_Run,             Te_MainStateType_RunCycle},
  {Te_MainState_ADV2HR_Terminate,       Te_MainStateType_RunOnce},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
/******************************************************/
const Ts_StateDependPair HmiAdapteDependCruiseManager[] = 
{
  {Te_MainState_HmiAdapte_Idle,         Te_MainState_CM_Idle},
  {Te_MainState_HmiAdapte_Init,         Te_MainState_CM_Init},
  {Te_MainState_HmiAdapte_PreSimple,    Te_MainState_CM_PreSimple},
  {Te_MainState_HmiAdapte_RunSimple,    Te_MainState_CM_RunSimple},
  {Te_MainState_HmiAdapte_PrePCC,       Te_MainState_CM_PrePcc},
  {Te_MainState_HmiAdapte_RunPCC,       Te_MainState_CM_RunPcc},
  {Te_MainState_HmiAdapte_Terminate,    Te_MainState_CM_Terminate},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const Ts_StateDependPair HmiAdapteDependADV2HR[] = 
{
  {Te_MainState_HmiAdapte_Idle,         Te_MainState_ADV2HR_Idle},
  {Te_MainState_HmiAdapte_Init,         Te_MainState_ADV2HR_Init},
  {Te_MainState_HmiAdapte_RunPCC,       Te_MainState_ADV2HR_Run},
  {Te_MainState_HmiAdapte_Terminate,    Te_MainState_ADV2HR_Terminate},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const Ts_DependModuleConfig HmiAdapteDependModuleList[] = 
{
  {
      Te_ModuleId_CruiseManager, 
      HmiAdapteDependCruiseManager, 
      sizeof(HmiAdapteDependCruiseManager)/sizeof(Ts_StateDependPair)
  },
#if 0
  {
      Te_ModuleId_ADV2HR, 
      HmiAdapteDependADV2HR, 
      sizeof(HmiAdapteDependADV2HR)/sizeof(Ts_StateDependPair)
  },  
#endif
  {     
      Te_ModuleId_End, 
      NULL, 
      0
  }
};
/******************************************************/
/******************************************************/
Ts_SsmInfo g_SsmInfo = 
{
  {//struct Ts_ModuleInfo list
    {//struct Ts_ModuleInfo
        Te_ModuleId_HmiAdapte, 
        {//struct Ts_StateManager
          Te_MainState_All_Idle,
          Te_SubState_StopSucceed,
          Te_MainState_HmiAdapte_Count,
          g_MainStateConfig_HmiAdapte,
          g_MainStateType_HmiAdapte
        },
        {//struct Ts_ModuleDependManager
          sizeof(HmiAdapteDependModuleList)/sizeof(Ts_DependModuleConfig),
          HmiAdapteDependModuleList,     
          ModuleDependInitLevel, 
          ModuleDepend_HmiAdapte
        },
        {//struct Ts_HeartBeatManager
          Te_HeartBeatState_Fine,
          0
        }
    },
    {
        Te_ModuleId_CruiseManager,
        {
          Te_MainState_All_Idle,
          Te_SubState_StopSucceed,
          Te_MainState_CM_Count,
          g_MainStateConfig_CM,
          g_MainStateType_CruiseManager
        },
        {
          0,
          NULL,
          ModuleDependInitLevel,
          NULL
        },
        {
          Te_HeartBeatState_Fine,
          0
        }
    },
    {
        Te_ModuleId_ADV2HR, 
        {
          Te_MainState_All_Idle,
          Te_SubState_StopSucceed,
          Te_MainState_ADV2HR_Count,
          g_MainStateConfig_ADV2HR,
          g_MainStateType_ADV2HR
        },
        {
          0,
          NULL,    
          ModuleDependInitLevel,           
          NULL
        },
        {
          Te_HeartBeatState_Fine,
          0
        }
    }
  }//struct Ts_ModuleInfo list end
};