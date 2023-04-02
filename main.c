#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main() {
    // Open serial port
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("Failed to open serial port");
        return 1;
    }

    // Configure serial port settings
    struct termios serial_settings;
    tcgetattr(fd, &serial_settings);
    cfsetspeed(&serial_settings, B9600);
    serial_settings.c_cflag &= ~PARENB;
    serial_settings.c_cflag &= ~CSTOPB;
    serial_settings.c_cflag &= ~CSIZE;
    serial_settings.c_cflag |= CS8;
    serial_settings.c_cflag &= ~CRTSCTS;
    serial_settings.c_cflag |= CREAD | CLOCAL;
    serial_settings.c_iflag &= ~(IXON | IXOFF | IXANY);
    serial_settings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    serial_settings.c_oflag &= ~OPOST;
    serial_settings.c_cc[VMIN] = 0;
    serial_settings.c_cc[VTIME] = 10;
    tcsetattr(fd, TCSANOW, &serial_settings);

    // Send command to board
    char command[] = "digitalWrite(13, HIGH)\n";
    int bytes_written = write(fd, command, strlen(command));
    if (bytes_written < 0) {
        perror("Failed to write to serial port");
        close(fd);
        return 1;
    }

    // Wait for board to process command
    usleep(100000);

    // Send another command to board
    strcpy(command, "digitalWrite(13, LOW)\n");
    bytes_written = write(fd, command, strlen(command));
    if (bytes_written < 0) {
        perror("Failed to write to serial port");
        close(fd);
        return 1;
    }

    // Wait for board to process command
    usleep(100000);

    // Close serial port
    close(fd);

    return 0;
}
