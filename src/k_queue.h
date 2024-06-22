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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @defgroup KQueueClass Class description
 *@{
 */

/**
 * @brief Opaque type for KQueue instance.
 */
typedef struct KQueue KQueue_t;

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
 * @param[in] pItemsStorage: Pointer to a static storage for queue items.
 * @param[in] itemSize: Size of one queue item in bytes.
 * @param[in] queueLength: Max amount of items that can be stored in queue.
 *
 * @retval NULL: Error with KQueue object creation.
 * @retval KQueue_Handle_t: Handle of a created KQueue object.
 */
KQueue_Handle_t KQueue_CreateStatic(KQueue_Static_t* pQueueStorage,
                                    int8_t* pItemsStorage, size_t itemSize,
                                    uint32_t queueLength);

/**
 * @brief Push item to the end of queue.
 * @note Items are pushed by copy and not by reference.
 *
 * @param[in] pSelf: KQueue handle.
 * @param[in] pItem: Pointer to an item to be pushed.
 *
 * @retval true: Item pushed successfully.
 * @retval false: Error while pushing an item.
 */
bool KQueue_Push(KQueue_Handle_t pSelf, void* pItem);

/**
 * @brief Pop item from the beginning of queue.
 * @note Items are copied to pBuffer pointer storage. Storage should be equal to
 * item size defined in queue creation.
 *
 * @param[in] pSelf: KQueue handle.
 * @param[out] pBuffer: Pointer to a storage in which item will be copied.
 *
 * @retval true: Item poped successfully.
 * @retval false: Error while getting an item.
 */
bool KQueue_Pop(KQueue_Handle_t pSelf, void* pBuffer);

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
