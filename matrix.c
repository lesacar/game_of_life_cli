#include "matrix.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void entropy_seed() {
  int urandom = open("/dev/urandom", O_RDONLY);
  if (urandom == -1) {
    perror("Failed to open /dev/urandom");
    exit(EXIT_FAILURE);
  }

  unsigned int seed;
  ssize_t result = read(urandom, &seed, sizeof(seed));
  if (result != sizeof(seed)) {
    perror("Failed to read from /dev/urandom");
    close(urandom);
    exit(EXIT_FAILURE);
  }

  close(urandom);
  srand(seed);
}

void print_matrix(uint64_t first, uint64_t second, char (*matrix)[second]) {
  // Print the top border
  printf("/");
  for (uint64_t j = 0; j < second * 2 + 1; ++j) {
    printf("-");
  }
  printf("\\\n");

  // Print the matrix with left and right borders
  for (uint64_t i = 0; i < first; ++i) {
    printf("| ");
    for (uint64_t j = 0; j < second; ++j) {
      printf("%c ", matrix[i][j]);
    }
    printf("|\n");
  }

  // Print the bottom border
  printf("\\");
  for (uint64_t j = 0; j < second * 2 + 1; ++j) {
    printf("-");
  }
  printf("/\n");
}

//
int64_t next_generation(uint64_t first, uint64_t second, char (*matrix)[second],
                        int64_t *p_neighbours) {
  int offset[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                      {0, 1},   {1, -1}, {1, 0},  {1, 1}};
  int64_t x = first;
  int64_t y = second;
  int64_t tempX = 0;
  int64_t tempY = 0;
  char newMatrix[x][y]; // Create a new matrix to store the next generation

  for (int i = 0; i < x; ++i) {
    for (int j = 0; j < y; ++j) {
      int64_t neighbours = 0; // Move this line inside the loop

      for (int k = 0; k < 8; ++k) {
        tempX = i + offset[k][0];
        tempY = j + offset[k][1];

        if (tempX >= 0 && tempX < x && tempY >= 0 && tempY < y) {
          if (matrix[tempX][tempY] == '*') {
            neighbours++;
          }
        }
      }

      // Update newMatrix based on Conway's Game of Life rules
      if (neighbours < 2 || neighbours > 3) {
        newMatrix[i][j] = ' ';
      } else if (neighbours == 3 && matrix[i][j] == ' ') {
        newMatrix[i][j] = '*';
      } else {
        newMatrix[i][j] = matrix[i][j]; // Preserve unchanged cells
      }
      *p_neighbours = neighbours;
    }
  }

  // Copy the newMatrix back to the original matrix
  for (int i = 0; i < x; ++i) {
    for (int j = 0; j < y; ++j) {
      matrix[i][j] = newMatrix[i][j];
    }
  }

  return 0; // Success
}
