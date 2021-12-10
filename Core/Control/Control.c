//
// Created by bei on 2021/12/3.
//

#include "Control.h"

#define L_roll 14
#define L_pitch 14
#define STEP_mm 0.03349

void Control_motor_Abs(void) {
    static uint8_t times = 0;
    static uint8_t ch;
    int Length_X, Length_Y, Length_Z;
    if (imu901_uart_receive(&ch, 1))    /*!< 获取串口fifo一个字节 */
    {
        if (imu901_unpack(ch))            /*!< 解析出有效数据包 */
        {
            if (rxPacket.startByte2 == UP_BYTE2)            /*!< 主动上传的数据包 */
            {
                AtkpParsing(&rxPacket);
                if (attitude.roll >= 0) {
                    Length_X = 500 + (Sin(attitude.roll) * L_roll / STEP_mm);
                    Length_Y = 500 - (Sin(attitude.roll) * L_roll / STEP_mm);
                } else {
                    Length_X = 500 - (Sin(-attitude.roll) * L_roll / STEP_mm);
                    Length_Y = 500 + (Sin(-attitude.roll) * L_roll / STEP_mm);
                }
                if (attitude.pitch >= 0)
                    Length_Z = 500 - (Sin(attitude.pitch) * L_pitch / STEP_mm);
                else
                    Length_Z = 500 + (Sin(-attitude.pitch) * L_pitch / STEP_mm);

                Motor_run_Abs(1, Length_X);
                Motor_run_Abs(3, Length_Y);
                Motor_run_Abs(2, Length_Z);

            }
        }
    } else {
        HAL_Delay(1);

    }
}

void Control_motor(void) {
    static uint8_t times = 0;
    static uint8_t ch;
    int Val_X, Val_Y, Val_Z;
    int roll_temp = 0,pitch_temp = 0;
    if (imu901_uart_receive(&ch, 1))    /*!< 获取串口fifo一个字节 */
    {
        if (imu901_unpack(ch))            /*!< 解析出有效数据包 */
        {
            if (rxPacket.startByte2 == UP_BYTE2)            /*!< 主动上传的数据包 */
            {
                AtkpParsing(&rxPacket);
                times ++;
                if (times == 5){
                    times = 0;
                    if (attitude.roll >= 0.2) {
                        Val_X = map((int)attitude.roll, 0, 180, 0, 300);
                        Val_X = Val_X*Val_X;
                        if (Val_X > 65)
                            Val_X = 65;
                        Motor_Run(1, Val_X);
                        Motor_Run(3, -Val_X);
                    } else if (attitude.roll <= -0.2){
                        Val_X = map(-(int)attitude.roll, 0, 180, 0, 300);
                        Val_X = Val_X*Val_X;
                        if (Val_X > 65)
                            Val_X = 65;
                        Motor_Run(1, -Val_X);
                        Motor_Run(3, Val_X);
                    }else
                    {
                        Motor_Run(1, 0);
                        Motor_Run(3, 0);
                    }
                    if (attitude.pitch >= 0.2) {
                        Val_Z = map((int)attitude.pitch, 0, 180, 0, 300);
                        Val_Z = Val_Z*Val_Z;
                        if (Val_Z > 65)
                            Val_Z = 65;
                        Motor_Run(2, -Val_Z);
                    } else if (attitude.pitch <= -0.2){
                        Val_Z = map(-(int)attitude.pitch, 0, 180, 0, 300);
                        Val_Z = Val_Z*Val_Z;
                        if (Val_Z > 65)
                            Val_Z = 65;
                        Motor_Run(2, Val_Z);
                    }else{
                        Motor_Run(2, 0);
                    }
                    printf("roll %f   pitch %f\r\n",attitude.roll,attitude.pitch);
                }



            }
        }
    } else {
        HAL_Delay(1);

    }
}