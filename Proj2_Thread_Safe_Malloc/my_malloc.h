#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct metadata {
  size_t size;
  int isfree;
  // size_t pre_size;
  struct metadata * next;
  struct metadata * prev;
};
typedef struct metadata Metadata;
//void printFreeList();
void * reuse_block(size_t size,
                   Metadata * p,
                   Metadata ** first_free_block,
                   Metadata ** last_free_block);
void * allocate_block(size_t size, int sbrk_lock);
void add_block(Metadata * p, Metadata ** first_free_block, Metadata ** last_free_block);
void remove_block(Metadata * p,
                  Metadata ** first_free_block,
                  Metadata ** last_free_block);
void * bf_malloc(size_t size,
                 Metadata ** first_free_block,
                 Metadata ** last_free_block,
                 int sbrk_lock);
void bf_free(void * ptr, Metadata ** first_free_block, Metadata ** last_free_block);

void * ts_malloc_lock(size_t size);
void ts_free_lock(void * ptr);
void * ts_malloc_nolock(size_t size);
void ts_free_nolock(void * ptr);
