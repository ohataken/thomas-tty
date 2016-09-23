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

    sleep(3);

    unsigned char ch[] = {getchar()};

    write(fd, "got character: ", 15);
    write(fd, ch, 1);
    write(fd, "\n\r", 2);

    tcsetattr(fd, TCSAFLUSH, &orig_tios);

    return 0;
}
