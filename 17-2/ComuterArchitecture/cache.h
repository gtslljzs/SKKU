#ifndef __CACHE_H__
#define __CACHE_H__
#include <cstdint>

struct cache {
    bool valid = false;
    bool dirty = false;
    uint8_t LRU = 0;
    uint32_t tag = 0;
    uint32_t* data;
};

class simulator {
public:
    uint8_t cache_size_exp;
    uint32_t cache_size;
    uint8_t assoc_exp;
    uint8_t associativity;
    uint8_t block_size_exp;
    uint8_t word_size;
    uint16_t idx_num;
    cache **tab;
    
    char type;
    char method;
    
    uint32_t access;
    uint32_t miss;
    uint32_t mem_write;

public:
    simulator( uint8_t c_size, uint8_t assoc, uint8_t b_size, char type, char method );
    ~simulator();
    
    void reset( uint8_t c_size, uint8_t assoc, uint8_t b_size, char type, char method );
};

#endif