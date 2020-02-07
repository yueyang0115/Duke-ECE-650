#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Metadata * first_free_block = NULL;
Metadata * last_free_block = NULL;
Metadata * first_block = NULL;
size_t data_size = 0;
size_t free_size = 0;

void printFreeList() {
  Metadata * cur = first_free_block;
  while (cur != NULL) {
    printf("cur: %p, cur->size: %lu\n", cur, cur->size);
    cur = cur->next;
  }
}

void * ff_malloc(size_t size) {
  if (first_free_block != NULL) {
    Metadata * p = first_free_block;
    while (p != NULL) {
      if (p->size >= size) {
        return reuse_block(size, p);
      }
      p = p->next;
    }
  }
  return allocate_block(size);
}

void * reuse_block(size_t size, Metadata * p) {
  if (p->size > size + sizeof(Metadata)) {
    Metadata * splitted_block;
    splitted_block = (Metadata *)((char *)p + sizeof(Metadata) + size);
    splitted_block->size = p->size - size - sizeof(Metadata);
    splitted_block->isfree = 1;
    splitted_block->next = NULL;
    splitted_block->prev = NULL;

    remove_block(p);
    add_block(splitted_block);
    p->size = size;
    free_size -= (size + sizeof(Metadata));
  }
  else {
    remove_block(p);
    free_size -= (p->size + sizeof(Metadata));
  }
  p->isfree = 0;
  p->next = NULL;
  p->prev = NULL;
  return (char *)p + sizeof(Metadata);
}

void * allocate_block(size_t size) {
  data_size += size + sizeof(Metadata);
  Metadata * new_block = sbrk(size + sizeof(Metadata));
  if (first_block == NULL) {
    first_block = new_block;
  }
  new_block->size = size;
  new_block->isfree = 0;
  new_block->prev = NULL;
  new_block->next = NULL;
  return (char *)new_block + sizeof(Metadata);
}

void add_block(Metadata * p) {
  if ((first_free_block == NULL) || (p < first_free_block)) {
    p->prev = NULL;
    p->next = first_free_block;
    if (p->next != NULL) {
      p->next->prev = p;
    }
    else {
      last_free_block = p;
    }
    first_free_block = p;
  }
  else {
    Metadata * curr = first_free_block;
    while ((curr->next != NULL) && (p > curr->next)) {
      curr = curr->next;  //curr< p< curr->next, keep going
    }
    p->prev = curr;
    p->next = curr->next;
    curr->next = p;
    if (p->next != NULL) {
      p->next->prev = p;
    }
    else {
      last_free_block = p;
    }
  }
}

void remove_block(Metadata * p) {
  if ((last_free_block == first_free_block) && (last_free_block == p)) {
    last_free_block = first_free_block = NULL;
  }
  else if (last_free_block == p) {
    last_free_block = p->prev;
    last_free_block->next = NULL;
  }
  else if (first_free_block == p) {
    first_free_block = p->next;
    first_free_block->prev = NULL;
  }
  else {
    p->prev->next = p->next;
    p->next->prev = p->prev;
  }
}

void ff_free(void * ptr) {
  Metadata * p;
  p = (Metadata *)((char *)ptr - sizeof(Metadata));
  p->isfree = 1;
  free_size += p->size + sizeof(Metadata);
  add_block(p);

  if ((p->next != NULL) && ((char *)p + p->size + sizeof(Metadata) == (char *)p->next)) {
    p->size += sizeof(Metadata) + p->next->size;
    remove_block(p->next);
    //p->next->next = NULL;
    //p->next->prev = NULL;
  }
  if ((p->prev != NULL) &&
      ((char *)p->prev + p->prev->size + sizeof(Metadata) == (char *)p)) {
    p->prev->size += sizeof(Metadata) + p->size;
    remove_block(p);
    //p->next = NULL;
    //p->prev = NULL;
  }
}

void * bf_malloc(size_t size) {
  Metadata * p = first_free_block;
  Metadata * min_ptr = NULL;
  while (p != NULL) {
    if (p->size > size) {
      if ((min_ptr == NULL) || (p->size < min_ptr->size)) {
        min_ptr = p;
      }
    }
    if (p->size == size) {
      min_ptr = p;
      break;
    }
    p = p->next;
  }
  if (min_ptr != NULL) {
    return reuse_block(size, min_ptr);
  }
  else {
    return allocate_block(size);
  }
}

void bf_free(void * ptr) {
  return ff_free(ptr);
}

unsigned long get_data_segment_size() {
  return data_size;
}

unsigned long get_data_segment_free_space_size() {
  return free_size;
}
