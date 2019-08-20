#ifndef SSM_H
#define SSM_H
#include "datadomain.h"


int ModuleHeartBeatCheck_SSM(uint8_t module_id);
int ModuleStateCheck_SSM(uint8_t this_module_id);

int StateCheck_SSM(uint8_t moduleId);
int ReportModuleError_SSM(uint8_t moduleId, uint8_t errorId);

int ReportFunctionState_SSM();

#endif