#include "datadomain.h"
#include "ssm_type.h"

/*********************************************/
uint8_t GetModuleDependLevel(uint8_t module_id)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  uint8_t level = g_SsmInfo.module_info[module_index].moduledepend_manager.module_depend_level;
  
  return level;
}
void SetModuleDependLevel(uint8_t module_id, uint8_t level)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  
  g_SsmInfo.module_info[module_index].moduledepend_manager.module_depend_level = level;
}
uint8_t GetDependModuleCount(uint8_t module_id)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  uint8_t depend_module_count = g_SsmInfo.module_info[module_index].moduledepend_manager.depend_module_count;
  
  return depend_module_count;
}
uint8_t GetDependModuleId(uint8_t module_id, uint8_t depend_module_index/*index of depend module list*/)
{
  uint8_t module_index = g_SsmInfo.ModuleIndex[module_id];
  uint8_t depend_module_id = g_SsmInfo.module_info[module_index].moduledepend_manager.plist_dependModule[depend_module_index].depend_module_id;
  
  return depend_module_id;
}
void SetModuleId_DpendLevelRaise(uint8_t list_index, uint8_t module_id)
{
  g_SsmInfo.ModuleId_DpendLevelRaise[list_index] = module_id;
}
ModuleMainState_T  GetModuleStateDepend(uint8_t this_module_id, 
                                        uint8_t depend_module_id, 
                                        ModuleMainState_T this_state)
{
  uint8_t module_index;
  ModuleMainState_T depend_state;
  
  if (this_module_id == depend_module_id)
    return StateNoDepend;
  
  module_index = g_SsmInfo.ModuleIndex[this_module_id];
  depend_state = g_SsmInfo.module_info[module_index].moduledepend_manager.pplist_ModuleStateDepend[this_state][depend_module_id];
    
  return depend_state;
}

void SetModuleStateDepend(uint8_t this_module_id, 
                          uint8_t depend_module_id, 
                          ModuleMainState_T this_state,
                          ModuleMainState_T depend_state)
{
  uint8_t                       this_module_index;
  uint8_t                       module_id;
  ModuleMainState_T             redepend_state;                                 /*recursion only once*/
  
  if (this_module_id == depend_module_id || this_state == StateNoDepend)
    return;
  
  this_module_index = g_SsmInfo.ModuleIndex[this_module_id];
  g_SsmInfo.module_info[this_module_index].moduledepend_manager.pplist_ModuleStateDepend[this_state][depend_module_id] = depend_state;
  
  if (depend_state == StateNoDepend)
    return ;
  
  for (module_id = 0; module_id < Te_ModuleId_Count; module_id++)
  {
    redepend_state = GetModuleStateDepend(depend_module_id, 
                                          module_id, 
                                          depend_state);
    if (redepend_state != StateNoDepend)
    {
      /*recursion only once and relation not raise up, so init shall be call from bottom*/
      g_SsmInfo.module_info[this_module_index].moduledepend_manager.pplist_ModuleStateDepend[this_state][module_id] = redepend_state;
    }
  }
}
void ClearAllModuleDependLevel()
{
  uint8_t       module_id = 0;
  
  /*clear all module depend level*/  
  for (module_id = 0; module_id < Te_ModuleId_Count; module_id++)
  {
    if (GetDependModuleCount(module_id) == 0)
    {
      /*set module depend level 1, if it have no depend module*/
      SetModuleDependLevel(module_id, 1);
    }
    else
    {
      SetModuleDependLevel(module_id, 0);
    }
  }
}
void InitModuleLevel()
{
  BOOL_t                go_continue = TRUE;
  uint8_t               module_id = 0;
  uint8_t               depend_module_count;
  uint8_t               list_index = 0;
  uint8_t               depend_module_id;
  uint8_t               depend_level = 0;
  
  /*a module depend level will be N+1, if max level of its depend modules is N*/
  ClearAllModuleDependLevel();
  
  /*if find out module level M not bigger than its depend module level N, set its level M = N + 1 *
   *behavior above will continuous cycle, until find out no bigger thing occur*/
  while (TRUE == go_continue)
  {
    go_continue = FALSE;
    for (module_id = 0; module_id < Te_ModuleId_Count; module_id++)
    {
      depend_module_count = GetDependModuleCount(module_id);      
      for (list_index = 0; list_index < depend_module_count; list_index++)
      {
        depend_module_id = GetDependModuleId(module_id, list_index);
        depend_level = GetModuleDependLevel(depend_module_id);
        if (depend_level >= GetModuleDependLevel(module_id))
        {
          SetModuleDependLevel(module_id, depend_level+1);          
          go_continue = TRUE;
        }
      }
    }
  }
  /* reorder module by depend level *
   * find all module at the level, from 1 add 1, record module id bu depend level raise */
  go_continue = TRUE;
  depend_level = 0;
  while (TRUE == go_continue)
  {
    go_continue = FALSE;
    depend_level += 1;
    list_index = 0;
    for (module_id = 0; module_id < Te_ModuleId_Count; module_id++)
    {
      if (GetModuleDependLevel(module_id) == depend_level)
      {
        SetModuleId_DpendLevelRaise(list_index, module_id)
        list_index++;
        go_continue = TRUE;
      }
    }
  }
}
/*********************************************/
void ClearStateDepend(uint8_t this_module_id)
{
  uint8_t       depend_module_id = 0;
  uint8_t       this_state_id = 0;
  uint8_t       state_count = 0;
  
  state_count = GetModuleStateCount(this_module_id);
    
  for (depend_module_id = 0; depend_module_id < Te_ModuleId_Count; depend_module_id++)
  {
    for (this_state_id = 0; this_state_id < state_count; this_state_id++)
    {
      SetModuleStateDepend(this_module_id, depend_module_id, this_state_id, StateNoDepend);
    }
  }
}

