#include "matrix.h"
// #include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#define PRESS_TO_GEN
#define RNG_GEN
// #define NUM_SEEDS 500
#define NUM_SEEDS 1000

int main(int argc, char **argv) {
  (void)argc;
  char user_input_colrows[256];
  uint64_t first, second;

  printf("Enter two integers separated by a space: ");

  if (fgets(user_input_colrows, sizeof(user_input_colrows), stdin)) {
    if (2 == sscanf(user_input_colrows, "%lu %lu", &first, &second)) {
      /* 'first' and 'second' can be safely used */
      if (first < 6 || second < 6) {
        printf("Matrix must be at least 6x6\n");
        return EXIT_SUCCESS;
      }

      // printf("You entered: %" PRIu64 " and %" PRIu64 "\n", first, second);
    } else {
      printf("Usage: %s x y\nx = horizontal, y = vertical\n", argv[0]);
      return EXIT_SUCCESS;
    }

  } else {
    printf("Error.\n");
  }
  fflush(stdin);

  // char matrix[first][second];
  char(*matrix)[second] = malloc(first * sizeof(*matrix));
  if (matrix == NULL) {
    perror("Failed to allocate memory for matrix");
    return EXIT_FAILURE;
  }

  entropy_seed();

  // Initialize the matrix with ' ' characters
  for (uint64_t i = 0; i < first; ++i) {
    for (uint64_t j = 0; j < second; ++j) {
      matrix[i][j] = ' ';
    }
  }

// Place a '*' at a random position
#ifdef RNG_GEN

  for (int i = 0; i < NUM_SEEDS; ++i) {
    matrix[rand() % first][rand() % second] = '*';
  }
#else
  // if rng is disabled then enter a pattern you want, beware becuase;
  // matrix[num][num] is in the format of matrix[y][x] becuase I skill issued
  matrix[10][10] = '*';
  matrix[11][9] = '*';
  matrix[11][10] = '*';
  matrix[11][11] = '*';

#endif /* ifdef RNG_GEN */
  // Print the matrix
  print_matrix(first, second, matrix);

// Finished printing first generation
//

// Second generation
#ifdef PRESS_TO_GEN

  fflush(stdin);
  while (1) {
    puts("Press ENTER for next generation, and q or ESC to quit");
    char ng_key = getchar();
    if (ng_key == 'q' || ng_key == 27) {
      puts("Simulation terminated");
      break;
    } else if (ng_key == '\n') {
      puts("Next generation:");
      int64_t neighbours = 0;
      int64_t *p_neighbour = &neighbours;
      next_generation(first, second, matrix, p_neighbour);
      print_matrix(first, second, matrix);
    }
  }
#else
  struct timespec delay;
  delay.tv_sec = 0;
  //  delay.tv_nsec = 250000000;
  delay.tv_nsec = 1000000;
  while (1) {

    puts("Next generation:");
    int64_t neighbours = 0;
    int64_t *p_neighbour = &neighbours;
    next_generation(first, second, matrix, p_neighbour);
    print_matrix(first, second, matrix);
    // Sleep using nanosleep
    nanosleep(&delay, NULL);
  }
#endif /* ifdef PRESS_TO_GEN */
  /*
  int64_t neighbours = 0;
  int64_t *p_neighbour = &neighbours;
  next_generation(first, second, matrix, p_neighbour);
  */
  // printf("Total neighbours: %" PRId64 "\n", neighbours);
  free(matrix);
  return EXIT_SUCCESS;
}
