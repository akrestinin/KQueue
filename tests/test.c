#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "k_queue.h"

#define TEST_CYCLES                 100
#define QUEUE_LENGTH                10

#define PRINT(_f_, ...)             printf((_f_), ##__VA_ARGS__)

#define PRINT_SYS(_f_, ...)         do { \
                                        PRINT("\033[36m"); \
                                        PRINT((_f_), ##__VA_ARGS__); \
                                        PRINT("\033[0m"); \
                                    } while(0)

#define PRINT_SUCCESS(_f_, ...)     do { \
                                        PRINT("\033[32m"); \
                                        PRINT((_f_), ##__VA_ARGS__); \
                                        PRINT("\033[0m"); \
                                    } while(0)

#define PRINT_ERROR(_f_, ...)       do { \
                                        PRINT("\033[31m"); \
                                        PRINT((_f_), ##__VA_ARGS__); \
                                        PRINT("\033[0m"); \
                                    } while(0)

typedef struct TestItem
{
    int32_t First;
    int32_t Second;
    int32_t Third;
} TestItem_t;

static void PrintResult(bool result)
{
    if(result)
        PRINT_SUCCESS("SUCCESS\r\n");
    else
        PRINT_ERROR("FAILED\r\n");
}

int main(void)
{
    PRINT_SYS("Starting KQueue testing procedure...\r\n");

    srand(time(NULL));

    PRINT_SYS("----------------------------------------------------------\r\n");

    PRINT_SYS("Queue creation:\r\n");

    PRINT("\tTEST 1: Crate too big ");
    PrintResult(KQueue_Create(sizeof(TestItem_t), ~0UL) == NULL);

    PRINT("\tTEST 2: Create normal ");
    KQueue_Handle_t qHandle = KQueue_Create(sizeof(TestItem_t), QUEUE_LENGTH);
    PrintResult(qHandle != NULL);

    PRINT("\tTEST 3: Is empty ");
    PrintResult(KQueue_IsEmpty(qHandle));

    PRINT("\tTEST 4: No items ");
    PrintResult(KQueue_ItemsNum(qHandle) == 0);

    PRINT_SYS("----------------------------------------------------------\r\n");

    PRINT_SYS("Queue push items:\r\n");

    PRINT("\tTEST 1: Pushing items \r\n");
    TestItem_t itemsArr[QUEUE_LENGTH] = {};
    for(int i = 0; i < QUEUE_LENGTH; i++)
    {
        TestItem_t item = {
            .First = rand(),
            .Second = rand(),
            .Third = rand()
        };

        memcpy(&itemsArr[i], &item, sizeof(TestItem_t));
        PRINT("\t\tPush item [%d]: First: %i\tSecond: %d\tThird: %u\t",
                i, item.First, item.Second, item.Third);
        PrintResult(KQueue_Push(qHandle, &item));
    }

    PRINT("\tTEST 2: Push too much ");
    TestItem_t badItem = {
        .First = rand(),
        .Second = rand(),
        .Third = rand()
    };
    PrintResult(KQueue_Push(qHandle, &badItem) == false);

    PRINT("\tTEST 3: Isn't empty ");
    PrintResult(KQueue_IsEmpty(qHandle) == false);

    PRINT("\tTEST 4: Full of items ");
    PrintResult(KQueue_ItemsNum(qHandle) == QUEUE_LENGTH);

    PRINT_SYS("----------------------------------------------------------\r\n");

    PRINT_SYS("Queue pop items:\r\n");

    PRINT("\tTEST 1: Poping items \r\n");
    for(int i = 0; i < QUEUE_LENGTH; i++)
    {
        TestItem_t item;
        memset(&item, 0, sizeof(TestItem_t));
        bool result = KQueue_Pop(qHandle, &item);
        PRINT("\t\tPoped item [%d]: First: %i\tSecond: %d\tThird: %u\t",
                i, item.First, item.Second, item.Third);
        PrintResult(result);
        PRINT("\t\tIs equal: ");
        PrintResult(memcmp(&itemsArr[i], &item, sizeof(TestItem_t)) == 0);
    }

    PRINT("\tTEST 2: Pop too much ");
    PrintResult(KQueue_Pop(qHandle, &badItem) == false);

    PRINT("\tTEST 3: Is empty ");
    PrintResult(KQueue_IsEmpty(qHandle));

    PRINT("\tTEST 4: No items ");
    PrintResult(KQueue_ItemsNum(qHandle) == 0);

    PRINT_SYS("----------------------------------------------------------\r\n");

    PRINT_SYS("Queue random operations check:\r\n");

    PRINT("\tTEST 1: Random operations [%d] ", TEST_CYCLES);

    memset(&itemsArr[0], 0, sizeof(TestItem_t) * QUEUE_LENGTH);
    uint32_t num = 0;
    TestItem_t itemBuff;
    for(uint32_t i = 0; i < TEST_CYCLES; i++)
    {
        bool PopOrPush = rand() > 0 ? true : false;
        if(PopOrPush)
        {
            itemBuff.First = rand();
            itemBuff.Second = rand();
            itemBuff.Third = rand();
            bool result = KQueue_Push(qHandle, &itemBuff);
            if( (num >= QUEUE_LENGTH && result == true) ||
                (num < QUEUE_LENGTH && result == false))
            {
                PrintResult(false);
                return 1;
            }

            if(num < QUEUE_LENGTH)
            {
                memcpy(&itemsArr[num], &itemBuff, sizeof(TestItem_t));
                num++;
                if(KQueue_ItemsNum(qHandle) != num)
                {
                    PrintResult(false);
                    return 1;
                }
            }
            else
            {
                if(KQueue_ItemsNum(qHandle) != num)
                {
                    PrintResult(false);
                    return 1;
                }
            }
        }
        else
        {
            memset(&itemBuff, 0, sizeof(TestItem_t));
            bool result = KQueue_Pop(qHandle, &itemBuff);
            if( (num == 0 && result == true) ||
                (num != 0 && result == false))
            {
                PrintResult(false);
                return 1;
            }

            if(num == 0)
            {
                if( KQueue_ItemsNum(qHandle) != 0 ||
                    KQueue_IsEmpty(qHandle) != true)
                {
                    PrintResult(false);
                    return 1;
                }
            }
            else
            {
                if(memcmp(&itemsArr[num], &itemBuff, sizeof(TestItem_t)) != 0)
                {
                    PrintResult(false);
                    return 1;
                }

                memset(&itemsArr[num], 0, sizeof(TestItem_t));
                num--;
                if(KQueue_ItemsNum(qHandle) != num)
                {
                    PrintResult(false);
                    return 1;
                }
            }
        }
    }

    PrintResult(true);

    PRINT_SYS("----------------------------------------------------------\r\n");

    KQueue_Destroy(qHandle);

    PRINT_SYS("KQueue testing procedure completed!\r\n");

    return 0;
}
