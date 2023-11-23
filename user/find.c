#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *path,char *file_name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  // printf("33debug:=1\n");
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  // printf("33debug:=1\n");
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  // printf("33debug:=1\n");
  switch(st.type){
  case T_DEVICE:
    printf("it is a deive not dir\n");
    break;
  case T_FILE:
    printf("it is a file not dir\n");
    printf("%s %d %d %d %d %d\n", buf, st.type, st.ino, st.size, st.nlink, st.dev);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      // printf("33debug:=4\n");
      // printf("%s %d %d %d %d %d\n", buf, st.type, st.ino, st.size, st.nlink, st.dev);
      if(st.type==2 || st.type==3){
        if(strcmp(de.name,file_name)==0){
          printf("%s\n",buf);
        }
      }
      else if(st.type==1){
        if(strcmp(de.name,".")==0 || strcmp(de.name,"..")==0){
          ;
        }
        else{
          find(buf,file_name);
        }
      }      
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("need a dir and a file name.");
    exit(0);
  }
  // printf("33debug:=1\n");
  find(argv[1],argv[2]);
  exit(0);
}
