#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  char* print_buf=(char*)malloc(sizeof(char)*40);
  print_buf="pingpong go!\n";
  atom_printf(print_buf);
  int p1[2];
  pipe(p1);
  int p2[2];
  pipe(p2);
  char buf[2];
  int pid=fork();
  if( pid == 0) {
    write(p1[1],"1",1);
    while(read(p2[0],buf,1)!=0){
      if(*buf=='2'){
        sprintf(print_buf,strlen(print_buf),"<%d>: received pong\n",pid);
        atom_printf(print_buf);
        break;
      }
    }
  } else {
    while(read(p1[0],buf,1)!=0){
      if(*buf=='1'){
        sprintf(print_buf,strlen(print_buf),"<%d>: received ping\n",pid);
        atom_printf(print_buf);
        write(p2[1],"2",1);
        break;
      }
    }
  }
  exit(0);
}