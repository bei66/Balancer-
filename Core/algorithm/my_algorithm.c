//
// Created by bei on 2021/11/15.
//

#include "my_algorithm.h"

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int abs(int nub) {
    int temp;
    if (nub < 0) temp = -nub;
    else temp = nub;
    return nub;
}

float Sin(int x) {
    int i = 0;
    long int result = 0, temp = 0;
    temp = (180 - x) * x;
    result = temp;//�˴�resultΪsin(x)�����2^13��
    for (i = 0; temp >>= 1; i++);  //���λλ������ 0��ʼ
    //�����resultת����float�ڴ��ʽ
    result = (result << 23 - i) + (i + 113 << 23);  //i+113=127-(13-i)-1
    return *((float *) &result);
}


