#include "k_queue.h"
#include "k_queue_cfg.h"

#ifndef EXPOSE_DATATYPES
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
#endif /* EXPOSE_DATATYPES */

KQueue_Handle_t KQueue_Create(size_t itemSize, uint32_t queueLength) {
    assert(itemSize > 0);
    assert(queueLength > 0);

    KQueue_t* pQueue = K_QUEUE_MALLOC(sizeof(KQueue_t));
    if (!pQueue)
        return NULL;

    void* pArr = K_QUEUE_MALLOC(itemSize * queueLength);
    if (!pArr) {
        K_QUEUE_FREE(pQueue);
        return NULL;
    }

    *pQueue = (KQueue_t){.Length    = queueLength,
                         .ItemSize  = itemSize,
                         .ItemsNum  = 0,
                         .pBegin    = (int8_t*)pArr,
                         .pEnd      = (int8_t*)pArr + (itemSize * queueLength),
                         .pReadFrom = (int8_t*)pArr,
                         .pWriteTo  = (int8_t*)pArr};
    return pQueue;
}

KQueue_Handle_t
KQueue_CreateStatic(KQueue_Static_t pQueueStorage[static 1],
                    size_t          itemSize,
                    uint32_t        queueLength,
                    int8_t pItemsStorage[static itemSize * queueLength]) {
    _STATIC_ASSERT(sizeof(KQueue_t) == sizeof(KQueue_Static_t));

    *(KQueue_t*)pQueueStorage =
        (KQueue_t){.Length   = queueLength,
                   .ItemSize = itemSize,
                   .ItemsNum = 0,
                   .pBegin   = (int8_t*)pItemsStorage,
                   .pEnd = (int8_t*)pItemsStorage + (itemSize * queueLength),
                   .pReadFrom = (int8_t*)pItemsStorage,
                   .pWriteTo  = (int8_t*)pItemsStorage};
    K_QUEUE_MEMSET(pItemsStorage, 0, itemSize * queueLength);
    return (KQueue_Handle_t)pQueueStorage;
}

bool KQueue_Push(KQueue_Handle_t pSelf,
                 uint32_t        itemSize,
                 int8_t          pItem[static itemSize]) {
    assert(pSelf != NULL);
    assert(pSelf->ItemSize == itemSize);

    if (pSelf->ItemsNum == pSelf->Length)
        return false;

    K_QUEUE_MEMCPY(pSelf->pWriteTo, pItem, pSelf->ItemSize);
    pSelf->ItemsNum++;
    pSelf->pWriteTo += pSelf->ItemSize;
    if (pSelf->pWriteTo == pSelf->pEnd)
        pSelf->pWriteTo = pSelf->pBegin;

    return true;
}

bool KQueue_Pop(KQueue_Handle_t pSelf,
                uint32_t        buffSize,
                int8_t          pBuffer[static buffSize]) {
    assert(pSelf != NULL);
    assert(pSelf->ItemSize == buffSize);

    if (pSelf->ItemsNum == 0)
        return false;

    K_QUEUE_MEMCPY(pBuffer, pSelf->pReadFrom, pSelf->ItemSize);
    pSelf->ItemsNum--;
    pSelf->pReadFrom += pSelf->ItemSize;
    if (pSelf->pReadFrom == pSelf->pEnd)
        pSelf->pReadFrom = pSelf->pBegin;

    return true;
}

#ifndef EXPOSE_DATATYPES
uint32_t KQueue_GetItemsNum(KQueue_Handle_t pSelf) {
    assert(pSelf != NULL);

    return pSelf->ItemsNum;
}

bool KQueue_IsEmpty(KQueue_Handle_t pSelf) {
    assert(pSelf != NULL);

    return pSelf->ItemsNum == 0;
}
#endif /* EXPOSE_DATATYPES */

void KQueue_Flush(KQueue_Handle_t pSelf) {
    assert(pSelf != NULL);

    pSelf->ItemsNum  = 0;
    pSelf->pReadFrom = (int8_t*)pSelf->pBegin;
    pSelf->pWriteTo  = (int8_t*)pSelf->pBegin;
}

void KQueue_Destroy(KQueue_Handle_t pSelf) {
    assert(pSelf != NULL);

    K_QUEUE_FREE(pSelf->pBegin);
    K_QUEUE_FREE(pSelf);
}
