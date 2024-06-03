#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
int main(int argc, char *argv[]) {
unsigned char key = 240;
int size = 27872;
for (int i = 0; i < size; i++) {
code[i] ^= key;
}
