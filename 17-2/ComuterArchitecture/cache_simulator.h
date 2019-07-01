#ifndef __SIMULATOR_H_
#define __SIMULATOR_H__

#include <iostream>
#include <cstdint>

void inSim4words( char** argv, uint32_t c_size, uint8_t associativity );
void exSim4words( char** argv, uint32_t c_size, uint8_t associativity );
void inSim16words( char** argv, uint32_t c_size, uint8_t associativity );
void exSim16words( char** argv, uint32_t c_size, uint8_t associativity );

#endif