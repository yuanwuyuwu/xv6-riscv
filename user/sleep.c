#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc<2 || argc>2){
    write(1, "need 1 parameter\n", 17);
    exit(0);
  }
  int time=atoi(argv[1]);
  sleep(time);
  exit(0);
}