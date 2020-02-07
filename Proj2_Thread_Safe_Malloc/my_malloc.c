#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Metadata * first_free_lock = NULL;
Metadata * last_free_lock = NULL;
//Metadata * first_block = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
__thread Metadata * first_free_nolock = NULL;
__thread Metadata * last_free_nolock = NULL;

/*
void printFreeList() {
  Metadata * cur = first_free_block;
  while (cur != NULL) {
    printf("cur: %p, cur->size: %lu\n", cur, cur->size);
    cur = cur->next;
  }
  }*/

void * ts_malloc_lock(size_t size) {
  pthread_mutex_lock(&lock);
  int sbrk_lock = 0;
  void * p = bf_malloc(size, &first_free_lock, &last_free_lock, sbrk_lock);
  pthread_mutex_unlock(&lock);
  return p;
}
void ts_free_lock(void * ptr) {
  pthread_mutex_lock(&lock);
  bf_free(ptr, &first_free_lock, &last_free_lock);
  pthread_mutex_unlock(&lock);
}

void * ts_malloc_nolock(size_t size) {
  int sbrk_lock = 1;
  void * p = bf_malloc(size, &first_free_nolock, &last_free_nolock, sbrk_lock);
  return p;
}
void ts_free_nolock(void * ptr) {
  bf_free(ptr, &first_free_nolock, &last_free_nolock);
}

void * reuse_block(size_t size,
                   Metadata * p,
                   Metadata ** first_free_block,
                   Metadata ** last_free_block) {
  if (p->size > size + sizeof(Metadata)) {
    Metadata * splitted_block;
    splitted_block = (Metadata *)((char *)p + sizeof(Metadata) + size);
    splitted_block->size = p->size - size - sizeof(Metadata);
    splitted_block->isfree = 1;
    splitted_block->next = NULL;
    splitted_block->prev = NULL;

    remove_block(p, first_free_block, last_free_block);
    add_block(splitted_block, first_free_block, last_free_block);
    p->size = size;
  }
  else {
    remove_block(p, first_free_block, last_free_block);
  }
  p->isfree = 0;
  p->next = NULL;
  p->prev = NULL;
  return (char *)p + sizeof(Metadata);
}

void * allocate_block(size_t size, int sbrk_lock) {
  Metadata * new_block = NULL;
  if (sbrk_lock == 0) {
    new_block = sbrk(size + sizeof(Metadata));
  }
  else {
    pthread_mutex_lock(&lock);
    new_block = sbrk(size + sizeof(Metadata));
    pthread_mutex_unlock(&lock);
  }
  new_block->size = size;
  new_block->isfree = 0;
  new_block->prev = NULL;
  new_block->next = NULL;
  return (char *)new_block + sizeof(Metadata);
}

void add_block(Metadata * p, Metadata ** first_free_block, Metadata ** last_free_block) {
  if ((*first_free_block == NULL) || (p < *first_free_block)) {
    p->prev = NULL;
    p->next = *first_free_block;
    if (p->next != NULL) {
      p->next->prev = p;
    }
    else {
      *last_free_block = p;
    }
    *first_free_block = p;
  }
  else {
    Metadata * curr = *first_free_block;
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
      *last_free_block = p;
    }
  }
}

void remove_block(Metadata * p,
                  Metadata ** first_free_block,
                  Metadata ** last_free_block) {
  if ((*last_free_block == *first_free_block) && (*last_free_block == p)) {
    *last_free_block = *first_free_block = NULL;
  }
  else if (*last_free_block == p) {
    *last_free_block = p->prev;
    (*last_free_block)->next = NULL;
  }
  else if (*first_free_block == p) {
    *first_free_block = p->next;
    (*first_free_block)->prev = NULL;
  }
  else {
    p->prev->next = p->next;
    p->next->prev = p->prev;
  }
}

void bf_free(void * ptr, Metadata ** first_free_block, Metadata ** last_free_block) {
  Metadata * p;
  p = (Metadata *)((char *)ptr - sizeof(Metadata));
  p->isfree = 1;

  add_block(p, first_free_block, last_free_block);

  if ((p->next != NULL) && ((char *)p + p->size + sizeof(Metadata) == (char *)p->next)) {
    p->size += sizeof(Metadata) + p->next->size;
    remove_block(p->next, first_free_block, last_free_block);
    //p->next->next = NULL;
    //p->next->prev = NULL;
  }
  if ((p->prev != NULL) &&
      ((char *)p->prev + p->prev->size + sizeof(Metadata) == (char *)p)) {
    p->prev->size += sizeof(Metadata) + p->size;
    remove_block(p, first_free_block, last_free_block);
    //p->next = NULL;
    //p->prev = NULL;
  }
}

void * bf_malloc(size_t size,
                 Metadata ** first_free_block,
                 Metadata ** last_free_block,
                 int sbrk_lock) {
  Metadata * p = *first_free_block;
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
    return reuse_block(size, min_ptr, first_free_block, last_free_block);
  }
  else {
    return allocate_block(size, sbrk_lock);
  }
}
