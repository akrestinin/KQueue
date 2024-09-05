/**
 * @file k_queue.h
 * @version 1.0
 * @author Krestinin Aleksandr (krestinin.al99@gmail.com)
 * @brief This module provides implementation of Queue data structure. Each
 * queue object can be dynamicly or statically allocated. All operations with
 * KQueue objects should be done through handles and public functions calls.
 */

#ifndef K_QUEUE_H
#define K_QUEUE_H

#include "k_queue_cfg.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @defgroup KQueueClass Class description
 *@{
 */

#ifndef EXPOSE_DATATYPES
/**
 * @brief Opaque type for KQueue instance.
 */
typedef struct KQueue KQueue_t;
#else  /* EXPOSE_DATATYPES */
typedef struct KQueue {
    uint32_t Length;
    size_t   ItemSize;
    uint32_t ItemsNum;
    int8_t*  pBegin;
    int8_t*  pEnd;
    int8_t*  pReadFrom;
    int8_t*  pWriteTo;
} KQueue_t;
#endif /* EXPOSE_DATATYPES */

/**
 * @brief KQueue instance handle for calling methods.
 */
typedef KQueue_t* KQueue_Handle_t;

/**
 * @brief Structure for static memory allocation of KQueue object.
 */
typedef struct {
    uint32_t Dummy0;
    size_t   Dummy1;
    uint32_t Dummy2;
    void*    Dummy3;
    void*    Dummy4;
    void*    Dummy5;
    void*    Dummy6;
} KQueue_Static_t;

/** @}*/

/**
 * @defgroup KQueuePublicMethods Public methods
 *@{
 */

/**
 * @brief Create dynamicly allocated KQueue object.
 *
 * @param[in] itemSize: Size in bytes of one queue item.
 * @param[in] queueLength: Amount of items that can be stored in queue.
 *
 * @retval NULL: Error with KQueue object creation.
 * @retval KQueue_Handle_t: Handle of a created KQueue object.
 */
KQueue_Handle_t KQueue_Create(size_t itemSize, uint32_t queueLength);

/**
 * @brief Create statically allocated KQueue object.
 * @note Remember that size of pItemsStorage should be equal to size of one
 * queue item times queueSize.
 *
 * @param[in] pQueueStorage: Pointer to a static storage for KQueue object.
 * @param[in] itemSize: Size of one queue item in bytes.
 * @param[in] queueLength: Max amount of items that can be stored in queue.
 * @param[in] pItemsStorage: Pointer to a static storage for queue items.
 *
 * @retval NULL: Error with KQueue object creation.
 * @retval KQueue_Handle_t: Handle of a created KQueue object.
 */
KQueue_Handle_t KQueue_CreateStatic(KQueue_Static_t pQueueStorage[static 1],
                                    size_t          itemSize,
                                    uint32_t        queueLength,
                                    int8_t          pItemsStorage[static itemSize * queueLength]);

/**
 * @brief Push item to the end of queue.
 * @note Items are pushed by copy and not by reference.
 *
 * @param[in] pSelf: KQueue handle.
 * @param[in] itemSize: Size of an input item in bytes.
 * @param[in] pItem: Pointer to an item to be pushed.
 *
 * @retval true: Item pushed successfully.
 * @retval false: Error while pushing an item.
 */
bool KQueue_Push(KQueue_Handle_t pSelf, uint32_t itemSize, int8_t pItem[static itemSize]);
/**
 * @brief Pop item from the beginning of queue.
 * @note Items are copied to pBuffer pointer storage. Storage should be equal to
 * item size defined in queue creation.
 *
 * @param[in] pSelf: KQueue handle.
 * @param[in] buffSize: Size of an output buffer in bytes.
 * @param[out] pBuffer: Pointer to a storage in which item will be copied.
 *
 * @retval true: Item poped successfully.
 * @retval false: Error while getting an item.
 */
bool KQueue_Pop(KQueue_Handle_t pSelf, uint32_t buffSize, int8_t pBuffer[static buffSize]);

#ifndef EXPOSE_DATATYPES
/**
 * @brief Get number of items currently stored in queue.
 *
 * @param[in] pSelf: KQueue handle.
 *
 * @return uint32_t: Amount of items in queue.
 */
uint32_t KQueue_GetItemsNum(KQueue_Handle_t pSelf);

/**
 * @brief Is queue empty.
 *
 * @param[in] pSelf: KQueue handle.
 *
 * @retval true: Queue is empty.
 * @retval false: There are some items in the queue.
 */
bool KQueue_IsEmpty(KQueue_Handle_t pSelf);
#else  /* EXPOSE_DATATYPES */
inline uint32_t KQueue_GetItemsNum(KQueue_Handle_t pSelf) {
    K_QUEUE_ASSERT(pSelf != NULL);

    return pSelf->ItemsNum;
}

inline bool KQueue_IsEmpty(KQueue_Handle_t pSelf) {
    K_QUEUE_ASSERT(pSelf != NULL);

    return pSelf->ItemsNum == 0;
}
#endif /* EXPOSE_DATATYPES */

/**
 * @brief Flush all of the queue items.
 *
 * @param[in] pSelf: KQueue handle.
 */
void KQueue_Flush(KQueue_Handle_t pSelf);

/**
 * @brief Destroy KQueue object and free allocated memory.
 * @note You should destroy only dynamicly allocated KQueue objects. In other
 * cases consider using KQueue_Flush().
 *
 * @param[in] pSelf: KQueue handle.
 */
void KQueue_Destroy(KQueue_Handle_t pSelf);

/** @}*/

#endif /* K_QUEUE_H */
