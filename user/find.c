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
find(char *path, char *pattern){
    // printf("%s %s pattern\n",path, pattern);
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            fprintf(2, "Usage: find in dir\n");
            exit(1);
            // printf("file %s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);

            // printf("%s %s\n",fmtname(path),pattern);
            // if(strcmp(fmtname(path),pattern)==0){
            //     printf(" find pattern %s\n",fmtname(path));
            //     // break;
            // }

            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0){
                    // printf(".or..\n");
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
                //find pattern
                switch(st.type){
                    case T_FILE:
                        if(strcmp(de.name,pattern)==0){
                            printf("%s\n",buf);
                        }
                    break;
                    case T_DIR:
                        if(fork()==0){
                            find(buf,pattern);
                            exit(0);
                        }
                        wait(0);
                    break;
                }
                
                
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
//   int i;

    if(argc < 2){
        fprintf(2, "Usage: find path string\n");
        exit(1);
    }
    else if(argc ==2){
        find(".", argv[1]);
        exit(0);
    }
    else if(argc ==3){
        find(argv[1], argv[2]);
        exit(0);
    }else{
        fprintf(2, "Usage: find path string\n");
        exit(1);
    }
  
  return 0;
}