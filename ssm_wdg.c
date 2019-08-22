#include "datadomain.h"
#include "ssm_type.h"
#include "ssm_config.h"

void AddModuleHeartBeatPauseCount(uint8_t module_id)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  
  g_SsmInfo.module_info[module_index].heartbeat_manager.heartbeat_pauseCount++;
}
void ClearModuleHeartBeatPauseCount(uint8_t module_id)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  
  g_SsmInfo.module_info[module_index].heartbeat_manager.heartbeat_pauseCount = 0;
}
uint16_t GetModuleHeartBeatPauseCount(uint8_t module_id)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  
  return g_SsmInfo.module_info[module_index].heartbeat_manager.heartbeat_pauseCount;
}

void SetHeartBeatState(uint8_t module_id, Te_HeartBeatState_u8 state)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  
  g_SsmInfo.module_info[module_index].heartbeat_manager.state = state;
}
Te_HeartBeatState_u8 GetHeartBeatState(uint8_t module_id)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  
  return g_SsmInfo.module_info[module_index].heartbeat_manager.state;
}
void ModuleHeartBeat(uint8_t module_id)
{
  SetHeartBeatState(module_id, Te_HeartBeatState_Fine);
  ClearModuleHeartBeatPauseCount(module_id);
}

void InitModuleHeartBeatState()
{
  uint8_t module_id = 0;
  
  for (module_id = 0; module_id < Te_ModuleId_Count; module_id++)
  {
    SetHeartBeatState(module_id, Te_HeartBeatState_Pause);
  }
}

int ModuleHeartBeatCheck_SSM(uint8_t module_id)
{
  Te_HeartBeatState_u8 state = GetHeartBeatState(module_id);
  
  if (state == Te_HeartBeatState_Fine)
  {
    SetHeartBeatState(module_id, Te_HeartBeatState_Pause);
  }
  else if (state == Te_HeartBeatState_Pause)
  {
    if (GetModuleHeartBeatPauseCount(module_id) < UpperLimit_HeartBeatStopTime)
    {
      AddModuleHeartBeatPauseCount(module_id);
    }
    else
    {
      SetHeartBeatState(module_id, Te_HeartBeatState_Stop);
      SetSubState(module_id, Te_SubState_HeartBeatError);
      return -1;
    }
  }
  return 0;
}