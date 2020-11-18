#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "hashfn.h"

char* val2key(FILE* fpvhs, FILE* fpkv, char* search) {

  int capacity = get_capacity(fpvhs);
  //printf("capacity: %d\n", capacity);
  //printf("%s\n", search);
  char* key = malloc(256);
  char val[256] = "";
  int valHash = hashfn(search, capacity);
  //printf("%d\n", valHash);
  int og_valHash = valHash;
  int* index = malloc(sizeof(int));


    while (1) {

      read_index(fpvhs, valHash, index);
      read_val(fpkv, *index, val);
      //printf("Val: %s\n", val);
      if (!strcmp(search, val)) {
        read_key(fpkv, *index, key);
        free(index);
        return(key);
      }

      valHash++;

      if (valHash == capacity) {
        valHash = 0;
      }

      if (valHash == og_valHash) {
        free(index);
        return("-1");
      }

    }

}

char* key2val(FILE* fpkhs, FILE* fpkv, char* search) {


  int capacity = get_capacity(fpkhs);
  //printf("capacity: %d\n", capacity);
  //printf("%s\n", search);
  char* val = malloc(256);
  char key[256] = "";
  int keyHash = hashfn(search, capacity);
  //printf("%d\n", keyHash);
  int og_keyHash = keyHash;
  int* index = malloc(sizeof(int));


    while (1) {

      read_index(fpkhs, keyHash, index);
      read_key(fpkv, *index, key);
      //printf("%d\n", keyHash);
      if (!strcmp(search, key)) {
        read_val(fpkv, *index, val);
        free(index);
        return(val);
      }

      keyHash++;

      if (keyHash == capacity) {
        keyHash = 0;
      }

      if (keyHash == og_keyHash) {
        free(index);
        return("-1");
      }

    }
  }





int main(int argc, char* argv[]) {

  if (argc!= 2) {
    fprintf( stderr, "Usage: %s 'actor'\n", argv[0]);
    exit(0);
  }

  FILE* fptbkhs = fopen("title.basics.khs", "rb");
  //FILE* fpnbkhs = fopen("name.basics.khs", "rb");
  FILE* fpnbvhs = fopen("name.basics.vhs", "rb");

  FILE* fptbkv = fopen("title.basics.kv", "rb");
  FILE* fptpkv = fopen("title.principals.kv", "rb");
  FILE* fpnbkv = fopen("name.basics.kv", "rb");

  char baconKey[256] = "nm0000102";
  char* actorName = argv[1];
  char* actorKey = val2key(fpnbvhs, fpnbkv, actorName);
  char* temp = NULL;
  char key[256];
  char val[256];

  char key2[256];
  char val2[256];

  //printf("Actor Key: %s\n", actorKey);
  int numReads = 1;
  int numReads2 = 2;
  int i = 0;
  int brk = 0;
  while(1) {
    numReads = read_val(fptpkv, i , val);
    //printf("%s\n", key);
    if (numReads != 1) {
      break;
    }



    if (strcmp(actorKey, val) == 0) {
      read_key(fptpkv, i, key);

      //printf("Key Of movie actor is in: %s\n", key);
      //printf("Val1: %s\n", val);
      rewind(fptpkv);
      while(1) {
        numReads2 = read_keyval(fptpkv, key2, val2 );
        if (numReads2 != 2) {
          printf("%s\n", "NOT FOUND");
          break;
        }
        //printf("Key = %s, Val = %s\n", key2, val2);
        if ((strcmp(baconKey, val2) == 0) && (strcmp(key2,key) == 0)) {
            //printf("Key: %s\n", key2);
            temp = key2val(fptbkhs, fptbkv, key );
            printf("%s\n", temp);
            free(temp);
            brk = 1;
            break;
        }

      }
      if (brk == 1) {
        break;
      }

    }

    i++;
  }

  fclose(fptbkv);
  fclose(fptpkv);
  fclose(fpnbkv);
  fclose(fpnbvhs);
  fclose(fptbkhs);
  free(actorKey);

}
