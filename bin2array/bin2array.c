/* bin2array.c
 *
 * this program is more of a programming utility than a general one. it allows
 * you to specify a file and dump it to another file as a C/C++ array. it's
 * currently very simple and has no options (in the form of arguments).
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Joe Glancy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// v0.1.0

#include <stdio.h>

#define FILE_BUF_SIZE  16
#define USE_FORMATTING
//#define USE_LIBEXPLAIN // use libexplain to help explain errors. compile with -lexplain
#ifdef USE_LIBEXPLAIN
#include <libexplain/fread.h>
#endif

// usage: bin2array array_name infile outfile

#ifdef USE_FORMATTING
char *usage = "usage: \e[1m%s [-n name] infile [outfile]\e[0m\n";
#else
char *usage = "usage: %s [-n name] infile [outfile]\n";
#endif
char *start1 = "", *start2 = "[] = {";
char *separator = ", ";
char *end = "\n};";
unsigned char filebuf[FILE_BUF_SIZE];

int main(int argc, char *argv[]) {
    int infd, outfd;
    /* sort through the arguments. because there is only one, check to see if it
     * exists.
     */
    if (argc < 3) {
        printf(usage, argv[0]);
        return 0;
    }
    
    infd = fopen(argv[2], "r");
    if (infd == NULL) {
        #ifdef USE_FORMATTING
        fprintf(stderr, "\e[1;31merror:\e[0m could not open file: \"%s\".\n", argv[2]);
        #else
        fprintf(stderr, "error: could not open file: \"%s\".\n", argv[2]);
        #endif
        return 0;
    }
    
    outfd = fopen(argv[3], "w");
    if (outfd == NULL) {
        #ifdef USE_FORMATTING
        fprintf(stderr, "\e[1;31merror:\e[0m could not open file: \"%s\".\n", argv[3]);
        #else
        fprintf(stderr, "error: could not open file: \"%s\".\n", argv[3]);
        #endif
        fclose(infd);
        return 0;
    }
    
    int n;
    int byte;
    fprintf(outfd, "// %s\nunsigned char %s[] = {\n    ", argv[2], argv[1]);
    while ((n=fread(filebuf, sizeof(filebuf), FILE_BUF_SIZE, infd)) > 0) {
        int i;
        for (i=0; i<n; i++) {
            fprintf(outfd, "0x%02x, ", filebuf[i]);
        }
        // new line
        fprintf(outfd, "\n    ");
    }
    #ifdef USE_LIBEXPLAIN
    if (n < 0 && ferror(infd)) {
        printf("error encountered while reading file: %s\n", explain_fread(filebuf, sizeof(filebuf), FILE_BUF_SIZE, infd));
    }
    #endif
    // move the pointer back 7 places to get rid of the trailing comma
    fseek(outfd, 0L, SEEK_END);
    int size = ftell(outfd);
    fseek(outfd, size-7, SEEK_SET);
    fprintf(outfd, "\n};\n");
    fclose(infd);
    fclose(outfd);
    return 0;
}

