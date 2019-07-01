#include <iostream>
#include <cstdio>
#include "cache.h"

using namespace std;

int main( int argc, char** argv ) {
    
    FILE *fp;
    uint8_t type;
    uint32_t address;
    
    double d_miss_16[4][5];
    double i_miss_16[4][5];
    double l2_miss_16[4][5];
    unsigned mem_writes_in_16[4][5];
    unsigned mem_writes_ex_16[4][5];
    double d_miss_64[4][5];
    double i_miss_64[4][5];
    double l2_miss_64[4][5];
    unsigned mem_writes_in_64[4][5];
    unsigned mem_writes_ex_64[4][5];
    
    printf( "Cache Miss Ratio (block size = 16B)\n" );
    printf( "LRU/16\t%d\t%d\t%d\t%d\t%d\n", 1024, 2048, 4096, 8192, 16384 );
    
    /***********************************************************************
     * Direct-mapped, 16 bytes block size, inclusive ***********************
     ***********************************************************************/
    
    printf( "Direct\t" );
    
    // 1KiB cache***********************************************************
    simulator D_16B( 10, 0, 4, 'd', 'i' );
    simulator I_16B( 10, 0, 4, 'i', 'i' );
    simulator L2_16B( 14, 3, 4, 'u', 'i' );
    
    for ( int s = 0; s < 5; s++ ) {
        
        fp = fopen( argv[1], "r" );
        
        uint32_t and_idx;
        uint32_t and_tag;
        
        and_idx = 1;
        for ( int i = 1; i < 6 + s; i++ ) {
            and_idx <<= 1;
            and_idx++;
        }
        and_idx <<= 4;
        and_tag = (1 << 31) >> (21 - s);
        
        
        while ( fscanf( fp, "%hhu %xu", &type, &address ) != EOF ) {
            
            uint32_t L1_idx = ((address & and_idx) >> 4) % D_16B.idx_num;
            uint32_t L1_tag = (address & and_tag) >> (10 + s);
            uint32_t L2_idx = ((address & 0x7f0) >> 4) % L2_16B.idx_num;
            uint32_t L2_tag = (address & 0xfffff800) >> 11;
            
            // Read data
            if ( type == 0 || type == 1) {
                D_16B.access++;
                
                // first access = L1 miss and L2 miss
                if ( !D_16B.tab[L1_idx][0].valid ) {
                    
                    // L1 performance
                    D_16B.miss++;
                    D_16B.tab[L1_idx][0].valid = true;
                    D_16B.tab[L1_idx][0].tag = L1_tag;
                    if ( type == 1 )
                        D_16B.tab[L1_idx][0].dirty = true;
                    
                    // L2 performance
                    L2_16B.access++;
                    L2_16B.miss++;
                    cout << L2_idx << ":";
                    
                    // find empty space
                    for ( int i = 0; i < 8; i++ ) {
                        
                        // found
                        if ( !L2_16B.tab[L2_idx][i].valid ) {
                            L2_16B.tab[L2_idx][i].valid = true;
                            L2_16B.tab[L2_idx][i].tag = L2_tag;
                            
                            for ( int j = 0; j < 8; j++ ) {
                                if ( j == i )
                                    continue;
                                L2_16B.tab[L2_idx][j].LRU++;
                            }
                            L2_16B.tab[L2_idx][i].LRU = 0;
                            
                            break;
                        }
                        
                        // there is no empty space -> memory write happens
                        if ( i == 7 ) {
                            int LRU = 0, tmp;
                            
                            // Find LRU
                            for ( int j = 0; j < 8; j++ ) {
                                if ( L2_16B.tab[L2_idx][j].LRU > LRU ) {
                                    LRU = L2_16B.tab[L2_idx][j].LRU;
                                    tmp = j;
                                }
                            }
                            
                            // Update LRU bit
                            for ( int j = 0; j < 8; j++ ) {
                                if ( j == tmp )
                                    continue;
                                L2_16B.tab[L2_idx][j].LRU++;
                            }
                            L2_16B.tab[L2_idx][tmp].LRU = 0;
                            
                            if ( type == 1 && L2_16B.tab[L2_idx][tmp].dirty )
                                L2_16B.mem_write++;
                            
                            L2_16B.tab[L2_idx][tmp].valid = true;
                            L2_16B.tab[L2_idx][tmp].tag = L2_tag;
                            L2_16B.tab[L2_idx][tmp].dirty = false;
                        }
                    }
                }
                    
                    // same index, different tag
                else if ( D_16B.tab[L1_idx][0].tag != L1_tag ) {
                    
                    // L1 performance
                    D_16B.miss++;
                    D_16B.tab[L1_idx][0].tag = L1_tag;
                    if ( D_16B.tab[L1_idx][0].dirty )
                        D_16B.mem_write++;
                    
                    // L2 performance
                    L2_16B.access++;
                    cout << L2_idx << ":";
                    bool L2_miss = false;
                    for ( int i = 0; i < 8; i++ ) {
                        // L2 hit
                        if ( L2_16B.tab[L2_idx][i].tag == L2_tag ) {
                            if ( D_16B.tab[L1_idx][0].dirty )
                                L2_16B.tab[L2_idx][i].dirty = true;
                            cout << " hit" << endl;
                            break;
                        }
                        
                        // L2 miss
                        if ( i == 7 ) {
                            L2_16B.miss++;
                            L2_miss = true;
                            cout << " miss" << endl;
                        }
                    }
                    
                    if ( L2_miss ) {
                        for ( int i = 0; i < 8; i++ ) {
                            if ( !L2_16B.tab[L2_idx][i].valid ) {
                                L2_16B.tab[L2_idx][i].valid = true;
                                L2_16B.tab[L2_idx][i].tag = L2_tag;
                                
                                for ( int j = 0; j < 8; j++ ) {
                                    if ( j == i )
                                        continue;
                                    L2_16B.tab[L2_idx][j].LRU++;
                                }
                                L2_16B.tab[L2_idx][i].LRU = 0;
                                
                                break;
                            }
                            
                            // there is no empty space -> memory write happens
                            if ( i == 7 ) {
                                int LRU = 0, tmp;
                                
                                // Find LRU
                                for ( int j = 0; j < 8; j++ ) {
                                    if ( L2_16B.tab[L2_idx][j].LRU > LRU ) {
                                        LRU = L2_16B.tab[L2_idx][j].LRU;
                                        tmp = j;
                                    }
                                }
                                
                                // Update LRU bit
                                for ( int j = 0; j < 8; j++ ) {
                                    if ( j == tmp )
                                        continue;
                                    L2_16B.tab[L2_idx][j].LRU++;
                                }
                                L2_16B.tab[L2_idx][tmp].LRU = 0;
                                
                                if ( L2_16B.tab[L2_idx][tmp].dirty )
                                    L2_16B.mem_write++;
                                
                                L2_16B.tab[L2_idx][tmp].valid = true;
                                L2_16B.tab[L2_idx][tmp].tag = L2_tag;
                                L2_16B.tab[L2_idx][tmp].dirty = false;
                                
                            }
                        }
                    }
                    if ( type == 1 )
                        D_16B.tab[L1_idx][0].dirty = true;
                    
                }
                
                else if ( type == 1 && D_16B.tab[L1_idx][0].tag == L1_tag ) {
                    D_16B.tab[L1_idx][0].dirty = true;
                }
                
                // Instruction fetch
            } else {
                I_16B.access++;
                
                // first access = L1 miss and L2 miss
                if ( !I_16B.tab[L1_idx][0].valid ) {
                    
                    // L1 performance
                    I_16B.miss++;
                    I_16B.tab[L1_idx][0].valid = true;
                    I_16B.tab[L1_idx][0].tag = L1_tag;
                    
                    // L2 performance
                    L2_16B.access++;
                    L2_16B.miss++;
                    cout << L2_idx << ":";
                    
                    // find empty space
                    for ( int i = 0; i < 8; i++ ) {
                        
                        // found
                        if ( !L2_16B.tab[L2_idx][i].valid ) {
                            L2_16B.tab[L2_idx][i].valid = true;
                            L2_16B.tab[L2_idx][i].tag = L2_tag;
                            
                            for ( int j = 0; j < 8; j++ ) {
                                if ( j == i )
                                    continue;
                                L2_16B.tab[L2_idx][j].LRU++;
                            }
                            L2_16B.tab[L2_idx][i].LRU = 0;
                            
                            break;
                        }
                        
                        // if there is no empty space
                        if ( i == 7 ) {
                            int LRU = 0, tmp;
                            
                            // Find LRU
                            for ( int j = 0; j < 8; j++ ) {
                                if ( L2_16B.tab[L2_idx][j].LRU > LRU ) {
                                    LRU = L2_16B.tab[L2_idx][j].LRU;
                                    tmp = j;
                                }
                            }
                            
                            // Update LRU bit
                            for ( int j = 0; j < 8; j++ ) {
                                if ( j == tmp )
                                    continue;
                                L2_16B.tab[L2_idx][j].LRU++;
                            }
                            L2_16B.tab[L2_idx][tmp].LRU = 0;
                            
                            L2_16B.tab[L2_idx][tmp].valid = true;
                            L2_16B.tab[L2_idx][tmp].dirty = false;
                            L2_16B.tab[L2_idx][tmp].tag = L2_tag;
                        }
                    }
                }
                    
                    // same index, different tag
                else if ( I_16B.tab[L1_idx][0].tag != L1_tag ) {
                    
                    // L1 performance
                    I_16B.miss++;
                    I_16B.tab[L1_idx][0].tag = L1_tag;
                    
                    // L2 performance
                    L2_16B.access++;
                    cout << L2_idx << ":";
                    bool L2_miss = false;
                    for ( int i = 0; i < 8; i++ ) {
                        // L2 hit
                        if ( L2_16B.tab[L2_idx][i].tag == L2_tag ) {
                            cout << " hit" << endl;
                            break;
                        }
                        
                        // L2 miss
                        if ( i == 7 ) {
                            L2_16B.miss++;
                            L2_miss = true;
                            cout << " miss" << endl;
                        }
                    }
                    
                    if ( L2_miss ) {
                        
                        // find empty space
                        for ( int i = 0; i < 8; i++ ) {
                            
                            // found
                            if ( !L2_16B.tab[L2_idx][i].valid ) {
                                L2_16B.tab[L2_idx][i].valid = true;
                                L2_16B.tab[L2_idx][i].tag = L2_tag;
                                
                                for ( int j = 0; j < 8; j++ ) {
                                    if ( j == i )
                                        continue;
                                    L2_16B.tab[L2_idx][j].LRU++;
                                }
                                L2_16B.tab[L2_idx][i].LRU = 0;
                                
                                break;
                            }
                            
                            // there is no empty space -> memory write happens
                            if ( i == 7 ) {
                                int LRU = 0, tmp;
                                
                                // Find LRU
                                for ( int j = 0; j < 8; j++ ) {
                                    if ( L2_16B.tab[L2_idx][j].LRU > LRU ) {
                                        LRU = L2_16B.tab[L2_idx][j].LRU;
                                        tmp = j;
                                    }
                                }
                                
                                // Update LRU bit
                                for ( int j = 0; j < 8; j++ ) {
                                    if ( j == tmp )
                                        continue;
                                    L2_16B.tab[L2_idx][j].LRU++;
                                }
                                L2_16B.tab[L2_idx][tmp].LRU = 0;
                                
                                L2_16B.tab[L2_idx][tmp].valid = true;
                                L2_16B.tab[L2_idx][tmp].tag = L2_tag;
                                L2_16B.tab[L2_idx][tmp].dirty = false;
                                
                            }
                        }
                    }
                }
            }
        }
        
        fclose ( fp );
        d_miss_16[0][s] =  ((double) D_16B.miss / (double) D_16B.access);
        i_miss_16[0][s] = ((double) I_16B.miss / (double) I_16B.access);
        l2_miss_16[0][s] = ((double) L2_16B.miss / (double) L2_16B.access);
        mem_writes_in_16[0][s] = D_16B.mem_write;
        
        D_16B.reset( 11 + s, 0, 4, 'd', 'i' );
        I_16B.reset( 11 + s, 0, 4, 'i', 'i' );
        L2_16B.reset( 14, 3, 4, 'u', 'i' );
        
    }
    for ( int i = 0; i < 5; i++ )
        printf( "%.2f\t", d_miss_16[0][i] );
    printf( "\ni_miss: %.2f\n", i_miss_16[0][0] );
    printf( "l2_miss: %.2f\n", l2_miss_16[0][0] );
    cout << "access: " << D_16B.access << endl;
    cout << "miss: " << D_16B.miss << endl;
    cout << mem_writes_in_16[0][0] << endl;
    /*
    // 2KiB cache***********************************************************
    D_16B.reset( 11, 0, 4, 'd', 'i' );
    I_16B.reset( 11, 0, 4, 'd', 'i' );
    L2_16B.reset( 14, 3, 4, 'u', 'i' );
    
    d_miss_16[0][1] = (double) (D_16B.miss / D_16B.access);
    i_miss_16[0][1] = (double) (I_16B.miss / I_16B.access);
    l2_miss_16[0][1] = (double) (L2_16B.miss / L2_16B.access);
    mem_writes_in_16[0][1] = D_16B.mem_write;
    
    // 4KiB cache***********************************************************
    D_16B.reset( 12, 0, 4, 'd', 'i' );
    I_16B.reset( 12, 0, 4, 'd', 'i' );
    L2_16B.reset( 14, 3, 4, 'u', 'i' );
    
    d_miss_16[0][2] = (double) (D_16B.miss / D_16B.access);
    i_miss_16[0][2] = (double) (I_16B.miss / I_16B.access);
    l2_miss_16[0][2] = (double) (L2_16B.miss / L2_16B.access);
    mem_writes_in_16[0][2] = D_16B.mem_write;
    
    // 8KiB cache***********************************************************
    D_16B.reset( 13, 0, 4, 'd', 'i' );
    I_16B.reset( 13, 0, 4, 'd', 'i' );
    L2_16B.reset( 14, 3, 4, 'u', 'u' );
    
    d_miss_16[0][3] = (double) (D_16B.miss / D_16B.access);
    i_miss_16[0][3] = (double) (I_16B.miss / I_16B.access);
    l2_miss_16[0][3] = (double) (L2_16B.miss / L2_16B.access);
    mem_writes_in_16[0][3] = D_16B.mem_write;
    
    // 16KiB cache**********************************************************
    D_16B.reset( 14, 0, 4, 'd', 'i' );
    I_16B.reset( 14, 0, 4, 'd', 'i' );
    L2_16B.reset( 14, 3, 4, 'u', 'i' );
    
    d_miss_16[0][4] = (double) (D_16B.miss / D_16B.access);
    i_miss_16[0][4] = (double) (I_16B.miss / I_16B.access);
    l2_miss_16[0][4] = (double) (L2_16B.miss / L2_16B.access);
    mem_writes_in_16[0][4] = D_16B.mem_write;
    */
    return 0;
}