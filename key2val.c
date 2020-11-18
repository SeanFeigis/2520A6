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
  //file stuff
  char* file = argv[1];
  char* search = argv[2];
  char otherFile[30] = "";

  FILE *fpkv;
  fpkv = fopen(file, "rb");


  FILE *fpkhs;
  strncpy(otherFile, file, strlen(file)-2);
  strcat(otherFile, "khs");
  fpkhs = fopen(otherFile , "rb");

  int capacity = get_capacity(fpkhs);

  char key[256] = "";
  char val[256] = "";
  int keyHash;
  keyHash = hashfn(search, capacity); //get the hash value of the search term
  int og_keyHash = keyHash;
  int* index = malloc(sizeof(int));


  while (1) {

    read_index(fpkhs, keyHash, index); //read the index
    read_key(fpkv, *index, key); //read the value of index
    if (!strcmp(search, key)) { //check for comparison and print if correct
      read_val(fpkv, *index, val);
      printf("%s\n", val);
      break;
    }
    //linear probing
    keyHash++;

    if (keyHash == capacity) {
      keyHash = 0;
    }

    if (keyHash == og_keyHash) {
      printf( "NOT FOUND\n" );
      break;
    }

  }
  //close everything
  fclose(fpkv);
  fclose(fpkhs);
  free(index);

  return(0);
}
