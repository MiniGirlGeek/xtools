/* getip.c
 *
 * this program actually turned out smaller than I'd expected. the user
 * specifies a hostnames as arguments, and then outputs the IP adress (ipv4) for
 * each one.
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
#include <sys/socket.h>
#include <netdb.h>

// usage: getip hosts ...

typedef unsigned char byte;

char *usage = "usage: %s hosts ...\n";

int main(int argc, char *argv[]) {
    struct hostent *host;
    char ipbuf[20];
    // check to see if we have at least one argument
    if (argc < 2) {
        printf(usage, argv[0]);
        return 0;
    }
    
    int i;
    for (i=1; i<argc; i++) {
        host = gethostbyname(argv[i]);
        if (host == NULL) {
            fprintf(stderr, "error: cannot resolve host %s\n", argv[1]);
            return 1;
        }
        // copy the address into the ipbuf buffer.
        sprintf(ipbuf, "%d.%d.%d.%d",
            (byte)host->h_addr_list[0][0],
            (byte)host->h_addr_list[0][1],
            (byte)host->h_addr_list[0][2],
            (byte)host->h_addr_list[0][3]);
        printf("%s\n", ipbuf);
    }
    return 0;
}

