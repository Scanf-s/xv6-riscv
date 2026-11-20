#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
basename(char *path)
{
  char *p;

  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void find(char* path, char* name) {
    int fd;
    struct stat st;
    struct dirent de;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_DIR:
            {
                // path len + '/' + name of the file + '\0'
                char buf[512];
                if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                    printf("find: path is too long\n");
                    break;
                }

                strcpy(buf, path);
                char *p = buf + strlen(buf);
                *p++ = '/';
                while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                    if(de.inum == 0) {
                        continue;
                    }

                    memmove(p, de.name, DIRSIZ);
                    p[DIRSIZ] = '\0';
                    if(strcmp(p, ".") == 0 || strcmp(p, "..") == 0) {
                        continue;
                    }
                    find(buf, name);
                }
                break;
            }
        case T_FILE:
            {
                char* b = basename(path);
                if (strcmp(b, name) != 0) {
                    break;
                }
                printf("%s\n", path);
                break;
            }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "usage: find path target\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    return 0;
}
