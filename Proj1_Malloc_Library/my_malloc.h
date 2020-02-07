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
void printFreeList();
void * reuse_block(size_t size, Metadata * p);
void * allocate_block(size_t size);
void add_block(Metadata * p);
void remove_block(Metadata * p);
void * ff_malloc(size_t size);
void ff_free(void * ptr);
void * bf_malloc(size_t size);
void bf_free(void * ptr);
unsigned long get_data_segment_size();             //in bytes
unsigned long get_data_segment_free_space_size();  //in bytes
