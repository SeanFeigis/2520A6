#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "hashfn.h"


int main(int argc, char* argv[]) {

  if (argc!= 3) {
    fprintf( stderr, "Usage: %s filename.kv 'search term'\n", argv[0]);
    exit(0);
  }

  char* file = argv[1];
  char* search = argv[2];
  char otherFile[30] = "";

  FILE *fpkv;
  fpkv = fopen(file, "rb");


  FILE *fpvhs;
  strncpy(otherFile, file, strlen(file)-2);
  strcat(otherFile, "vhs");
  fpvhs = fopen(otherFile , "rb");

  int capacity = get_capacity(fpvhs);

  char key[256] = "";
  char val[256] = "";
  int valHash;
  valHash = hashfn(search, capacity);
  //printf("%d\n", valHash);
  int og_valHash = valHash;
  int* index = malloc(sizeof(int));


  while (1) {

    read_index(fpvhs, valHash, index);
    read_val(fpkv, *index, val);

    if (!strcmp(search, val)) {
      read_key(fpkv, *index, key);
      printf("%s\n", key);
      break;
    }

    valHash++;

    if (valHash == capacity) {
      valHash = 0;
    }

    if (valHash == og_valHash) {
      printf( "NOT FOUND\n" );
      break;
    }

  }

  fclose(fpkv);
  fclose(fpvhs);
  free(index);

}
