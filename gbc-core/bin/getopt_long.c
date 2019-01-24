/*

Copyright (c) 2015 gameboxcloud.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#define K_PREFIX 10000

static int k_debug = 0;
static int k_reload = 0;

static struct option long_options[] = {
    /* These options set a flag. */
    {"all", no_argument, 0, 'a'},
    {"nginx", no_argument, 0, 'n'},
    {"redis", no_argument, 0, 'r'},
    {"beanstalkd", no_argument, 0, 'b'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"debug", no_argument, &k_debug, 1},
    {"reload", no_argument, &k_reload, 1},
    {"prefix", required_argument, 0, K_PREFIX},
    {0, 0, 0, 0}
};

char* save_option(char **opts, char c, int index, unsigned* len, unsigned* max_len) {
    char opt[20];

    if (index > 0) {
        strcpy(opt, " --");
        strcat(opt, long_options[index].name);
    }
    else {
        char tmp[4] = {' ', '-', c, '\0'};
        strcpy(opt, tmp);
    }

    if (*len + strlen(opt) + 1 > *max_len) {
        *opts = (char*)realloc(*opts, *max_len+strlen(opt)*2+1);
        if (!*opts) {
            return NULL;
        }
    }

    *opts = strcat(*opts, opt);
    *len = *len + strlen(opt) + 1;
    *max_len = *max_len + strlen(opt)*2 + 1;

    return *opts;
}

int main (int argc, char **argv) {
    int c;
    char *install_path = NULL;
    char *options = NULL;
    unsigned int cur_opt_len = 0;
    unsigned int max_opt_len = 0;

    while(1) {

        int option_index = -1;

        c = getopt_long (argc, argv, "abrnh", long_options, &option_index);
        if (c == -1)
            break;

        switch(c) {
            /* store path from prefix */
            case K_PREFIX:
                install_path = optarg;
                break;

            /* handle "--debug" and "--reload" */
            case 0:
                break;

            case 'a':
            case 'b':
            case 'r':
            case 'n':
            case 'v':
            case 'h':
                if (!save_option(&options, c, option_index, &cur_opt_len, &max_opt_len)) {
                    free(options);
                    exit(-1);
                }
                break;

            case '?':
                free(options);
                exit(-1);

            default:
                abort();
        }
    }

    /* output args as command "getopt". */
    if (options) {
        printf("%s ", options);
    }

    if (install_path) {
        printf("--prefix %s ", install_path);
    }

    if (k_debug) {
        printf("--debug ");
    }

    if (k_reload) {
        printf("--reload ");
    }

    printf("--");

    free(options);

    return 0;
}
