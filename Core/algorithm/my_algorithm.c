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
    result = temp;//此处result为sin(x)结果的2^13倍
    for (i = 0; temp >>= 1; i++);  //最高位位数，从 0开始
    //下面把result转换成float内存格式
    result = (result << 23 - i) + (i + 113 << 23);  //i+113=127-(13-i)-1
    return *((float *) &result);
}


