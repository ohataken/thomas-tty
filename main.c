#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    int fd = open("/dev/tty", O_RDWR);

    struct termios orig_tios;
    struct termios tios;

    tcgetattr(fd, &orig_tios);
    memcpy(&tios, &orig_tios, sizeof(tios));

    tios.c_iflag &= ~IGNBRK;
    tios.c_iflag &= ~BRKINT;
    tios.c_iflag &= ~PARMRK;
    tios.c_iflag &= ~ISTRIP;
    tios.c_iflag &= ~INLCR;
    tios.c_iflag &= ~IGNCR;
    tios.c_iflag &= ~ICRNL;
    tios.c_iflag &= ~IXON;

    tios.c_oflag &= ~OPOST;

    tios.c_lflag &= ~ECHO;
    tios.c_lflag &= ~ECHONL;
    tios.c_lflag &= ~ICANON;
    tios.c_lflag &= ~ISIG;
    tios.c_lflag &= ~IEXTEN;
    tios.c_cflag &= CSIZE;
    tios.c_cflag &= PARENB;
    tios.c_cflag &= CS8;

    tios.c_cc[VMIN] = 0;
    tios.c_cc[VTIME] = 0;

    tcsetattr(fd, TCSAFLUSH, &tios);

    /* - - - */

    struct timeval timeval;
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    timeval.tv_sec = 5;
    timeval.tv_usec = 0;

    for (int i = 0; i < 10; ++i) {
        select(fd + 1, &readfds, NULL, NULL, &timeval);
        timeval.tv_sec = 5;
        timeval.tv_usec = 0;

        if (FD_ISSET(fd, &readfds)) {
            char buf[1];
            read(fd, buf, 1);
            printf("got: %s\r\n", buf);
        } else {
            printf("else\r\n");
            FD_SET(fd, &readfds);
        }
    }

    /* - - - */

    tcsetattr(fd, TCSAFLUSH, &orig_tios);

    return 0;
}
