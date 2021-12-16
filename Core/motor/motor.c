//
// Created by bei on 2021/11/15.
//

#include "motor.h"

#define Val_max 0
#define Val_min 8000
#define Long_min 0
#define Long_max 2150


static int Motor_X = 0, Motor_Y = 0, Motor_Z = 0;
int8_t DIR_X = 1, DIR_Y = 1, DIR_Z = 1;
int Long_X = 0, Long_Y = 0, Long_Z = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim1) {
        if (Long_X == Motor_X) {
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
            HAL_TIM_Base_Stop_IT(&htim1);
        }
        if (DIR_X)
            Motor_X++;
        else
            Motor_X--;
    }
    if (htim == &htim2) {
        if (Long_Y == Motor_Y) {
            HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
            HAL_TIM_Base_Stop_IT(&htim2);
        }
        if (DIR_Y)
            Motor_Y++;
        else
            Motor_Y--;
    }
    if (htim == &htim3) {
        if (Long_Z == Motor_Z) {
            HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
            HAL_TIM_Base_Stop_IT(&htim3);
        }
        if (DIR_Z)
            Motor_Z++;
        else
            Motor_Z--;
    }
}

void Motor_init() {
    const int8_t Val = 0;
    HAL_GPIO_WritePin(GPIOA, EN_ALL_Pin, 1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    TIM1->PSC = map(Val, 0, 100, Val_min, Val_max);       //时钟频率
    TIM2->PSC = map(Val, 0, 100, Val_min, Val_max);
    TIM3->PSC = map(Val, 0, 100, Val_min, Val_max);
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);                    //占空比
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
    HAL_GPIO_WritePin(GPIOA, EN_ALL_Pin, 0);
}

void Motor_zero() {
    const int8_t Val = 5;
    const int8_t DIR = 0;
    int8_t Flag_X, Flag_Y, Flag_Z;
    Flag_X = Flag_Y = Flag_Z = 0;
    TIM1->PSC = map(Val, 0, 100, Val_min, Val_max);       //时钟频率
    TIM2->PSC = map(Val, 0, 100, Val_min, Val_max);
    TIM3->PSC = map(Val, 0, 100, Val_min, Val_max);
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 5);                    //占空比
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 5);
            __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 5);
    HAL_GPIO_WritePin(DIR_X_GPIO_Port, DIR_X_Pin, DIR);          //方向
    HAL_GPIO_WritePin(DIR_Y_GPIO_Port, DIR_Y_Pin, DIR);
    HAL_GPIO_WritePin(DIR_Z_GPIO_Port, DIR_Z_Pin, DIR);
    HAL_GPIO_WritePin(GPIOA, EN_ALL_Pin, 0);             //EN_ON
    while (1) {
        if (HAL_GPIO_ReadPin(X_ZERO_GPIO_Port, X_ZERO_Pin) == 0 && Flag_X == 0) {
            HAL_Delay(5);
            if (HAL_GPIO_ReadPin(X_ZERO_GPIO_Port, X_ZERO_Pin) == 0) {
                        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
                Flag_X = 1;
            }
        }
        if (HAL_GPIO_ReadPin(Y_ZERO_GPIO_Port, Y_ZERO_Pin) == 0 && Flag_Y == 0) {
            HAL_Delay(5);
            if (HAL_GPIO_ReadPin(Y_ZERO_GPIO_Port, Y_ZERO_Pin) == 0) {
                        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
                Flag_Y = 1;
            }
        }
        if (HAL_GPIO_ReadPin(Z_ZERO_GPIO_Port, Z_ZERO_Pin) == 0) {
            HAL_Delay(5);
            if (HAL_GPIO_ReadPin(Z_ZERO_GPIO_Port, Z_ZERO_Pin) == 0 && Flag_Z == 0) {
                        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
                Flag_Z = 1;
            }
        }
        if (Flag_X == 1 && Flag_Y == 1 && Flag_Z == 1)
            break;
    }

    Motor_X = Motor_Y = Motor_Z = 0;
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void Motor_run_Abs(int8_t motor, int16_t Long) {
    HAL_GPIO_WritePin(GPIOA, EN_ALL_Pin, 0);
    static int Long_temp_X = 0, Long_temp_Y = 0, Long_temp_Z = 0;
    static int Val = 10;
    if (Long > 920)
        Long = 920;
    if (Long < 20)
        Long = 20;
    if (motor == 1) {
        Val = map(abs(map(Motor_X, Long_min, Long_max, 0, 1000) - Long), 0, 500, 1, 60);
        Val = Val * Val;
        if (Val > 55)
            Val = 55;
//        printf("Motor_Z  %d  long  %d   Val  %d \r\n",abs(map(Motor_Z,Long_min,Long_max,0,1000)),Long,Val);
    } else if (motor == 2) {
        Val = map(abs(map(Motor_Y, Long_min, Long_max, 0, 1000) - Long), 0, 500, 1, 60);
        Val = Val * Val;
        if (Val > 55)
            Val = 55;
    } else if (motor == 3) {
        Val = map(abs(map(Motor_Z, Long_min, Long_max, 0, 1000) - Long), 0, 500, 1, 60);
        Val = Val * Val;
        if (Val > 55)
            Val = 55;
    }
    if (motor == 1 && Motor_X != Long && Long_temp_X != Long) {
        Long_X = map(Long, 0, 1000, Long_min, Long_max);
        if (Long_X > Motor_X) {
            DIR_X = 1;
        } else {
            DIR_X = 0;
        }
        TIM1->PSC = map(Val, 0, 100, Val_min, Val_max);
                __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 5);
        HAL_GPIO_WritePin(DIR_X_GPIO_Port, DIR_X_Pin, DIR_X);
        HAL_TIM_Base_Start_IT(&htim1);
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        //HAL_TIM_PWM_Start_IT(&htim1,TIM_CHANNEL_1);
    }
    if (motor == 2 && Long_temp_Y != Long) {
        Long_Y = map(Long, 0, 1000, Long_min, Long_max);
        if (Long_Y > Motor_Y) {
            DIR_Y = 1;
        } else {
            DIR_Y = 0;
        }
        TIM2->PSC = map(Val, 0, 100, Val_min, Val_max);
                __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 5);
        HAL_GPIO_WritePin(DIR_Y_GPIO_Port, DIR_Y_Pin, DIR_Y);
        HAL_TIM_Base_Start_IT(&htim2);
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    }
    if (motor == 3 && Long_temp_Z != Long) {
        Long_Z = map(Long, 0, 1000, Long_min, Long_max);
        if (Long_Z > Motor_Z) {
            DIR_Z = 1;

        } else {
            DIR_Z = 0;
        }
        TIM3->PSC = map(Val, 0, 100, Val_min, Val_max);
                __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 5);
        HAL_GPIO_WritePin(DIR_Z_GPIO_Port, DIR_Z_Pin, DIR_Z);
        HAL_TIM_Base_Start_IT(&htim3);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    }
    if (motor == 1)
        Long_temp_X = Long;
    if (motor == 2)
        Long_temp_Y = Long;
    if (motor == 3)
        Long_temp_Z = Long;


}


