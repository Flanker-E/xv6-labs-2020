#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXARGS 10

int
main(int argc, char *argv[])
{
    // int i;
    // int *n=argv;
    // read(0,n++,1);
    while(1){
        int len=0;
        char append_argv[MAXARGS][MAXARG];
        char *m[MAXARGS];
        
        while(1){
            // int i;
            char c;
            char read_argv[MAXARG];
            memset(append_argv,0,sizeof(char)*MAXARG*MAXARGS);
            for(int i=1;i<argc;i++){
                strcpy(append_argv[i-1],argv[i]);
                m[i-1]=append_argv[i-1];
            }
            // char *n=read_argv;
            int flag=read(0,&c,1);
            if(flag!=0){
                read_argv[len]=c;
                len++;
            }
            else{
                break;
            }
            if(c=='\n'){
                int temp_ind=0, ind=0;
                int i=argc-1;
                while(read_argv[temp_ind]!='\n'){
                    if(read_argv[temp_ind]==' '){
                        // append_argv[i][ind]='\n';
                        m[i]=append_argv[i];
                        // printf("%s",append_argv[i]);
                        temp_ind++;
                        i++;
                        ind=0;
                    }
                    else{
                        append_argv[i][ind++]=read_argv[temp_ind++];
                    }
                }
                // append_argv[i][ind]='\n';
                m[i]=append_argv[i];
                // printf("%s\n",append_argv[i]);
                if(fork()==0){
                    exec(m[0], m);
                    }
                else
                    wait(0);
                // printf("%s %d\n",read_argv,len);
                break;
            }
        }
        if(len<=0){
            break;
        }
        else{
            //get new args
            
        }
    }
    
    // for(i = 1; i < argc; i++){
    //     printf("%s \n",argv[i]);
        // exec(argv[1], argv+1);
        // write(1, argv[i], strlen(argv[i]));
        // if(i + 1 < argc){
        //   write(1, " ", 1);
        // } else {
        //   write(1, "\n", 1);
        // }
    // }
    exit(0);
}