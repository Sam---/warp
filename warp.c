#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>

const char ETX = '\x3';

int _findwarp(char *warp, char *buffer, size_t bl, FILE *stream) {
    size_t bcur = 0;
    size_t wcur = 0;
    int c = 0;
    while ((c = fgetc(stream)) != EOF) {
        if (warp[wcur] == c) {
            if (warp[wcur++] == '\0') {
                bcur = 0;
                while ((c = fgetc(stream)) != EOF && c && bcur < bl) {
                    buffer[bcur++] = c;
                }
                if (bcur >= bl) {
                    puts("warp: path too long, max is " STRWPMAX);
                    exit(EXIT_FAILURE);
                }
                return 0;
            }
        } else {
            wcur = 0;
            if (c == ETX) {
                while ((c = fgetc(stream)) != EOF && c);
            }
        }
    }
    return -1;
}

const char *warpfilename(void) {
    const char *warpfile;
    if (warpfile = getenv("WARPFILE")) {} else {
        warpfile = "~/.warpfile";
    }
    return warpfile;
}

json_t *getwarpfile(void) {
    const char *warpfile = warpfilename();
    json_error_t error;
    json_t *tree = json_load_file(warpfile, 0, &error);
    if (!tree) {
        fprintf(stderr, "warp: error reading %s on line %d: %s\n",
                warpfile, error.line, error.text);
        exit(EXIT_FAILURE);
    }
    return tree;
}

void setwarp(const char *name, const char *path) {
    const char *warpfile = warpfilename();
    json_t tree = getwarpfile();
    char abspath[PATH_MAX];
    realpath(path, abspath);
    json_object_set_new(tree, name, json_string(abspath));

    if (json_dump_file(tree, warpfile, JSON_COMPACT) != 0) {
        fprintf(stderr, "warp: error while writing to %s\n", warpfile);
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char* argv) {
    if (argc < 2) {
        usage();
    } else {
        if (argv[1][0] == '-') switch (argv[1][1]) {
            case 's':
                if (argc < 3 && !argv[1][2]) {
                    puts("warp: need another arguments to \e[4ms\e[0met");
                } else {
                    char *name = argv[1][2]? argv[1] + 1 : argv[2];
                    if (argc < 4) {
                        setwarp(name, ".");
                    } else {
                        setwarp(name, argv[3]);
                    }
                }
                break;
            case 'd':
                    

        

