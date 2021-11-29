//
// Created by bei on 2021/11/28.
//

#include "RingBuffer.h"

/**
  * @brief  fifo��ʼ��
  * @param  fifo: ʵ��
  * @param  buffer: fifo�Ļ�����
  * @param  size: ��������С
  * @retval None
  */
void Ringbuffer_init(RingBuffer_t *fifo, uint8_t *buffer, uint16_t size)
{
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = 0;
    fifo->out = 0;
}


/**
  * @brief  ��ȡ�Ѿ�ʹ�õĿռ�
  * @param  fifo: ʵ��
  * @retval uint16_t: ��ʹ�ø���
  */
uint16_t RingBuffer_getUsedSize(RingBuffer_t *fifo)
{
    if (fifo->in >= fifo->out)
        return (fifo->in - fifo->out);
    else
        return (fifo->size - fifo->out + fifo->in);
}


/**
  * @brief  ��ȡδʹ�ÿռ�
  * @param  fifo: ʵ��
  * @retval uint16_t: ʣ�����
  */
uint16_t RingBuffer_getRemainSize(RingBuffer_t *fifo)
{
    return (fifo->size - RingBuffer_getUsedSize(fifo) - 1);
}


/**
  * @brief  FIFO�Ƿ�Ϊ��
  * @param  fifo: ʵ��
  * @retval uint8_t: 1 Ϊ�� 0 ��Ϊ�գ������ݣ�
  */
uint8_t RingBuffer_isEmpty(RingBuffer_t *fifo)
{
    return (fifo->in == fifo->out);
}


/**
  * @brief  �������ݵ����λ������������ʣ��ռ䣩
  * @param  fifo: ʵ��
  * @param  data: &#&
  * @param  len: &#&
  * @retval none
  */
void RingBuffer_in(RingBuffer_t *fifo, uint8_t *data, uint16_t len)
{
    for (int i = 0; i < len; i++)
    {
        fifo->buffer[fifo->in] = data[i];
        fifo->in = (fifo->in + 1) % fifo->size;
    }
}


/**
  * @brief  �������ݵ����λ�����(��ʣ��ռ��⣬�ռ䲻�㷢��ʧ��)
  * @param  fifo: ʵ��
  * @param  data: &#&
  * @param  len: &#&
  * @retval uint8_t: 0 �ɹ� 1ʧ�ܣ��ռ䲻�㣩
  */
uint8_t RingBuffer_in_check(RingBuffer_t *fifo, uint8_t *data, uint16_t len)
{
    uint16_t remainsize = RingBuffer_getRemainSize(fifo);

    if (remainsize < len) //�ռ䲻��
        return 1;

    RingBuffer_in(fifo, data, len);

    return 0;
}


/**
  * @brief  �ӻ��λ�������ȡ����
  * @param  fifo: ʵ��
  * @param  buf: �������
  * @param  len: ������鳤��
  * @retval uint16_t: ʵ�ʶ�ȡ����
  */
uint16_t RingBuffer_out(RingBuffer_t *fifo, uint8_t *buf, uint16_t len)
{
    uint16_t remainToread = RingBuffer_getUsedSize(fifo);

    if (remainToread > len)
    {
        remainToread = len;
    }

    for (int i = 0; i < remainToread; i++)
    {
        buf[i] = fifo->buffer[fifo->out];
        fifo->out = (fifo->out + 1) % fifo->size;
    }

    return remainToread;
}
