#ifndef __APP_PRINT__
#define __APP_PRINT__

#include <stdarg.h>
#include <stdio.h>

typedef enum
{
  Te_AppLogLevel_FATAL,
  Te_AppLogLevel_ERROR,
  Te_AppLogLevel_WARN,
  Te_AppLogLevel_INFO,
  Te_AppLogLevel_DEBUG,
  
  Te_AppLogLevel_Count
} Te_AppLogLevel;
#define		PRINTF	printf
#define         LOG_PRINTF(log_level, module_name, format, ...)                                                                                      \
        {                                                                                                                               \
              if (log_level == Te_AppLogLevel_FATAL)                                                                                    \
                PRINTF("FATAL_LOG(file: %s; line: %d; function: %s) %s" format, __FILE__, __LINE__, __FUNCTION__, module_name,##__VA_ARGS__);         \
              else if (log_level == Te_AppLogLevel_ERROR)                                                                               \
                PRINTF("ERROR_LOG(file: %s; line: %d; function: %s) %s" format, __FILE__, __LINE__, __FUNCTION__, module_name, ##__VA_ARGS__);         \
              else if (log_level == Te_AppLogLevel_WARN)                                                                                \
                PRINTF("WARN_LOG %s" format, module_name, ##__VA_ARGS__);                                                                              \
              else if (log_level == Te_AppLogLevel_INFO)                                                                                \
                PRINTF("INFO_LOG %s" format, module_name, ##__VA_ARGS__);                                                                              \
        }

#endif