#include "bop_prefetcher.h"
#include "cache.h"
#include <map>
#include <iostream>

using namespace std;

namespace {
  constexpr int OFFSET_COUNT = 34;
  constexpr int ROUNDMAX = 100;
  constexpr int SCOREMAX = 31;
  int offset_list[OFFSET_COUNT] = {1,2,3,4,5,6,8,9,10,12,15,16,18,20,24,25,27,30,32,36,40,45,48,50,54,60,64,72,75,80,81,90,96,100};
}

map<int, int> offset_score_table;
int current_offset = 1;
int round_count = 0;

void bop_prefetcher::l2c_prefetcher_initialize() {
  for (int i = 0; i < OFFSET_COUNT; i++) {
    offset_score_table[offset_list[i]] = 0;
  }
}

uint32_t bop_prefetcher::prefetcher_cache_operate(champsim::address addr, champsim::address ip, uint8_t cache_hit, bool useful_prefetch, access_type type, uint32_t metadata_in) {
  if (round_count >= ROUNDMAX) {
    int max_score = 0;
    for (auto& e : offset_score_table) {
      if (e.second > max_score) {
        max_score = e.second;
        current_offset = e.first;
      }
      e.second = 0;
    }
    round_count = 0;
  }

  for (int i = 0; i < OFFSET_COUNT; i++) {
    champsim::address test_addr{addr - offset_list[i]};
    if (cache_hit) offset_score_table[offset_list[i]]++;
  }

  round_count++;

  champsim::address pf_addr{addr + current_offset};
  const bool mshr_under_light_load = intern_->get_mshr_occupancy_ratio() < 0.5;
  prefetch_line(pf_addr, mshr_under_light_load, 0);

  return metadata_in;
}

void bop_prefetcher::prefetcher_cycle_operate() {
}

uint32_t bop_prefetcher::prefetcher_cache_fill(champsim::address addr, long set, long way, uint8_t prefetch, champsim::address evicted_addr, uint32_t metadata_in) {
  return metadata_in;
}