#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "hashfn.h"


int main(int argc, char* argv[]) {

  if (argc!= 3) {
    fprintf( stderr, "Usage: %s filename.kv capacity\n", argv[0]);
    exit(0);
  }
  char otherFile[30] = "";
  char otherFile2[30] = "";
  char* file = argv[1];
  int capacity = atoi(argv[2]);

  FILE *fpkv;
  fpkv = fopen(file, "rb");

  FILE *fpkhs;
  strncpy(otherFile, file, strlen(file)-2);
  strcat(otherFile, "khs");
  //printf("%s\n", otherFile);
  fpkhs = fopen(otherFile , "wb");


  FILE *fpvhs;
  strncpy(otherFile2, file, strlen(file)-2);
  strcat(otherFile2, "vhs");
  //printf("%s\n", otherFile2);
  fpvhs = fopen(otherFile2 , "wb");

  write_empty(fpkhs, capacity);
  write_empty(fpvhs, capacity);

  char* key = malloc(256);
  char* val = malloc(256);
  int keyHash;
  int valHash;
  int numreads = 2;
  //int tempIndex;
  int i = 0;
  int ind = -1;

  while (1) {
    //printf("%s\n", "before keyval");
    numreads = read_keyval(fpkv, key, val);
    if (numreads != 2) {
      break;
    }
    //printf("Key = %s, Val = %s\n", key, val);
    //printf("%s\n", "before hashfn");
    keyHash = hashfn(key, capacity);
    valHash = hashfn(val, capacity);

    //printf("keyHash = %d, valhash = %d\n", keyHash, valHash);

    //printf("%s\n", "before loop");

    while (1) {
      read_index(fpkhs, keyHash, &ind);
      //printf("return of keyHash read_index: %d\n", ind);
      if (ind == -1) {
        break;
      }
      keyHash++;
      //printf("%d bruh\n", keyHash);
      if (keyHash == capacity) { //Loop back around to 0 if it reaches capacity
        keyHash = 0;
      }

    }

    //printf("%s\n", "after read loop");

    write_index(fpkhs, i, keyHash);

    while (1) {
      read_index(fpvhs, valHash, &ind);
      //printf("return of valHash read_index: %d\n", ind);
      if (ind == -1) {
        break;
      }
      valHash++;
      if (valHash == capacity) { //Loop back around to 0 if it reaches capacity
        valHash = 0;
      }

    }

    write_index(fpvhs, i, valHash);

    i++;

  }

  fclose(fpkv);
  fclose(fpkhs);
  fclose(fpvhs);
  free(key);
  free(val);


  return(0);
}
