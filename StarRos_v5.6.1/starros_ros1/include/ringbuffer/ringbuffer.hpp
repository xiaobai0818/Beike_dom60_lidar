/**
 * @file ringbuffer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "lwrb.h"

class ringbuffer
{
public:
    /**
     * @brief Construct a new ringbuffer object
     * 
     * @param size 
     */
    ringbuffer(uint32_t size);

    /**
     * @brief Destroy the ringbuffer object
     * 
     */
    ~ringbuffer();

    /**
     * @brief 初始化
     * 
     * @return uint32_t 
     */
    uint32_t init();

    /**
     * @brief 解除初始化
     * 
     * @return uint32_t 
     */
    uint32_t deinit();

    /**
     * @brief 读数据
     * 
     * @param p 数据存放数组
     * @param lens 希望读取数据大小，并将ringbuffer中读指针移动
     * @return uint32_t 实际读取数据大小
     */
    uint32_t read(uint8_t*p ,uint32_t lens);

    /**
     * @brief 窥读数据
     * 
     * @param p 数据存放数组
     * @param lens 希望读取数据大小，不改变ringbuffer中读指针
     * @return uint32_t 实际读取数据大小
     */
    uint32_t read_without_free(uint8_t*p ,uint32_t lens);

    /**
     * @brief 移动读指针，释放ringbuffer
     * 
     * @param lens 移动读指针
     * @return uint32_t 
     */
    uint32_t free(uint32_t lens);

    /**
     * @brief 零拷贝读取
     * 
     * @param p 
     * @param lens 
     * @return uint32_t 
     */
    uint32_t get_read(uint8_t **p ,int32_t &lens);

    /**
     * @brief 向ringbuffer中写数据
     * 
     * @param p 数据
     * @param lens 待写入数据大小
     * @return uint32_t 实际写入数据大小
     */
    uint32_t write(uint8_t*p ,uint32_t lens);

    /**
     * @brief 获取当前可写入大小
     * 
     * @return uint32_t 当前可写入字节数
     */
    uint32_t get_free();

    /**
     * @brief 获取初始化时设置缓存区大小
     * 
     * @return uint32_t 当前可写入字节数
     */
    inline uint32_t get_set_size(){ return m_size; }

private:
    
    /// @brief lwrb类
    lwrb_t m_buff;

    /// @brief 缓存指针
    uint8_t *m_pbuffptr;

    /// @brief 缓存大小
    uint32_t m_size;
};

ringbuffer::ringbuffer(uint32_t size)
{
    m_size = size;
    uint8_t rtn = 0;
    m_pbuffptr = new uint8_t[m_size];
    lwrb_init(&m_buff, (void*) m_pbuffptr, m_size);
}

ringbuffer::~ringbuffer()
{
    lwrb_free(&m_buff);

    if (nullptr != m_pbuffptr)
    {
        delete[] m_pbuffptr;
        m_pbuffptr = nullptr;
    }
}

uint32_t ringbuffer::read(uint8_t*p ,uint32_t lens)
{
    if (nullptr == m_pbuffptr)
    {
        return 0;
    }

    return lwrb_read(&m_buff, p, lens);

}

uint32_t ringbuffer::read_without_free(uint8_t*p ,uint32_t lens)
{
    if (nullptr == m_pbuffptr)
    {
        return 0;
    }

    return lwrb_peek(&m_buff, 0, p, lens);

}

uint32_t ringbuffer::free(uint32_t lens)
{
    if (nullptr == m_pbuffptr)
    {
        return 0;
    }
    return lwrb_skip(&m_buff, lens);
}

uint32_t ringbuffer::get_read(uint8_t **p ,int32_t &lens)
{
    if (nullptr == m_pbuffptr)
    {
        return 0;
    }
    *p = (uint8_t*)lwrb_get_linear_block_read_address(&m_buff);
    lens = lwrb_get_linear_block_read_length(&m_buff);
    return lens;
}

uint32_t ringbuffer::write(uint8_t*p ,uint32_t lens)
{
    if (nullptr == m_pbuffptr)
    {
        return 0;
    }

    return lwrb_write(&m_buff, p, lens);
}

uint32_t ringbuffer::get_free()
{
    if (nullptr == m_pbuffptr)
    {
        return 0;
    }

    return lwrb_get_free(&m_buff);
}