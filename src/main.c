#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#define VERSION 1.0

typedef enum {
    mod_write,
    mod_read
} Mode;

void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
void parseArgs(int argc, const char *argv[]);

char *jpg_path;
bool isDebug = false;
unsigned int buff_sz = 128;
Mode mode = mod_read;
char *writeStr = NULL;

int main(int argc, const char *argv[]) {
    parseArgs(argc, argv);
    FILE *fptr = fopen(jpg_path, "rb");
    if(fptr == NULL) {
        fprintf(stderr, "%s: Couldn't open file, %s\n", argv[0], strerror(errno));
        return -1;
    }
    //unsigned const short max = 4900;
    bool fSet = false;
    int c;
    off_t offset = -1;
    for(/*unsigned short i = 0*/; /*i <= max && */(c = getc(fptr)) != EOF;/* i++*/) {
        if(isDebug)
            printf("%02x ", c);
        if(fSet) { 
            if(c == 217) {
                offset = ftell(fptr);
                //break;
            }
        }
        fSet = false;
        if(c == 255) {
            fSet = true;
        }
    }
    if(offset < 0) {
        fprintf(stderr, "%s: Invalid file\n", argv[0]);
        return -1;
    }
    printf("\n");
    fseek(fptr, offset, SEEK_SET);
    if(mode == mod_read) {
        if(isDebug) printf("Allocating buffer with size: %u\n", buff_sz);
        char *rBuff = (char *)malloc(buff_sz);
        if(rBuff == NULL) {
            fprintf(stderr, "Couldn't allocate buffer with size: %u\n", buff_sz);
            return -2;
        }
        {
            unsigned j = 0;
            while((c = getc(fptr)) != EOF) {
                rBuff[j] = (char)c;
                j++;
            }
        }
        printf("%s\n", rBuff);
        free(rBuff);
    }else if(mode == mod_write) {
        FILE *nfptr = fopen(jpg_path, "a");
        assert(writeStr != NULL);
        fprintf(nfptr, writeStr);
        fclose(nfptr);
    }
    fclose(fptr);
    return 0;
}
void parseArgs(int argc, const char *argv[]) {
    int c;
    char *cval = NULL;
    while((c = getopt(argc, argv, "dw:Vf:s:v")) != -1) {
        switch (c) {
            case 'v':
            case 'd':
                isDebug = true;
                break;
            case 'f':
                cval = optarg;
                break;
            case 'w':
                mode = mod_write;
                writeStr = optarg;
                break;
            case 'V':
                printf("%s Version: %f\n", argv[0], VERSION);
                break;
            case 's':
                buff_sz = atoi(optarg);
                break;
            case 'r': break;
            case '?':
                if (optopt == 'f' || optopt == 's' || optopt == 'w') {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    abort();
                } else if(isprint(optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt
                    );
            default: abort();
        }
    }
    if(cval != NULL)
        jpg_path = cval;
}