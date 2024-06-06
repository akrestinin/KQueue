#include "k_queue.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct KQueue {
    uint32_t Length;
    size_t   ItemSize;
    uint32_t ItemsNum;
    int8_t*  Begin_p;
    int8_t*  End_p;
    int8_t*  ReadFrom_p;
    int8_t*  WriteTo_p;
} KQueue_t;

KQueue_Handle_t KQueue_Create(size_t itemSize, uint32_t queueLength) {
    KQueue_t* pQueue = malloc(sizeof(KQueue_t));
    void*     pArr = malloc(itemSize * queueLength);
    if(pQueue == NULL || pArr == NULL)
        return NULL;

    pQueue->Length = queueLength;
    pQueue->ItemSize = itemSize;
    pQueue->ItemsNum = 0;
    pQueue->Begin_p = (int8_t*)pArr;
    pQueue->End_p = (int8_t*)pArr + (itemSize * queueLength);
    pQueue->ReadFrom_p = (int8_t*)pArr;
    pQueue->WriteTo_p = (int8_t*)pArr;

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
    pNewQueue->Length = queueLength;
    pNewQueue->ItemSize = itemSize;
    pNewQueue->ItemsNum = 0;
    pNewQueue->Begin_p = pItemsStorage;
    pNewQueue->End_p = pItemsStorage + (itemSize * queueLength);
    pNewQueue->ReadFrom_p = pItemsStorage;
    pNewQueue->WriteTo_p = pItemsStorage;

    return (KQueue_Handle_t)pNewQueue;
}

bool KQueue_Push(KQueue_Handle_t self, void* pItem) {
    assert(self != NULL);
    assert(pItem != NULL);

    if(self->ItemsNum == self->Length)
        return false;

    memcpy(self->WriteTo_p, pItem, self->ItemSize);
    self->ItemsNum++;
    self->WriteTo_p += self->ItemSize;
    if(self->WriteTo_p == self->End_p)
        self->WriteTo_p = self->Begin_p;

    return true;
}

bool KQueue_Pop(KQueue_Handle_t self, void* pBuffer) {
    assert(self != NULL);
    assert(pBuffer != NULL);

    if(self->ItemsNum == 0)
        return false;

    memcpy(pBuffer, self->ReadFrom_p, self->ItemSize);
    self->ItemsNum--;
    self->ReadFrom_p += self->ItemSize;
    if(self->ReadFrom_p == self->End_p)
        self->ReadFrom_p = self->Begin_p;

    return true;
}

uint32_t KQueue_ItemsNum(KQueue_Handle_t self) {
    assert(self != NULL);

    return self->ItemsNum;
}

bool KQueue_IsEmpty(KQueue_Handle_t self) {
    assert(self != NULL);

    return self->ItemsNum == 0;
}

void KQueue_Destroy(KQueue_Handle_t self) {
    assert(self != NULL);
    free(self->Begin_p);
    free(self);
}
