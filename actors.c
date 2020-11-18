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


    FILE* fptbvhs = fopen("title.basics.vhs", "rb");
    FILE* fpnbkhs = fopen("name.basics.khs", "rb");
    FILE* fptbkv = fopen("title.basics.kv", "rb");
    FILE* fptpkv = fopen("title.principals.kv", "rb");
    FILE* fpnbkv = fopen("name.basics.kv", "rb");

    char key[256] = "";
    char val[256] = "";
    char* actorName = NULL;
    char* title = argv[1];
    char* titleKey = NULL;
    int numreads = 1;
    titleKey = val2key(fptbvhs, fptbkv, title);
    //printf("TITLE KEY: %s\n", titleKey);

    int i = 0;
    while (1) {
      numreads = read_key(fptpkv, i, key);
      if (numreads != 1) {
        break;
      }
      if (strcmp(titleKey, key) == 0) {
        read_val(fptpkv, i, val);
        actorName = key2val(fpnbkhs, fpnbkv, val);
        printf("%s\n", actorName);
        free(actorName);
      }
      i++;
    }

    free(titleKey);
    fclose(fptbkv);
    fclose(fptbvhs);
    fclose(fptpkv);
    fclose(fpnbkv);
    fclose(fpnbkhs);

    return(0);

  }
