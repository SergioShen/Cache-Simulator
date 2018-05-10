//
// Name: config
// Project: Cache
// Author: Shen Sijie
// Date: 5/8/18
//

#include "config.h"
#include "utility.h"
#include <string.h>

StorageLatency get_memory_latency() {
    StorageLatency latency;
    latency.bus_latency = 3;
    latency.hit_latency = 100 * NANOSECONDS_PER_CYCLE;

    return latency;
}


StorageLatency get_l1_cache_latency() {
    StorageLatency latency;
    latency.bus_latency = 1;
    latency.hit_latency = 1 * NANOSECONDS_PER_CYCLE;

    return latency;
}

CacheConfig get_l1_cache_config() {
    CacheConfig config;
    config.size = 32 * 1024;
    config.block_size = 64;
    config.associativity = 8;
    config.set_num = 64;
    config.write_through = 0;
    config.write_allocate = 1;
    config.num_of_bits_block = 6;
    config.num_of_bits_index = 6;

    ASSERT(config.size = config.block_size * config.associativity * config.set_num);
    ASSERT(config.block_size == 1 << config.num_of_bits_block);
    ASSERT(config.set_num == 1 << config.num_of_bits_index);

    return config;
}

PrefetchConfig get_l1_prefetch_config() {
    PrefetchConfig config;
    config.prefetch = true;
    config.prefetch_num = 1;

    return config;
}

StorageStats get_zero_stats() {
    StorageStats stats;
    memset(&stats, 0, sizeof(stats));

    return stats;
}

#ifdef MULTI_LEVEL

StorageLatency get_l2_cache_latency() {
    StorageLatency latency;
    latency.bus_latency = 1;
    latency.hit_latency = 8 * NANOSECONDS_PER_CYCLE;

    return latency;
}

StorageLatency get_l3_cache_latency() {
    StorageLatency latency;
    latency.bus_latency = 1;
    latency.hit_latency = 20 * NANOSECONDS_PER_CYCLE;

    return latency;
}

CacheConfig get_l2_cache_config() {
    CacheConfig config;
    config.size = 256 * 1024;
    config.block_size = 64;
    config.associativity = 8;
    config.set_num = 512;
    config.write_through = 0;
    config.write_allocate = 1;
    config.num_of_bits_block = 6;
    config.num_of_bits_index = 9;

    ASSERT(config.size = config.block_size * config.associativity * config.set_num);
    ASSERT(config.block_size == 1 << config.num_of_bits_block);
    ASSERT(config.set_num == 1 << config.num_of_bits_index);

    return config;
}

CacheConfig get_l3_cache_config() {
    CacheConfig config;
    config.size = 8 * 1024 * 1024;
    config.block_size = 64;
    config.associativity = 8;
    config.set_num = 16384;
    config.write_through = 0;
    config.write_allocate = 1;
    config.num_of_bits_block = 6;
    config.num_of_bits_index = 14;

    ASSERT(config.size = config.block_size * config.associativity * config.set_num);
    ASSERT(config.block_size == 1 << config.num_of_bits_block);
    ASSERT(config.set_num == 1 << config.num_of_bits_index);

    return config;
}

PrefetchConfig get_l2_prefetch_config() {
    PrefetchConfig config;
    config.prefetch = true;
    config.prefetch_num = 2;

    return config;
}

PrefetchConfig get_l3_prefetch_config() {
    PrefetchConfig config;
    config.prefetch = true;
    config.prefetch_num = 4;

    return config;
}

#endif // MULTI_LEVEL