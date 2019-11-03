#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include"header.h"
#include<unistd.h>
#include<math.h>
 #include <sys/wait.h>

int main(int argc, char const *argv[]) {
  if(argc < 2){
    printf("Usage : <bin> <number of a files> <number of b files> <number of random integers> <number of threads in parent process> <number of threads in child process>\n");
    return 0;
  }

  int n_a = atoi(argv[1]);
  int n_b = atoi(argv[2]);
  int n_rand = atoi(argv[3]);
  int num_threads_parent = atoi(argv[4]);
  int num_threads_child = atoi(argv[5]);
  create_files_a(n_a,n_rand);
  create_files_b(n_b,n_rand);
  int flag_child,flag_parent;
  flag_child = (n_b/num_threads_child >= 1)?1:0;
  flag_parent = (n_a/num_threads_parent >= 1)?1:0;
  int fd[2];
  pipe(fd);
  int k = fork();
  if(k == 0){
      int batch_size = n_b/num_threads_child;
      pthread_t tid_array[num_threads_child];
      int sum=0;
      struct parent_params* params_thread[num_threads_child];
      int j=0;
      for(j=0;j<num_threads_child;j++){
        params_thread[j] = (struct parent_params*)malloc(sizeof(struct parent_params));
        params_thread[j]->base = j*batch_size;
        params_thread[j]->type=1;
        if(j==0){
          params_thread[j]->batchsize = batch_size+ n_b%num_threads_child;
        }
        else{
          params_thread[j]->batchsize = batch_size;
        }

        pthread_create(&tid_array[j], NULL, add_numbers_files, params_thread[j]);
      }
      void* sum_file;
      for(j=0;j<num_threads_child;j++){
        pthread_join(tid_array[j], &sum_file);
        sum+=(int)sum_file;
      }
      printf("Child Process %d\n",sum);
      close(fd[0]);
      write(fd[1],&sum, sizeof(sum));
      close(fd[1]);

  }


  else{
    if(flag_parent){
      int batch_size = n_a/num_threads_parent;
      pthread_t tid_array[num_threads_parent];
      int sum=0;
      struct parent_params* params_thread[num_threads_parent];
      int j=0;
      for(j=0;j<num_threads_parent;j++){
        params_thread[j] = (struct parent_params*)malloc(sizeof(struct parent_params));
        params_thread[j]->base = j*batch_size;
        params_thread[j]->type=0;
        if(j==0){
          params_thread[j]->batchsize = batch_size+ n_a%num_threads_parent;
        }
        else{
          params_thread[j]->batchsize = batch_size;
        }
        pthread_create(&tid_array[j], NULL, add_numbers_files, params_thread[j]);
      }
      void* sum_file;
      for(j=0;j<num_threads_parent;j++){
        pthread_join(tid_array[j], &sum_file);
        sum+=(int)sum_file;
      }
    /*for(j=0;j<num_threads_parent;j++){
        printf("thread %d base %d\n",j,params_thread[j]->base);

        printf("batch size %d\n",params_thread[j]->batchsize );
    }*/
      int sum_child;
      wait(NULL);
      close(fd[1]);
      read(fd[0],&sum_child,sizeof(sum_child));
      close(fd[0]);
      printf("Parent process %d\n",sum);
      sum = sum+sum_child;
      printf("Parent process Total %d\n",sum);
    }
    else{
      int n_threads_per_file = ceil((float)n_rand/(float)500);
      if(n_threads_per_file*n_a > num_threads_parent ){
        printf("Life shouldn't be this tough\n");
        exit(0);
      }
      else{
        pthread_t tid_array[num_threads_parent];
        int sum=0;
        struct child_params* params_thread[num_threads_parent];
        int i,j=0;
        for( i=0;i<num_threads_parent;i++){
          params_thread[i] = (struct child_params*)malloc(sizeof(struct child_params));
          params_thread[i]->file_index = i/n_threads_per_file;
          params_thread[i]->base = i%n_threads_per_file;
          params_thread[i]->type = 0;
          if(params_thread[i]->file_index >= n_a){
            params_thread[i] = NULL;
          }
            pthread_create(&tid_array[i], NULL, add_files_numbers, params_thread[i]);
        }
        void* sum_file;
        for(j=0;j<num_threads_parent;j++){
          pthread_join(tid_array[j], &sum_file);
          sum+=(int)sum_file;
        }
        int sum_child;
        wait(NULL);
        close(fd[1]);
        read(fd[0],&sum_child,sizeof(sum_child));
        close(fd[0]);
        printf("Parent process %d\n",sum);
        sum = sum+sum_child;
        printf("Parent process Total %d\n",sum);
      }

      }

    }
  //printf("%d %d\n",flag_child,flag_parent);
  return 0;
}
