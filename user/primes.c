#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1
int primes(int *pre_p){
    int prime;
    int newCome;
    int p[2];
    close(pre_p[WRITE]);
    pipe(p);
    //pipe close
    read(pre_p[READ],&prime,sizeof(prime));
    printf("prime %d\n",prime);
    int flag=read(pre_p[READ],&newCome,sizeof(newCome));
    if(flag){
        if(fork()==0){
            primes(p);
        }
        else{
            close(p[READ]);
            if(newCome%prime!=0)
                write(p[WRITE],&newCome,sizeof(newCome));
            while(read(pre_p[READ],&newCome,sizeof(newCome))){
                if(newCome%prime!=0)
                    write(p[WRITE],&newCome,sizeof(newCome));
            }
            close(pre_p[READ]);
            close(p[WRITE]);
            wait(0);
        }
    }
    
    exit(0);
}
int
main(int argc, char *argv[])
{
//   int i;

    if(argc >= 2){
        fprintf(2, "Usage: primes\n");
        exit(1);
    }
    // char buf[1];
    int p[2];
    pipe(p);
    //pipe close 
    
    if(fork()==0){
        primes(p);
    }
    else{
        close(p[READ]);
        for(int i=2;i<32;i++){
            write(p[WRITE],&i,sizeof(i));
        }
        close(p[WRITE]);
        wait(0);
    }
    exit(0);

}