/* choice.c
 *
 * this is a simple program, designed to be run from the command line and used
 * mainly in shell scripts. it presents the user with a string and a set of
 * options, and return the index of the option they select.
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
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define USE_FORMATTING  // whether to format output (e.g: colour errors red)

typedef unsigned int uint;
typedef unsigned char xbool;

/* badly written comment below, I'll reword it later. also need to settle on
 * whether they're called options or choices.
 *
 *
 * usage: choice [-c choices] [-n] text
 *
 * options:
 * -c, --choices        this is a single string which defines the choices
 *                      avilable to the user. each choice is one character, and
 *                      the default is yn. if more than one is specified, the
 *                      last one will be used. the return value of the program
 *                      is the index of the chosen option+1, so it begins at one
 *                      and carries on, with the second option being 2 etc. the
 *                      value 0 is returned upon error or through the -h option.
 *                      the actual option string is the argument after this one.
 * -n, --nooptions      hide the options printed at the end of the question
 * -h, --help           display the usage message, along with option
 *                      definitions.
 *
 * "text" is basically the question posed to the user.
 */

#ifdef USE_FORMATTING
char *usage = "usage: \e[1m%s [-c choices] [-n] text\e[0m\n";
#else
char *usage = "usage: %s [-c choices] [-n] text\n";
#endif
char *help = "options:\n\
 -c, --choices          list of characters the user can choose from. if none\n\
                        are specified, \"yn\" is used as a default (note: the\n\
                        program is case-sensitive).\n\
 -n, --nooptions        do not display the options (defined with -c). default\n\
                        is display options at the end (within square brackets).\n\
 text                   text to display to the user, usually a question\n\
                        corresponding to the options.\n";
char *options = "yn";
int textindex = 0;
xbool nooptions = 0;

/* initially I was going to create both my own strlen and strcmp to make the
 * resulting executable (hopefully) ever so slightly smaller as then I wouldn't
 * need string.h. so far, I only have xstrlen.
 */
uint xstrlen(char *str) {
    char *ptr = str;
    while (*ptr++);
    return ptr-str-1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // no arguments
        printf(usage, argv[0]);
        return 0;
    }
    // process through the arguments
    int i;
    for (i=1; i<argc; i++) {
        if (*argv[i] == 45) { // 45 = ASCII for "-" (compiler didn't like *argv[i]=="-" :\ )
            argv[i]++;
            if (*argv[i] == 45) {
                /* long option invoked. I *could* only check the first letter of
                 * the argument to make it easier (--choices and --casesensitive
                 * would need checking with the second letter too), however the
                 * user could have specified something completely different to
                 * the argument, like --chocolate which would have been
                 * interpreted as --choices.
                 */
                argv[i]++;
                if (!strcmp(argv[i], "choices")) {
                    if (i+1 < argc) {
                        options = argv[i+1];
                        i++;
                    }
                    else { // no argument after "-c"
                        #ifdef USE_FORMATTING
                        fprintf(stderr, "\e[1;31merror:\e[0m expected argument after \"-c\".\n");
                        #else
                        fprintf(stderr, "error: expected argument after \"-c\".\n");
                        #endif
                        return 0;
                    }
                }
                else if (!strcmp(argv[i], "nooptions")) {
                    nooptions = 1;
                }
                else if (!strcmp(argv[i], "help")) {
                    printf(usage, argv[0]);
                    printf(help);
                    return 0;
                }
                else {
                    argv[i] -= 2;
                    #ifdef USE_FORMATTING
                    fprintf(stderr, "\e[1;31merror:\e[0m unrecognised argument \"%s\".\n", argv[i]);
                    #else
                    fprintf(stderr, "error: unrecognised argument \"%s\".\n", argv[i]);
                    #endif
                    return 0;
                }
            }
            else {
                /* short option invoked
                 * note: -hx would be recognised as -h, therefore ignoring any
                 * letters beside the first one.
                 */
                if (*argv[i] == 99) { // 99 = ASCII for "c"
                    if (i+1 < argc) {
                        options = argv[i+1];
                        i++;
                    }
                    else { // no argument after "-c"
                        #ifdef USE_FORMATTING
                        fprintf(stderr, "\e[1;31merror:\e[0m expected argument after \"-c\".\n");
                        #else
                        fprintf(stderr, "error: expected argument after \"-c\".\n");
                        #endif
                        return 0;
                    }
                }
                else if (*argv[i] == 110) { // 110 = ASCII for "n"
                    nooptions = 1;
                }
                else if (*argv[i] == 104) { // 104 = ASCII for "h"
                    printf(usage, argv[0]);
                    printf(help);
                    return 0;
                }
                else {
                    argv[i]--;
                    #ifdef USE_FORMATTING
                    fprintf(stderr, "\e[1;31merror:\e[0m unrecognised argument \"%s\".\n", argv[i]);
                    #else
                    fprintf(stderr, "error: unrecognised argument \"%s\".\n", argv[i]);
                    #endif
                    return 0;
                }
                    
            } // end of short option invoked
        }
        else {
            // if the argument doesn't start with a "-", it must be "text"
            if (!textindex) {
                textindex = i;
            }
        }
    } // end of argument processing loop
    
    uint lenoptions = xstrlen(options);
    
    /* here the terminal needs to be set so that return does not need to be
     * pressed to enter a character.
     */
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    while (1) {
        char input = 0;
        int i;
        for (i=textindex; i<argc; i++) {
            printf("%s ", argv[i]);
        }
        if (!nooptions) {
            printf("[%s] ", options);
        }
        scanf("%c", &input);
        printf("\n");
        for (i=0; i<lenoptions; i++) {
            if (options[i] == input) {
                // we've got outselves a match.
                tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
                return i+1;
            }
        }
        if (input != 10) {
            printf("invalid option %c.\n", input);
        }
    }
    // should never get here
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    return -42;
}

