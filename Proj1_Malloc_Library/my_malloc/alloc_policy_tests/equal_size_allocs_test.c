#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "my_malloc.h"

#define NUM_ITERS 1
#define NUM_ITEMS 6
#define ALLOC_SIZE 128

#ifdef FF
#define MALLOC(sz) ff_malloc(sz)
#define FREE(p) ff_free(p)
#endif
#ifdef BF
#define MALLOC(sz) bf_malloc(sz)
#define FREE(p) bf_free(p)
#endif

int main(int argc, char * argv[]) {
  int i, j;
  int * array[NUM_ITEMS];
  int * spacing_array[NUM_ITEMS];

  for (i = 0; i < NUM_ITEMS; i++) {
    array[i] = (int *)MALLOC(ALLOC_SIZE);

  }  //for i

  for (i = 0; i < NUM_ITEMS; i++) {
    FREE(array[NUM_ITEMS - 1 - i]);
    //printFreeList();
  }  //for i

  //printFreeList();
  /*block_t *cur = head_block;
  while (cur != NULL) {
    printf("cur: %p, cur->size: %lu", cur, cur->size
           );
    cur = cur->next_list;
  }*/
  return 0;
}
