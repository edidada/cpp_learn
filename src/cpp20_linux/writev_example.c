#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    const char *str1 = "Hello, ";
    const char *str2 = "world!\n";

    struct iovec iov[2];
    iov[0].iov_base = (void*)str1;
    iov[0].iov_len = strlen(str1);
    iov[1].iov_base = (void*)str2;
    iov[1].iov_len = strlen(str2);

    int connfd = STDOUT_FILENO; // 例如：写入到标准输出
    ssize_t ret = writev(connfd, iov, 2);

    if (ret == -1) {
        perror("writev error");
    }

    return 0;
}