void InitModuleIndex()
{
  uint8_t module_index = 0;
  uint8_t module_id = 0;

  for (module_index = 0; module_index < Te_ModuleId_Count; module_index++)
  {
    module_id = g_SsmInfo.module_info[module_index].module_id;
    g_SsmInfo.ModuleIndex[module_id] = module_index;
  }
}
void InitModuleDepend()
{
  uint8_t                       module_index = 0;
  uint8_t                       list_index = 0;
  uint8_t                       depend_module_count = 0;
  uint8_t                       this_module_id;
  struct Ts_DependModuleConfig* plist_dependModule;
  uint8_t                       depend_module_id;
  struct Ts_StateDependPair*    plist_pair;
  uint8_t                       pair_count;
  uint8_t                       pair_index;
  uint8_t                       self_state;
  uint8_t                       depend_state;
  
  InitModuleIndex();
  InitModuleLevel();
  
  //buff_count = sizeof(ModuleId_DpendLevelRaise)/sizeof(uint8_t);
  for (buff_index = 0; buff_index < Te_ModuleId_Count; buff_index++)
  {
    //init order through depend level raise
    this_module_id = g_SsmInfo.ModuleId_DpendLevelRaise[buff_index];
    module_index = g_SsmInfo.ModuleIndex[this_module_id];
    depend_module_count = GetDependModuleCount(this_module_id);
    /*if module depend other modules *
     *first clear state depend*/
    if (depend_module_count == 0)
      continue;
    else
      ClearStateDepend(this_module_id);
    
    plist_dependModule = g_SsmInfo.module_info[module_index].moduledepend_manager.plist_dependModule;
    list_index = 0;
    while (plist_dependModule[list_index].depend_module_id != Te_ModuleId_End)
    {
      depend_module_id = plist_dependModule[list_index].depend_module_id;
      plist_pair = plist_dependModule[list_index].plist_pair;
      pair_count = plist_dependModule[list_index].pair_count;
      
      pair_index = 0;
      while (plist_pair[pair_index].module_self_state != Te_MainState_All_End)
      {
        self_state = plist_pair[pair_index].module_self_state;
        depend_state = plist_pair[pair_index].depend_state;
        
        SetModuleDepend( this_module_id, depend_module_id, self_state, depend_state);
        pair_index++;
      }
      list_index++;
    }
  }
}