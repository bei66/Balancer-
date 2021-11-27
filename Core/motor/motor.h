//
// Created by bei on 2021/11/15.
//

#ifndef BALANCER_MOTOR_H
#define BALANCER_MOTOR_H


#include "main.h"
#include "tim.h"
#include "my_algorithm.h"
#include "stdio.h"
void Motor_init();
void Motor_zero();
void Motor_run(int8_t motor,int16_t Long);
#endif //BALANCER_MOTOR_H

