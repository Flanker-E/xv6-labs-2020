#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // int i;
  if(argc<2){
    //less arg
    fprintf(2, "Usage: sleep time...\n");
    exit(1);
  }
  else{
    // get ticks and sleep
    int numTicks=atoi(argv[1]);
    sleep(numTicks);
    // exit(1);
  }

  // for(i = 1; i < argc; i++){
  //   write(1, argv[i], strlen(argv[i]));
  //   if(i + 1 < argc){
  //     write(1, " ", 1);
  //   } else {
  //     write(1, "\n", 1);
  //   }
  // }
  exit(0);
}