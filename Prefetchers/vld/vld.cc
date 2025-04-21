#include "vld.h"
#include <algorithm>
#include <iostream>

uint64_t vld::get_hash(uint64_t key)
{
  // Robert Jenkins' 32 bit mix function
  key += (key << 12);
  key ^= (key >> 22);
  key += (key << 4);
  key ^= (key >> 9);
  key += (key << 10);
  key ^= (key >> 2);
  key += (key << 7);
  key ^= (key >> 12);

  // Knuth's multiplicative method
  key = (key >> 3) * 2654435761;

  return key;
}

void vld::prefetcher_initialize() {

    // Initialize statistics
    pf_issued = 0;
    pf_useful = 0;
    pf_useless = 0;
    access_count = 0;
    pattern_count = 0;
    // last_addr = 0;

    DHB._parent = this;
    P1._parent = this;
    P2._parent = this;
    P3._parent = this;

    if (ENABLE_DEBUG) {
        std::cout << "VLD Prefetcher initialized" << std::endl;
    }
}

uint32_t vld::prefetcher_cache_operate(champsim::address addr, champsim::address ip, 
                                     uint8_t cache_hit, bool useful_prefetch, 
                                     access_type type, uint32_t metadata_in) {


    champsim::page_number page{addr};
    uint32_t last_sig = 0, curr_sig = 0;
    offset_type::difference_type delta;

    DHB.read_and_update_DHB(addr,last_sig,curr_sig,delta);
    


    auto DHB_IDX = vld::get_hash(champsim::page_number{addr}.to<uint64_t>()) % DHB_ENTRIES;
    uint32_t delta1 = curr_sig & ((1 << 8) - 1);
    uint32_t delat2 = curr_sig & ((1 << 16) - 1);
    uint32_t delta3 = curr_sig & ((1 << 24) - 1);

    if(DHB.freq[DHB_IDX] > 1) {
        P1.update_pattern(last_sig, delta, 1);
    }

    if(DHB.freq[DHB_IDX] > 2) {
        P2.update_pattern(last_sig, delta, 2);

    }

    if(DHB.freq[DHB_IDX] > 3) {
        P3.update_pattern(last_sig, delta, 3);
    }
    

    if(cache_hit == 0) {
        bool ans;
        if(DHB.freq[DHB_IDX] > 3) {
            ans = P3.read_pattern(addr,curr_sig, 3);
        } 

        else if(!ans && DHB.freq[DHB_IDX] > 2) {
            ans = P2.read_pattern(addr, curr_sig, 2);
        }

        else if(!ans && DHB.freq[DHB_IDX] > 1){ 
            ans = P1.read_pattern(addr, curr_sig, 1);
        }

        // std::cout << "Cache Access at Address: " << addr << "\n"; 
        // std::cout << "Prefetch Opportunity" << "\n";

    }

    return metadata_in;
}

void vld::DHB_TABLE::read_and_update_DHB(champsim::address addr, uint32_t& last_sig, uint32_t& curr_sig, typename offset_type::difference_type& delta)
{
    auto DHB_IDX = vld::get_hash(champsim::page_number{addr}.to<uint64_t>()) % DHB_ENTRIES;
    tag_type partial_page{addr};
    offset_type page_offset{addr};
    long sig_delta = 0;

    last_sig = sig[DHB_IDX];
    delta = champsim::offset(DHB_TABLE::last_offset[DHB_IDX], page_offset);

    sig_delta = (delta < 0) ? (((-1) * delta) + (1 << (SIG_DELTA_BIT - 1))) : delta;
    curr_sig = ((last_sig << SIG_SHIFT) ^ sig_delta);
    sig[DHB_IDX] = curr_sig;
    last_offset[DHB_IDX] = page_offset;
    freq[DHB_IDX] = std::min(4, freq[DHB_IDX] + 1);

}


