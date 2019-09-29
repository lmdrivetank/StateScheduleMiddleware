#ifndef _C_BASE_H_
#define _C_BASE_H_
#include "app_print.h"

#define C_LOG(log_level, format, ...)                                   \
{                                                                               \
  LOG_PRINTF(log_level, "employee c action: ", format, ##__VA_ARGS__);            \
}

#endif