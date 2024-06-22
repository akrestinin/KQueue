#include "k_queue.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BENCHMARK_TESTS  0

#define TEST_CYCLES      1000000
#define QUEUE_LENGTH     10

#define STYLE_SYS(s)     ("\033[36m" s "\033[0m")
#define STYLE_SUCCESS(s) ("\033[32m" s "\033[0m")
#define STYLE_ERROR(s)   ("\033[31m" s "\033[0m")

#if BENCHMARK_TESTS
#include "x86intrin.h"
#endif /* BENCHMARK_TESTS */

typedef struct TestItem {
    int32_t First;
    int32_t Second;
    int32_t Third;
} TestItem_t;

static void PrintResult(bool result) {
    if (result)
        printf(STYLE_SUCCESS("SUCCESS\r\n"));
    else
        printf(STYLE_ERROR("FAILED\r\n"));
}

int main(void) {
    printf(STYLE_SYS("Starting KQueue testing procedure...\r\n"));

    srand(time(NULL));

    printf(STYLE_SYS("--------------------------------------------------\r\n"));

    printf(STYLE_SYS("Unit testing:\r\n"));

    printf("\tTEST 1: Crate too big ");
    PrintResult(KQueue_Create(sizeof(TestItem_t), ~0UL) == NULL);

    printf("\tTEST 2: Create normal ");
    KQueue_Handle_t qHandle = KQueue_Create(sizeof(TestItem_t), QUEUE_LENGTH);
    PrintResult(qHandle != NULL);

    printf("\tTEST 3: Is empty ");
    PrintResult(KQueue_IsEmpty(qHandle));

    printf("\tTEST 4: No items ");
    PrintResult(KQueue_GetItemsNum(qHandle) == 0);

    printf("\tTEST 5: Pushing items \r\n");
    TestItem_t itemsArr[QUEUE_LENGTH];
    memset(itemsArr, 0, sizeof(TestItem_t) * QUEUE_LENGTH);
    for (int i = 0; i < QUEUE_LENGTH; i++) {
        TestItem_t item = {.First = rand(), .Second = rand(), .Third = rand()};

        memcpy(&itemsArr[i], &item, sizeof(TestItem_t));
        printf("\t\tPush item [%d]: First: %i\tSecond: %d\tThird: %u\t",
               i,
               item.First,
               item.Second,
               item.Third);
        PrintResult(KQueue_Push(qHandle, &item));
    }

    printf("\tTEST 6: Push too much ");
    TestItem_t badItem = {.First = rand(), .Second = rand(), .Third = rand()};
    PrintResult(KQueue_Push(qHandle, &badItem) == false);

    printf("\tTEST 7: Isn't empty ");
    PrintResult(KQueue_IsEmpty(qHandle) == false);

    printf("\tTEST 8: Full of items ");
    PrintResult(KQueue_GetItemsNum(qHandle) == QUEUE_LENGTH);

    printf("\tTEST 9: Poping items \r\n");
    for (int i = 0; i < QUEUE_LENGTH; i++) {
        TestItem_t item;
        memset(&item, 0, sizeof(TestItem_t));
        bool result = KQueue_Pop(qHandle, &item);
        printf("\t\tPoped item [%d]: First: %i\tSecond: %d\tThird: %u\t",
               i,
               item.First,
               item.Second,
               item.Third);
        PrintResult(result);
        printf("\t\tIs equal: ");
        PrintResult(memcmp(&itemsArr[i], &item, sizeof(TestItem_t)) == 0);
    }

    printf("\tTEST 10: Pop too much ");
    PrintResult(KQueue_Pop(qHandle, &badItem) == false);

    printf("\tTEST 11: Is empty ");
    PrintResult(KQueue_IsEmpty(qHandle));

    printf("\tTEST 12: No items ");
    PrintResult(KQueue_GetItemsNum(qHandle) == 0);

    printf("\tTEST 13: Flush ");

    TestItem_t item = {.First = rand(), .Second = rand(), .Third = rand()};
    KQueue_Push(qHandle, &item);
    KQueue_Flush(qHandle);
    PrintResult(KQueue_IsEmpty(qHandle));

    printf(STYLE_SYS("--------------------------------------------------\r\n"));
#if BENCHMARK_TESTS
    printf(STYLE_SYS("Benchmark testing:\r\n"));

    printf("\tTEST 1: Up-down operations [%d]\r\n", TEST_CYCLES);

    KQueue_Flush(qHandle);
    memset(&itemsArr[0], 0, sizeof(TestItem_t) * QUEUE_LENGTH);
    for (int i = 0; i < QUEUE_LENGTH; i++) {
        itemsArr[i].First  = rand();
        itemsArr[i].Second = rand();
        itemsArr[i].Third  = rand();
    }

    uint64_t start, end;
    float    res = 0;
    uint32_t cnt = 0;
    for (int i = 0; i < TEST_CYCLES; i++) {
        start = __rdtsc();
        while (KQueue_GetItemsNum(qHandle) < QUEUE_LENGTH) {
            KQueue_Push(qHandle, &itemsArr[cnt]);
            cnt++;
        }

        while (!KQueue_IsEmpty(qHandle)) {
            TestItem_t item;
            KQueue_Pop(qHandle, &item);
            item.First++;
        }
        end = __rdtsc();
        res += (float)(end - start);
        cnt = 0;
    }

    printf("\t\tPerformance: %f clock cycles\r\n", res / TEST_CYCLES);
    printf(STYLE_SYS("--------------------------------------------------\r\n"));
#endif /* BENCHMARK_TESTS */

    KQueue_Destroy(qHandle);

    printf(STYLE_SYS("KQueue testing procedure completed!\r\n"));

    return 0;
}
