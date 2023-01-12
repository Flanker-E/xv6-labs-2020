#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

int
fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

int
main(int argc, char *argv[])
{
//   int i;

  if(argc >= 2){
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }

//   for(i = 1; i < argc; i++){
//     if(unlink(argv[i]) < 0){
//       fprintf(2, "rm: %s failed to delete\n", argv[i]);
//       break;
//     }
//   }
  int p1[2];
  int p2[2];
  char buf[1];
  if(pipe(p1)<0||pipe(p2)<0){
    panic("pipe");
  }
//   write(p1[WRITE],"1",1);
  if(fork1()==0){
    //child
    close(p1[WRITE]);
    close(p2[READ]);
    read(p1[READ],buf,1);
    printf("%d: received ping\n", getpid());
    write(p2[WRITE],"1",1);
    close(p1[READ]);
    close(p2[WRITE]);
    exit(0);
    }
  //parent
//   wait(0);
  close(p1[READ]);
  close(p2[WRITE]);
  write(p1[WRITE],"1",1);
  read(p2[READ],buf,1);
  printf("%d: received pong\n", getpid());
  close(p1[WRITE]);
  close(p2[READ]);
  exit(0);
}