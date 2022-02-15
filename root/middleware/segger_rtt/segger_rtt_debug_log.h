/**
  ******************************************************************************
  * @file          segger_rtt_debug_log.h
  * @brief         
  * @author        Egahp
  *                2687434412@qq.com
  * @version       1.0
  * @date          2022.02.15
  ******************************************************************************
  * @attention
  * 
  * <h2><center>&copy; Copyright 2021 Egahp.
  * All rights reserved.</center></h2>
  * 
  * @htmlonly 
  * <span style='font-weight: bold'>History</span> 
  * @endhtmlonly
  * 版本|作者|时间|描述
  * ----|----|----|----
  * 1.0|Egahp|2022.02.15|创建文件
  ******************************************************************************
  */

#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif
/* include -------------------------------------------------------------------*/
#include "misc.h"
#include "SEGGER_RTT.h"

    void debug_log_init(void);

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

#define __DEBUG_ENABLE

#ifdef __DEBUG_ENABLE

    void assert_handler(const char *x_string, const char *file, uint32_t line, int32_t error);

#define ASSERT_PARAM(x)                                  \
    if (x)                                               \
    {                                                    \
        assert_handler(#x, __FILENAME__, __LINE__, (x)); \
    }

#define ASSERT_FUCN(x)                                           \
    {                                                            \
        int32_t _ret_ = (x);                                     \
        if (_ret_)                                               \
            assert_handler(#x, __FILENAME__, __LINE__, (_ret_)); \
    }
#else
#define ASSERT_PARAM(x)         ((void)x)
#define ASSERT_FUNC(x)          ((void)x)
#endif

#ifndef LOG_ERROR_CHANNEL
#define LOG_ERROR_CHANNEL       0
#endif

#ifndef LOG_WARN_CHANNEL
#define LOG_WARN_CHANNEL        0
#endif

#ifndef LOG_INFO_CHANNEL
#define LOG_INFO_CHANNEL        0
#endif

#ifndef LOG_DEBUG_CHANNEL
#define LOG_DEBUG_CHANNEL       0
#endif

#ifndef LOG_ERROR_TERMINAL
#define LOG_ERROR_TERMINAL      0
#endif

#ifndef LOG_WARN_TERMINAL
#define LOG_WARN_TERMINAL       0
#endif

#ifndef LOG_INFO_TERMINAL
#define LOG_INFO_TERMINAL       0
#endif

#ifndef LOG_DEBUG_TERMINAL
#define LOG_DEBUG_TERMINAL      0
#endif

#ifndef LOG_ERROR_COLOR
#define LOG_ERROR_COLOR RTT_CTRL_TEXT_BRIGHT_RED
#endif

#ifndef LOG_WARN_COLOR
#define LOG_WARN_COLOR RTT_CTRL_TEXT_BRIGHT_YELLOW
#endif

#ifndef LOG_INFO_COLOR
#define LOG_INFO_COLOR RTT_CTRL_TEXT_BRIGHT_GREEN
#endif

#ifndef LOG_DEBUG_COLOR
#define LOG_DEBUG_COLOR RTT_CTRL_TEXT_BRIGHT_MAGENTA
#endif

#ifdef __DEBUG_ENABLE
#define LOG_ERROR(format, ...)                                         \
    SEGGER_RTT_SetTerminal(LOG_ERROR_TERMINAL);                        \
    SEGGER_RTT_printf(LOG_ERROR_CHANNEL, "%s[E]>> ", LOG_ERROR_COLOR); \
    SEGGER_RTT_printf(LOG_ERROR_CHANNEL, format, ##__VA_ARGS__);

#define LOG_WARN(format, ...)                                        \
    SEGGER_RTT_SetTerminal(LOG_WARN_TERMINAL);                       \
    SEGGER_RTT_printf(LOG_WARN_CHANNEL, "%s[W]>> ", LOG_WARN_COLOR); \
    SEGGER_RTT_printf(LOG_WARN_CHANNEL, format, ##__VA_ARGS__)

#define LOG_INFO(format, ...)                                        \
    SEGGER_RTT_SetTerminal(LOG_INFO_TERMINAL);                       \
    SEGGER_RTT_printf(LOG_INFO_CHANNEL, "%s[I]>> ", LOG_INFO_COLOR); \
    SEGGER_RTT_printf(LOG_INFO_CHANNEL, format, ##__VA_ARGS__)

#define LOG_DEBUG(format, ...)                                         \
    SEGGER_RTT_SetTerminal(LOG_DEBUG_TERMINAL);                        \
    SEGGER_RTT_printf(LOG_DEBUG_CHANNEL, "%s[D]>> ", LOG_DEBUG_COLOR); \
    SEGGER_RTT_printf(LOG_DEBUG_CHANNEL, format, ##__VA_ARGS__)

#else
#define LOG_ERROR(format, ...) ((void)0)
#define LOG_WARN(format, ...) ((void)0)
#define LOG_INFO(format, ...) ((void)0)
#define LOG_DEBUG(format, ...) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif
/************************ (C) COPYRIGHT 2021 Egahp *****END OF FILE*************/
