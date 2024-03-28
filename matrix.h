#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

void entropy_seed();
void print_matrix(uint64_t first, uint64_t second, char (*matrix)[second]);
int64_t next_generation(uint64_t first, uint64_t second, char (*matrix)[second],
                        int64_t *p_neighbours);

#endif // MATRIX_H
