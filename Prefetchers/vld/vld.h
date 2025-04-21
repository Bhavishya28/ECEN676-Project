#ifndef VLD_H
#define VLD_H

#include <cstdint>
#include <vector>

#include "cache.h"
#include "modules.h"
#include "msl/lru_table.h"


struct vld : public champsim::modules::prefetcher {
    // Configuration parameters
    // Cache Size - 8MB
    // Page Size - 8KB
    // Cache Block - 64B
    static constexpr bool ENABLE_DEBUG = false;
    static constexpr uint32_t MAX_DEGREE = 16;  // Maximum number of prefetches per access
    static constexpr uint32_t HISTORY_LENGTH = 64;  // Length of delta history
    static constexpr uint32_t MIN_PATTERN_LENGTH = 2;  // Minimum pattern length to consider
    static constexpr uint32_t MAX_PATTERN_LENGTH = 4;  // Maximum pattern length to consider
    static constexpr uint32_t CONFIDENCE_THRESHOLD = 3; // Minimum occurrences for a pattern
    static constexpr uint32_t DHB_ENTRIES = 1024;
    static constexpr uint32_t DHB_SHIFT_BITS = 8;
    constexpr static unsigned SIG_BIT = 32;
    constexpr static unsigned SIG_SHIFT = 8;
    constexpr static unsigned SIG_DELTA_BIT = 8;
    // constexpr static uint64_t SIG_MASK = ((1 << SIG_BIT) - 1);
    constexpr static unsigned DHB_TAG_BIT = 16;
    constexpr static unsigned DHB_FREQ_BIT = 8;
     constexpr static unsigned PT_ENTRIES = 256;

      using prefetcher::prefetcher;
      uint32_t prefetcher_cache_operate(champsim::address addr, champsim::address ip, uint8_t cache_hit, bool useful_prefetch, access_type type,
                                        uint32_t metadata_in);
      uint32_t prefetcher_cache_fill(champsim::address addr, long set, long way, uint8_t prefetch, champsim::address evicted_addr, uint32_t metadata_in);

      void prefetcher_initialize();
      void prefetcher_cycle_operate();
      void prefetcher_final_stats();

      struct block_in_page_extent : champsim::dynamic_extent {
        block_in_page_extent() : dynamic_extent(champsim::data::bits{LOG2_PAGE_SIZE}, champsim::data::bits{LOG2_BLOCK_SIZE}) {}
      };
    using offset_type = champsim::address_slice<block_in_page_extent>;

    struct tag_extent : champsim::dynamic_extent {
      tag_extent() : dynamic_extent(champsim::data::bits{DHB_TAG_BIT + LOG2_PAGE_SIZE}, champsim::data::bits{LOG2_PAGE_SIZE}) {}
    };
    using tag_type = champsim::address_slice<tag_extent>;

class DHB_TABLE
  {
  public:
    vld* _parent;

    tag_type tag[DHB_ENTRIES];
    offset_type last_offset[DHB_ENTRIES];
    uint32_t sig[DHB_ENTRIES];
    uint8_t DPT_IDX[DHB_ENTRIES];
    uint8_t freq[DHB_ENTRIES];

    DHB_TABLE()
    {

        for (uint32_t way = 0; way < DHB_ENTRIES; way++) {
          tag[way] = tag_type{};
          last_offset[way] = offset_type{};
          sig[way] = 0;
          DPT_IDX[way] = 0;
          freq[way] = 0;
        }
    };

    void read_and_update_DHB(champsim::address addr, uint32_t& last_sig, uint32_t& curr_sig, typename offset_type::difference_type& delta);
};

  class PATTERN_TABLE
  {
  public:
    vld* _parent;
    uint32_t history[PT_ENTRIES];
    uint8_t pred[PT_ENTRIES];
    bool valid[PT_ENTRIES];
    bool nMRU[PT_ENTRIES];
    uint8_t accuracy[PT_ENTRIES];


    PATTERN_TABLE()
    {
        for (uint32_t way = 0; way < PT_ENTRIES; way++) {
          history[way] = 0;
          nMRU[way] = true;
          pred[way] = 0;
          valid[way] = false;
          accuracy[way] = 0;
        }
    }


    void update_pattern(uint32_t last_sig, typename offset_type::difference_type curr_delta, uint8_t level);
    bool read_pattern(champsim::address addr, uint32_t curr_sig, uint8_t history);
  };



    // Statistics
    uint64_t pf_issued;
    uint64_t pf_useful;
    uint64_t pf_useless;
    uint64_t access_count;
    uint64_t pattern_count;

    // Last accessed address for delta calculation
    champsim::address last_addr;

    // Helper functions
    static uint64_t get_hash(uint64_t key);

    DHB_TABLE DHB;
    PATTERN_TABLE P1;
    PATTERN_TABLE P2;
    PATTERN_TABLE P3;

};

#endif