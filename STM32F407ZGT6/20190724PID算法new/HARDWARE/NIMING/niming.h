#ifndef __NIMING_H
#define __NIMING_H

#include "sys.h"

void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_niming_pid_report(u8 fun,u8*data,u8 len);
void pid_send_data(short rol_p,short rol_i,short rol_d,short pit_p,short pit_i,short pit_d);
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
void PID_send_data(float pitch,u8 zhen);
#endif
