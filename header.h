#include<stdio.h>
#include<stdlib.h>

struct parent_params{
  int base;
  int batchsize;
  int type;
};
struct child_params{
  int file_index;
  int base;
  int type;
};
char *strrev(char *str);
char* itoa(int k);
void create_files_a(int n_a,int n_rand);
void create_files_b(int n_b,int n_rand);

void* add_numbers_files(void *param);
void* add_files_numbers(void *param);
