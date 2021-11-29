//
// Created by bei on 2021/11/28.
//

#ifndef BALANCER_RINGBUFFER_H
#define BALANCER_RINGBUFFER_H
#include "main.h"

/*环形缓冲区数据结构*/
typedef struct
{
    uint8_t  *buffer;
    uint16_t size;
    uint16_t in;
    uint16_t out;
} RingBuffer_t;


void Ringbuffer_init(RingBuffer_t *fifo, uint8_t *buffer, uint16_t size);

uint16_t RingBuffer_getUsedSize(RingBuffer_t *fifo);
uint16_t RingBuffer_getRemainSize(RingBuffer_t *fifo);
uint8_t RingBuffer_isEmpty(RingBuffer_t *fifo);

void RingBuffer_in(RingBuffer_t *fifo, uint8_t *data, uint16_t len);
uint8_t RingBuffer_in_check(RingBuffer_t *fifo, uint8_t *data, uint16_t len);
uint16_t RingBuffer_out(RingBuffer_t *fifo, uint8_t *buf, uint16_t len);

#endif //BALANCER_RINGBUFFER_H
