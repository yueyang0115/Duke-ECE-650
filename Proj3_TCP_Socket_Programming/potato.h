#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
class Potato {
 public:
  int num_hops;
  int count;
  int path[512];
  Potato() : num_hops(0), count(0) { memset(path, 0, sizeof(path)); }
};
