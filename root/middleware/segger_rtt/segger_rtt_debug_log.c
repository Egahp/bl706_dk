/**
  ******************************************************************************
  * @file          segger_rtt_debug_log.c
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


/* include -------------------------------------------------------------------*/
#include "segger_rtt_debug_log.h"
/* marco ---------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

/* declare -------------------------------------------------------------------*/

/* variable ------------------------------------------------------------------*/

/* code ----------------------------------------------------------------------*/
void assert_handler(const char *x_string, const char *file, uint32_t line, int32_t error){
    volatile uint8_t dummy = 0;
    LOG_ERROR("assertion failed\r\n       assert [ %s ]\r\n       error [ %d ]\r\n       line [ %d ]\r\n       file [ %s ]\r\n\r\n",x_string,error,line,file);
    while(dummy == 0);
}

void debug_log_init(void){
    SEGGER_RTT_Init();
}

/************************ (C) COPYRIGHT 2021 Egahp *****END OF FILE*************/
