#ifndef _MANAGER_BASE_H_
#define _MANAGER_BASE_H_
#include "app_print.h"

#define Manager_LOG(log_level, format, ...)                                   \
{                                                                               \
  LOG_PRINTF(log_level, "manager call: ", format, ##__VA_ARGS__);            \
}

#endif