#include "cache.h"
#include <cmath>

simulator::simulator( uint8_t c_size, uint8_t assoc, uint8_t b_size, char type, char method ) {
    this->cache_size_exp = c_size;
    this->cache_size = (uint32_t) pow( 2, cache_size_exp );
    this->assoc_exp = assoc;
    this->associativity = (uint8_t) pow( 2, assoc_exp );
    this->block_size_exp = b_size;
    this->word_size = (uint8_t) pow( 2, block_size_exp - 2 );
    this->idx_num = (uint16_t) pow( 2, cache_size_exp - assoc_exp - block_size_exp );
    
    this->tab = new cache *[idx_num];
    for ( int i = 0; i < idx_num; i++ )
        this->tab[i] = new cache [associativity];
    
    for (int i = 0; i < idx_num; i++ ) {
        for ( int j = 0; j < associativity; j++ ) {
            tab[i][j].data = new uint32_t [word_size];
            for ( int k = 0; k < word_size; k++ )
                tab[i][j].data[k] = 0;
        }
    }
    
    this->type = type;
    this->method = method;
    
    this->access = 0;
    this->miss = 0;
    this->mem_write = 0;
}

simulator::~simulator() {
    for ( int i = 0; i < idx_num; i++ )
        for ( int j = 0; j < (int) pow( 2, assoc_exp ); j++ )
            delete[] tab[i][j].data;
    
    for ( int i = 0; i < idx_num; i++ )
        delete[] tab[i];
    
    delete[] tab;
}

void simulator::reset( uint8_t c_size, uint8_t assoc, uint8_t b_size, char type, char method ) {
    this->~simulator();
    
    this->cache_size_exp = c_size;
    this->cache_size = (uint32_t) pow( 2, cache_size_exp );
    this->assoc_exp = assoc;
    this->associativity = (uint8_t) pow( 2, assoc_exp );
    this->block_size_exp = b_size;
    this->word_size = (uint8_t) pow( 2, block_size_exp - 2 );
    this->idx_num = (uint16_t) pow( 2, cache_size_exp - assoc_exp - block_size_exp );
    
    this->tab = new cache *[idx_num];
    for ( int i = 0; i < idx_num; i++ )
        this->tab[i] = new cache [associativity];
    
    for (int i = 0; i < idx_num; i++ ) {
        for ( int j = 0; j < associativity; j++ ) {
            tab[i][j].data = new uint32_t [word_size];
            for ( int k = 0; k < word_size; k++ )
                tab[i][j].data[k] = 0;
        }
    }
    
    this->type = type;
    this->method = method;
    
    this->access = 0;
    this->miss = 0;
    this->mem_write = 0;
}