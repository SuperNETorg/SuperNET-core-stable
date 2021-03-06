/*
    Copyright (c) 2014 Martin Sustrik  All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "../src/nn.h"
#include "../src/pair.h"
#include "../src/tcpmux.h"

#include "testutil.h"

/*  Tests TCPMUX transport. */

int sc;

int main ()
{
#if !defined NN_HAVE_WINDOWS
    int rc;
    int sb;
    int sc;
    int i;

    /*  First, start tcpmux daemon. */
    rc = nn_tcpmuxd (5555);
    errno_assert (rc == 0);

    /*  Create a connection. */
    sb = test_socket (AF_SP, NN_PAIR);
    test_bind (sb, "tcpmux://*:5555/foo");
    sc = test_socket (AF_SP, NN_PAIR);
    test_connect (sc, "tcpmux://127.0.0.1:5555/foo");

    /*  Ping-pong test. */
    for (i = 0; i != 100; ++i) {
        test_send (sc, "ABC");
        test_recv (sb, "ABC");
    }

    /*  Cleanup. */
    test_close (sc);
    test_close (sb);
#endif

    return 0;
}