void Motor_Run(uint8_t motor, int Val) {
    HAL_GPIO_WritePin(GPIOA, EN_ALL_Pin, 0);
    if (Motor_X > (Long_max - 100) && Val > 0) {
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIM_Base_Stop_IT(&htim1);
        return;
    } else if (Motor_Y > (Long_max - 100) && Val > 0) {
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        HAL_TIM_Base_Stop_IT(&htim2);
        return;
    } else if (Motor_Z > (Long_max - 100) && Val > 0) {
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
        HAL_TIM_Base_Stop_IT(&htim3);
        return;
    }
    if (Motor_X < 100 && Val < 0) {
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIM_Base_Stop_IT(&htim1);
        return;
    } else if (Motor_Y < 100 && Val < 0) {
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        HAL_TIM_Base_Stop_IT(&htim2);
        return;
    } else if (Motor_Z < 100 && Val < 0) {
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
        HAL_TIM_Base_Stop_IT(&htim3);
        return;
    }
    if (Val == 0) {
        if (motor == 1) {
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
            HAL_TIM_Base_Stop_IT(&htim1);
            return;
        }
        if (motor == 2) {
            HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
            HAL_TIM_Base_Stop_IT(&htim2);
            return;
        }
        if (motor == 3) {
            HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
            HAL_TIM_Base_Stop_IT(&htim3);
            return;
        }
    }


    if (motor == 1 && Val != 0) {
        if (Val > 0) {
            DIR_X = 1;
        } else {
            DIR_X = 0;
        }
        TIM1->PSC = map(abs(Val), 0, 100, Val_min, Val_max);
                __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 5);
        HAL_GPIO_WritePin(DIR_X_GPIO_Port, DIR_X_Pin, DIR_X);
        HAL_TIM_Base_Start_IT(&htim1);
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        //HAL_TIM_PWM_Start_IT(&htim1,TIM_CHANNEL_1);
    }
    if (motor == 2 && Val != 0) {
        if (Val > 0) {
            DIR_Y = 1;
        } else {
            DIR_Y = 0;
        }
        TIM2->PSC = map(abs(Val), 0, 100, Val_min, Val_max);
                __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 5);
        HAL_GPIO_WritePin(DIR_Y_GPIO_Port, DIR_Y_Pin, DIR_Y);
        HAL_TIM_Base_Start_IT(&htim2);
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    }
    if (motor == 3 && Val != 0) {
        if (Val > 0) {
            DIR_Z = 1;
        } else {
            DIR_Z = 0;
        }
        TIM3->PSC = map(abs(Val), 0, 100, Val_min, Val_max);
                __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 5);
        HAL_GPIO_WritePin(DIR_Z_GPIO_Port, DIR_Z_Pin, DIR_Z);
        HAL_TIM_Base_Start_IT(&htim3);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    }

}