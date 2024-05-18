/**
 * @file k_queue.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef K_QUEUE_H
#define K_QUEUE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct KQueue KQueue_t;

typedef KQueue_t* KQueue_Handle_t;

KQueue_Handle_t KQueue_Create(size_t itemSize, uint32_t queueLength);

bool KQueue_Push(KQueue_Handle_t self, void* pItem);

bool KQueue_Pop(KQueue_Handle_t self, void* pBuffer);

uint32_t KQueue_ItemsNum(KQueue_Handle_t self);

bool KQueue_IsEmpty(KQueue_Handle_t self);

void KQueue_Destroy(KQueue_Handle_t self);

#endif /* K_QUEUE_H */