
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: sleep time(integer)\n");
        exit(1);
    }

    int n = strlen(argv[1]);
    for (int i = 0; i < n; ++i) {
        if (!('0' <= argv[1][i] && argv[1][i] <= '9')) {
            fprintf(2, "Usage: sleep time(integer)\n");
            exit(1);
        }
    }

    int to_integer = atoi(argv[1]);
    int signal = pause(to_integer);
    if (signal == -1) {
        fprintf(2, "sleep: process killed\n");
        exit(127);
    }

    exit(0);
}
