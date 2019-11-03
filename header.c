#include"header.h"
#include<string.h>
#include<stdio.h>
char *strrev(char *str)
{
      char *p1, *p2;
      //printf("%d\n",strlen(str));
      //printf("here3\n");
      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            //printf("here3\n");
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      //printf("here3\n");
      return str;
}

char* itoa(int k){
  char num[255];
  num[0]='\0';
  int a;
  char ch = '0';
  if(k==0){
    strncat(num,&ch,1);
  }
  else{
  while(k > 0){
    //printf("here2\n");
    a = k%10;
    k=k/10;
    char b = a+'0';
  //  printf("%c\n",b);
    strncat(num,&b,1);

  }
}
  char* ans = strrev(&num[0]);
  return ans;
}
void create_files_a(int n_a,int n_rand){
  int i=0;
  char* prefix_a = "a_";
  for(i=0;i<n_a;i++){
    FILE* fp;
    char new_file[255] = "./";
    strcat(new_file,prefix_a);
    /*printf("here\n");*/
    strcat(new_file,itoa(i));
    fp = fopen(new_file,"w");
    int j;
    for( j=0;j<n_rand;j++){
      int rand_num = rand()%1000;
      fprintf(fp,"%d\n",rand_num);
    }
    fclose(fp);
  }
}
void create_files_b(int n_b,int n_rand){
  int i=0;
  char* prefix_b = "b_";
  for(i=0;i<n_b;i++){
    FILE* fp;
    char new_file[255] = "./";
    strcat(new_file,prefix_b);
    /*printf("here\n");*/
    strcat(new_file,itoa(i));
    fp = fopen(new_file,"w");
    int j;
    for( j=0;j<n_rand;j++){
      int rand_num = rand()%1000;
      fprintf(fp,"%d\n",rand_num);
    }
    fclose(fp);
  }
}

void* add_numbers_files(void* param){
  struct parent_params* temp = (struct parent_params*)param;
  FILE* fp;
  int i;
  int sum=0;
  char* prefix;
  if(temp->type == 0){
    prefix = "a_";
  }
  else{
    prefix="b_";
  }
  for(i = 0;i<temp->batchsize;i++){
    char file_name[255] = "./";
    FILE*fp;
    strcat(file_name,prefix);
    strcat(file_name,itoa(temp->base + i ));
    fp = fopen(file_name,"r");
    int buff;
    while( fscanf(fp,"%d",&buff) ==1){
      sum = sum+buff;
    }
    fclose(fp);
  }
  return (void*)sum;

}
void* add_files_numbers(void* param){
  struct child_params* temp = (struct child_params*)param;
  int sum=0;
  char* prefix;
  if(param == NULL){
    return (void*)0;
  }
  if(temp->type == 0){
    prefix = "a_";
  }
  else{
    prefix="b_";
  }
  char file_name[255] = "./";
  FILE*fp;
  strcat(file_name,prefix);
  strcat(file_name,itoa(temp->file_index ));
  fp = fopen(file_name,"r");
  printf("%s\n",file_name);

  int i = 0,buff;
  while(fscanf(fp,"%d",&buff) ==1){
      if(i >= temp->base && i < (temp->base+500))
        {
          sum+= buff;
        }
              i++;
  }

    fclose(fp);
    return (void*)sum;

}
