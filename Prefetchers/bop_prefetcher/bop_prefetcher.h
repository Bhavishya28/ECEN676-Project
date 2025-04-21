#ifndef BOP_PREFETCHER_H
#define BOP_PREFETCHER_H

#include <cstdint>

#include "address.h"
#include "champsim.h"
#include "modules.h"

struct bop_prefetcher : public champsim::modules::prefetcher {
  explicit bop_prefetcher(CACHE* cache) : prefetcher(cache) { }
  void l2c_prefetcher_initialize();
  uint32_t prefetcher_cache_operate(champsim::address addr, champsim::address ip, uint8_t cache_hit, bool useful_prefetch, access_type type, uint32_t metadata_in);
  uint32_t prefetcher_cache_fill(champsim::address addr, long set, long way, uint8_t prefetch, champsim::address evicted_addr, uint32_t metadata_in);
  void prefetcher_cycle_operate();
};

#endif