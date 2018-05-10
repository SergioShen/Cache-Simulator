//
// Name: main
// Project: Cache
// Author: Shen Sijie
// Date: 5/8/18
//

#include <stdio.h>
#include <string.h>
#include "cache.h"
#include "memory.h"
#include "config.h"
#include "utility.h"

bool debug_enabled;
bool initializing;

Memory *memory;
Cache *l1;
FILE *trace_file;

#ifdef BI_LEVEL
Cache *l2;
#endif

#ifdef TRI_LEVEL
Cache *l2;
Cache *l3;
#endif

void Initialize(int argc, char **argv) {
    initializing = true;
    // Build cache hierarchy
    DEBUG("Memory initializing\n");
    memory = new Memory();
    memory->SetLatency(get_memory_latency());
    memory->SetStats(get_zero_stats());
    DEBUG("Memory initialized\n");

    l1 = new Cache();
    l1->SetLatency(get_l1_cache_latency());
    l1->SetConfig(get_l1_cache_config());
    l1->SetStats(get_zero_stats());
    l1->SetPrefetchConfig(get_l1_prefetch_config());
    DEBUG("L1 cache initialized\n");
#if defined(BI_LEVEL) || defined(TRI_LEVEL)
    l2 = new Cache();
    l2->SetLatency(get_l2_cache_latency());
    l2->SetConfig(get_l2_cache_config());
    l2->SetStats(get_zero_stats());
    l2->SetPrefetchConfig(get_l2_prefetch_config());
    DEBUG("L2 cache initialized\n");
#endif

#if defined(TRI_LEVEL)
    l3 = new Cache();
    l3->SetLatency(get_l3_cache_latency());
    l3->SetConfig(get_l3_cache_config());
    l3->SetStats(get_zero_stats());
    l3->SetPrefetchConfig(get_l3_prefetch_config());
    DEBUG("L3 cache initialized\n");
#endif

#if defined(BI_LEVEL)
    l2->SetLower(memory);
    l1->SetLower(l2);
#elif defined(TRI_LEVEL)
    l3->SetLower(memory);
    l2->SetLower(l3);
    l1->SetLower(l2);
#else
    l1->SetLower(memory);
#endif

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-d"))
            debug_enabled = true;
        else {
            if (i != argc - 1)
                fprintf(stderr, "Invalid command\n");
            DEBUG("Open trace file: %s\n", argv[i]);
            trace_file = fopen(argv[i], "r");
        }
    }

    initializing = false;
}

void PrintStats() {
    int64_t total_time = 0;
    printf("\n********\n");
    StorageStats stats;
    l1->GetStats(stats);
    total_time += stats.access_time;
    printf("Total L1 access time: %d ns, access count: %d\n", stats.access_time, stats.access_counter);
    printf("        miss num: %d, replace num: %d\n", stats.miss_num, stats.replace_num);
    printf("        fetch num: %d, prefetch num: %d\n", stats.fetch_num, stats.prefetch_num);

#if defined(BI_LEVEL) || defined(TRI_LEVEL)
    l2->GetStats(stats);
    total_time += stats.access_time;
    printf("Total L2 access time: %d ns, access count: %d\n", stats.access_time, stats.access_counter);
    printf("        miss num: %d, replace num: %d\n", stats.miss_num, stats.replace_num);
    printf("        fetch num: %d, prefetch num: %d\n", stats.fetch_num, stats.prefetch_num);
#endif

#if defined(TRI_LEVEL)
    l3->GetStats(stats);
    total_time += stats.access_time;
    printf("Total L3 access time: %d ns, access count: %d\n", stats.access_time, stats.access_counter);
    printf("        miss num: %d, replace num: %d\n", stats.miss_num, stats.replace_num);
    printf("        fetch num: %d, prefetch num: %d\n", stats.fetch_num, stats.prefetch_num);
#endif

    memory->GetStats(stats);
    total_time += stats.access_time;
    printf("Total memory access time: %d cycle, access count: %d\n", stats.access_time, stats.access_counter);
    printf("TOTAL ACCESS TIME: %d ns\n", total_time);
}

int main(int argc, char **argv) {
    DEBUG("Start up\n");
    Initialize(argc, argv);

    int hit, time;
    char method[8];
    uint64_t address;
    while (fscanf(trace_file, "%s%ld", method, &address) == 2) {
        DEBUG("\n*** Trace: %s %16.16lx\n", method, address);
        l1->HandleRequest(address, 1, method[0] == 'r', hit, time);
        DEBUG("--- Request time: %d\n", time);
    }

    PrintStats();
    return 0;
}
