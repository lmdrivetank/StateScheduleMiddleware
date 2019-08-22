#ifndef SSM_H
#define SSM_H
#include "datadomain.h"
#include "ssm_type.h"
#include "ssm_config.h"
extern int InitProcess_SSM();
extern int TerminateProcess_SSM();
extern int UserFunctionInputCheck_SSM();
extern int ModuleStateCheckAll_SSM();

int ModuleHeartBeatCheck_SSM(uint8_t module_id);
int ModuleStateCheck_SSM(uint8_t this_module_id);

int StateCheck_SSM(uint8_t moduleId);
int ReportModuleError_SSM(uint8_t moduleId, uint8_t errorId);

int ReportFunctionState_SSM();

#endif