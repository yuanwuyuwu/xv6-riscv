#include "kernel/types.h"
#include "user/user.h"
void swap_int(int* a, int* b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}
int main(){
  int pipe1[2];
  pipe(pipe1);
  for(int i=2;i<=100;i++){
    write(pipe1[1],&i,4);
  }
  // printf("info=%d %d\n",pipe1[0],pipe1[1]);
  close(pipe1[1]);
  int pipe2[2];
  pipe(pipe2);
  while(1){
    int pid=fork();
    if(pid!=0){
      int temp;
      int k;
      int n=read(pipe1[0],&k,4);
      if(n==0){
        goto out;
      }
      printf("prime %d\n",k);
      while(1){
        int n=read(pipe1[0],&temp,4);
        if(n==0){
          close(pipe2[1]);
          close(pipe1[0]);
          pipe1[0]=pipe2[0];
          pipe1[1]=pipe2[1];
          pipe(pipe2);
          // if(pipe(pipe2)!=0){
          //   printf("xv6 has limited number of file descriptors and processes");
          // }
          // printf("pipes= %d %d\n",pipe1[0],pipe1[1]);
          break;
        }
        // printf("info num = %d n==%d\n",temp,n);
        if(temp%k!=0){
          write(pipe2[1],&temp,4);
        }
      }
    }
    else{
      goto out;
    }
  }
  // int pid=fork();
  out:
  exit(0);
}