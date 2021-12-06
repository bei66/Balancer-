//
// Created by bei on 2021/12/3.
//

#include "Control.h"

#define L_roll 14
#define L_pitch 14
#define STEP_mm 0.03349

void Control_motor(void) {
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
                Motor_run(1, Length_X);
                Motor_run(2, Length_Y);
                Motor_run(3, Length_Z);

            }
        }
    } else {
        HAL_Delay(1);

    }
}