void vld::PATTERN_TABLE::update_pattern(uint32_t last_sig, offset_type::difference_type delta, uint8_t level) {

    if(level == 1) {
        last_sig = last_sig & ((1 << 8) - 1);
    }

    if(level == 2) {
        last_sig = last_sig & ((1 << 16) - 1);
    }

    if(level == 3) {
        last_sig = last_sig & ((1 << 24) - 1);
    }

    uint32_t way = PT_ENTRIES;
    for(way = 0; way < PT_ENTRIES; way++) {
        if(this->history[way] == last_sig) {
            this->pred[way] = delta;
            this->nMRU[way] = false;
            break;
        }
    }

    if(way == PT_ENTRIES) {
        for(way = 0; way < PT_ENTRIES; way++) {
            if(nMRU[way]) {
                this->pred[way] = delta;
                this->history[way] = last_sig;
                this->nMRU[way] = false;
                break;
            }
        }
    }

    if(way == PT_ENTRIES) {
        this->pred[0] = delta;
        this->history[0] = last_sig;
        this->nMRU[0] = false;
        for(way = 1; way < PT_ENTRIES; way++) {
            this->nMRU[way] = true;
        }
    }
}


bool vld::PATTERN_TABLE::read_pattern(champsim::address addr, uint32_t curr_sig, uint8_t history) {

    uint8_t MASK_BIT = 8;
    champsim::page_number page{addr};
    if(history == 1) {
        MASK_BIT = 8;
        curr_sig = curr_sig & ((1 << 8) - 1);        
    }

    if(history == 2) {
        MASK_BIT = 16;
        curr_sig = curr_sig & ((1 << 16) - 1);
    }

    if(history == 3) {
        MASK_BIT = 24;
        curr_sig = curr_sig & ((1 << 24) - 1);
    }


    uint8_t cnt = 0;
    uint32_t way = PT_ENTRIES;
    champsim::address prev_addr = addr;

    while(cnt < 16) {
        uint8_t prev_cnt = cnt;
        for(way = 0; way < PT_ENTRIES; way++) {
            if(curr_sig == this->history[way]) {
                champsim::address pf_addr{champsim::block_number{addr} + this->pred[way]};
                if(champsim::page_number{pf_addr} == page && pf_addr != prev_addr) {
                    _parent->prefetch_line(pf_addr, true, 0);
                    prev_addr = pf_addr;
                    cnt++;
                    // std::cout << "VLD: Issued prefetch for address 0x" << std::hex << pf_addr.to<uint64_t>() << " Signature before prefetch: " << curr_sig << " ";

                    curr_sig = ((curr_sig << 8) ^ this->pred[way]) & ((1 << (MASK_BIT - 1)) - 1);
                    // std::cout << "Signature after prefetch: " << curr_sig << "\n";
                    continue;

                }
            }
        }

        if(cnt == prev_cnt) {
            return false;
        }    
    }

    return (cnt > 0);
}


uint32_t vld::prefetcher_cache_fill(champsim::address addr, long set, long way, 
                               uint8_t prefetch, champsim::address evicted_addr, uint32_t metadata_in) {
    if (prefetch) {
        if (ENABLE_DEBUG) {
            std::cout << "VLD: Prefetch fill for address 0x" << std::hex 
                     << addr.to<uint64_t>() << std::dec << std::endl;
        }
        
    }

    return metadata_in;
}

void vld::prefetcher_cycle_operate() {
    // Optional: Implement any cycle-based operations
}

void vld::prefetcher_final_stats() {
    std::cout << "VLD Prefetcher Stats:" << std::endl;
    std::cout << "Total accesses: " << access_count << std::endl;
    std::cout << "Prefetches issued: " << pf_issued << std::endl;
    std::cout << "Useful prefetches: " << pf_useful << std::endl;
    std::cout << "Accuracy: " << (pf_issued ? (100.0 * pf_useful / pf_issued) : 0) << "%" << std::endl;
    std::cout << "Patterns detected: " << pattern_count << std::endl;
}
