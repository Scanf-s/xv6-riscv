#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(2, "Usage: sixfive filename.txt\n");
        exit(1);
    }

    for (int file_idx = 1; file_idx < argc; ++file_idx){
        int fd = open(argv[file_idx], O_RDONLY);
        if (fd < 0) {
            fprintf(2, "failed to open a file\n");
            exit(1);
        }

        char c;
        int num = 0;
        int is_digit = 0;
        int is_invalid = 0;
        const char* separators = " -\r\t\n./,";
        while (read(fd, &c, 1) != 0) {

            if (strchr(separators, c) != 0) {
                if (
                    is_digit == 1 &&
                    is_invalid == 0 &&
                    (num % 6 == 0 || num % 5 == 0)
                ) {
                    printf("%d\n", num);
                }

                is_digit = 0;
                is_invalid = 0;
                num = 0;
            } else if (c >= '0' && c <= '9') {
                num = num * 10 + (c - '0');
                is_digit = 1;
            } else {
                is_invalid = 1;
            }
            
        }

        if (
            is_digit == 1 &&
            is_invalid == 0 &&
            (num % 6 == 0 || num % 5 == 0)
        ) {
            printf("%d\n", num);
        }

        close(fd);
    }
    exit(0);
}
