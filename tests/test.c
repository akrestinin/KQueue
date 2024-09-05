#include "k_queue.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BENCHMARK_TESTS  1

#define TEST_CYCLES      1000000
#define QUEUE_LENGTH     10

#define STYLE_SYS(s)     ("\033[36m" s "\033[0m")
#define STYLE_SUCCESS(s) ("\033[32m" s "\033[0m")
#define STYLE_ERROR(s)   ("\033[31m" s "\033[0m")

#if BENCHMARK_TESTS
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
#endif /* BENCHMARK_TESTS */

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sched.h>
#include <unistd.h>
#endif

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

static void PinToCore(int core_id) {
    printf(STYLE_SYS("PinToCore: "));
#if defined(_WIN32) || defined(_WIN64)
    DWORD_PTR mask   = (DWORD_PTR)1 << core_id;
    DWORD_PTR result = SetThreadAffinityMask(GetCurrentThread(), mask);
    PrintResult(result == 0 ? false : true);
#else
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t current_thread = pthread_self();
    int       result         = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
    PrintResult(result != 0 ? false : true);
#endif
}

static KQueue_Handle_t Test_StaticCreation(void) {
    static KQueue_Static_t queueStorage;
    static TestItem_t      itemsStorage[QUEUE_LENGTH];

    printf("\tTEST 1: Create ");
    KQueue_Handle_t hQueue =
        KQueue_CreateStatic(&queueStorage, sizeof(TestItem_t), QUEUE_LENGTH, (int8_t*)&itemsStorage[0]);
    PrintResult(hQueue != NULL);

    printf("\tTEST 2: Is empty ");
    PrintResult(KQueue_IsEmpty(hQueue));

    printf("\tTEST 3: No items ");
    PrintResult(KQueue_GetItemsNum(hQueue) == 0);

    return hQueue;
}

static KQueue_Handle_t Test_DynamicCreation(void) {
    printf("\tTEST 1: Crate too big ");
    PrintResult(KQueue_Create(sizeof(TestItem_t), ~0UL) == NULL);

    printf("\tTEST 2: Create normal ");
    KQueue_Handle_t hQueue = KQueue_Create(sizeof(TestItem_t), QUEUE_LENGTH);
    PrintResult(hQueue != NULL);

    printf("\tTEST 3: Is empty ");
    PrintResult(KQueue_IsEmpty(hQueue));

    printf("\tTEST 4: No items ");
    PrintResult(KQueue_GetItemsNum(hQueue) == 0);

    return hQueue;
}

static void Test_Unit(KQueue_Handle_t hQueue) {
    printf("\tTEST 1: Pushing items \r\n");
    TestItem_t itemsArr[QUEUE_LENGTH];
    memset(itemsArr, 0, sizeof(TestItem_t) * QUEUE_LENGTH);
    for (int i = 0; i < QUEUE_LENGTH; i++) {
        TestItem_t item = {.First = rand(), .Second = rand(), .Third = rand()};

        memcpy(&itemsArr[i], &item, sizeof(TestItem_t));
        printf("\t\tPush item [%d]: First: %i\tSecond: %d\tThird: %u\t", i, item.First, item.Second, item.Third);
        PrintResult(KQueue_Push(hQueue, sizeof(TestItem_t), (int8_t*)&item));
    }

    printf("\tTEST 2: Push too much ");
    TestItem_t badItem = {.First = rand(), .Second = rand(), .Third = rand()};
    PrintResult(KQueue_Push(hQueue, sizeof(TestItem_t), (int8_t*)&badItem) == false);

    printf("\tTEST 3: Isn't empty ");
    PrintResult(KQueue_IsEmpty(hQueue) == false);

    printf("\tTEST 4: Full of items ");
    PrintResult(KQueue_GetItemsNum(hQueue) == QUEUE_LENGTH);

    printf("\tTEST 5: Poping items \r\n");
    for (int i = 0; i < QUEUE_LENGTH; i++) {
        TestItem_t item;
        memset(&item, 0, sizeof(TestItem_t));
        bool result = KQueue_Pop(hQueue, sizeof(TestItem_t), (int8_t*)&item);
        printf("\t\tPoped item [%d]: First: %i\tSecond: %d\tThird: %u\t", i, item.First, item.Second, item.Third);
        PrintResult(result);
        printf("\t\tIs equal: ");
        PrintResult(memcmp(&itemsArr[i], &item, sizeof(TestItem_t)) == 0);
    }

    printf("\tTEST 6: Pop too much ");
    PrintResult(KQueue_Pop(hQueue, sizeof(TestItem_t), (int8_t*)&badItem) == false);

    printf("\tTEST 7: Is empty ");
    PrintResult(KQueue_IsEmpty(hQueue));

    printf("\tTEST 8: No items ");
    PrintResult(KQueue_GetItemsNum(hQueue) == 0);

    printf("\tTEST 9: Flush ");

    TestItem_t item = {.First = rand(), .Second = rand(), .Third = rand()};
    KQueue_Push(hQueue, sizeof(TestItem_t), (int8_t*)&item);
    KQueue_Flush(hQueue);
    PrintResult(KQueue_IsEmpty(hQueue));
}

static void Test_Benchmark(KQueue_Handle_t hQueue) {
    printf("\tTEST 1: Up-down operations [%d]\r\n", TEST_CYCLES);
    TestItem_t itemsArr[QUEUE_LENGTH];
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
        while (KQueue_GetItemsNum(hQueue) < QUEUE_LENGTH) {
            KQueue_Push(hQueue, sizeof(TestItem_t), (int8_t*)&itemsArr[cnt]);
            cnt++;
        }

        while (!KQueue_IsEmpty(hQueue)) {
            TestItem_t item;
            KQueue_Pop(hQueue, sizeof(TestItem_t), (int8_t*)&item);
            item.First++;
        }
        end = __rdtsc();
        res += (float)(end - start);
        cnt = 0;
    }

    printf("\t\tPerformance: %f clock cycles\r\n", res / TEST_CYCLES);
}

int main(void) {
    PinToCore(0);
    printf(STYLE_SYS("Starting KQueue testing procedure...\r\n"));

    srand(time(NULL));

    printf(STYLE_SYS("--------------------------------------------------\r\n"));

    printf("Static creation:\r\n");

    KQueue_Handle_t hStaticQueue = Test_StaticCreation();

    printf(STYLE_SYS("--------------------------------------------------\r\n"));

    printf("Dynamic creation:\r\n");

    KQueue_Handle_t hDynamicQueue = Test_DynamicCreation();

    printf(STYLE_SYS("--------------------------------------------------\r\n"));

    printf(STYLE_SYS("Unit testing static version:\r\n"));

    Test_Unit(hStaticQueue);

    printf(STYLE_SYS("--------------------------------------------------\r\n"));

    printf(STYLE_SYS("Unit testing dynamic version:\r\n"));

    Test_Unit(hDynamicQueue);

    printf(STYLE_SYS("--------------------------------------------------\r\n"));

#if BENCHMARK_TESTS
    printf(STYLE_SYS("Benchmark testing static version:\r\n"));

    Test_Benchmark(hStaticQueue);

    printf(STYLE_SYS("--------------------------------------------------\r\n"));

    printf(STYLE_SYS("Benchmark testing dynamic version:\r\n"));

    Test_Benchmark(hDynamicQueue);

    printf(STYLE_SYS("--------------------------------------------------\r\n"));
#endif /* BENCHMARK_TESTS */

    KQueue_Destroy(hDynamicQueue);

    printf(STYLE_SYS("KQueue testing procedure completed!\r\n"));

    return 0;
}
