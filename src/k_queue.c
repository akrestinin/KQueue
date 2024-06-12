#include "k_queue.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief KQueue data type structure.
 */
typedef struct KQueue {
    uint32_t Length;    /**< Max amount of items that can be stored in queue. */
    size_t   ItemSize;  /**< Size of one queue item in bytes. */
    uint32_t ItemsNum;  /**< Amount of items currently stored in queue. */
    int8_t*  pBegin;    /**< Pointer to items storage beginning. */
    int8_t*  pEnd;      /**< Pointer to items storage ending. */
    int8_t*  pReadFrom; /**< Pointer to read next item data. */
    int8_t*  pWriteTo;  /**< Pointer to write next item data. */
} KQueue_t;

KQueue_Handle_t KQueue_Create(size_t itemSize, uint32_t queueLength) {
    KQueue_t* pQueue = malloc(sizeof(KQueue_t));
    void*     pArr   = malloc(itemSize * queueLength);
    if (pQueue == NULL || pArr == NULL)
        return NULL;

    pQueue->Length    = queueLength;
    pQueue->ItemSize  = itemSize;
    pQueue->ItemsNum  = 0;
    pQueue->pBegin    = (int8_t*)pArr;
    pQueue->pEnd      = (int8_t*)pArr + (itemSize * queueLength);
    pQueue->pReadFrom = (int8_t*)pArr;
    pQueue->pWriteTo  = (int8_t*)pArr;

    return pQueue;
}

KQueue_Handle_t KQueue_CreateStatic(KQueue_Static_t* pQueueStorage,
                                    int8_t* pItemsStorage, size_t itemSize,
                                    uint32_t queueLength) {
    assert(pQueueStorage != NULL);
    assert(pItemsStorage != NULL);
    assert(sizeof(KQueue_t) == sizeof(KQueue_Static_t));

    KQueue_t* pNewQueue = (KQueue_t*)pQueueStorage;
    memset(pItemsStorage, 0, itemSize * queueLength);
    pNewQueue->Length    = queueLength;
    pNewQueue->ItemSize  = itemSize;
    pNewQueue->ItemsNum  = 0;
    pNewQueue->pBegin    = pItemsStorage;
    pNewQueue->pEnd      = pItemsStorage + (itemSize * queueLength);
    pNewQueue->pReadFrom = pItemsStorage;
    pNewQueue->pWriteTo  = pItemsStorage;

    return (KQueue_Handle_t)pNewQueue;
}

bool KQueue_Push(KQueue_Handle_t pSelf, void* pItem) {
    assert(pSelf != NULL);
    assert(pItem != NULL);

    if (pSelf->ItemsNum == pSelf->Length)
        return false;

    memcpy(pSelf->pWriteTo, pItem, pSelf->ItemSize);
    pSelf->ItemsNum++;
    pSelf->pWriteTo += pSelf->ItemSize;
    if (pSelf->pWriteTo == pSelf->pEnd)
        pSelf->pWriteTo = pSelf->pBegin;

    return true;
}

bool KQueue_Pop(KQueue_Handle_t pSelf, void* pBuffer) {
    assert(pSelf != NULL);
    assert(pBuffer != NULL);

    if (pSelf->ItemsNum == 0)
        return false;

    memcpy(pBuffer, pSelf->pReadFrom, pSelf->ItemSize);
    pSelf->ItemsNum--;
    pSelf->pReadFrom += pSelf->ItemSize;
    if (pSelf->pReadFrom == pSelf->pEnd)
        pSelf->pReadFrom = pSelf->pBegin;

    return true;
}

uint32_t KQueue_GetItemsNum(KQueue_Handle_t pSelf) {
    assert(pSelf != NULL);

    return pSelf->ItemsNum;
}

bool KQueue_IsEmpty(KQueue_Handle_t pSelf) {
    assert(pSelf != NULL);

    return pSelf->ItemsNum == 0;
}

void KQueue_Destroy(KQueue_Handle_t pSelf) {
    assert(pSelf != NULL);
    free(pSelf->pBegin);
    free(pSelf);
}
