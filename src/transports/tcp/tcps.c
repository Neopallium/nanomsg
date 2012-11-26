/*
    Copyright (c) 2012 250bpm s.r.o.

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

#include "tcps.h"

#include "../../utils/err.h"
#include "../../utils/cont.h"

static void sp_tcps_hdr_received (const struct sp_sink **self,
    struct sp_usock *usock, size_t len);
static void sp_tcps_hdr_sent (const struct sp_sink **self,
    struct sp_usock *usock, size_t len);
static void sp_tcps_hdr_timeout (const struct sp_sink **self,
    struct sp_timer *timer);
static void sp_tcps_received (const struct sp_sink **self,
    struct sp_usock *usock, size_t len);
static void sp_tcps_sent (const struct sp_sink **self,
    struct sp_usock *usock, size_t len);
static void sp_tcps_err (const struct sp_sink **self,
    struct sp_usock *usock, int errnum);

/*  START state. */
static const struct sp_sink sp_tcps_state_start = {
    sp_tcps_hdr_received,
    sp_tcps_hdr_sent,
    NULL,
    NULL,
    sp_tcps_err,
    sp_tcps_hdr_timeout
};

/*  SENT state. */
static const struct sp_sink sp_tcps_state_sent = {
    sp_tcps_hdr_received,
    NULL,
    NULL,
    NULL,
    sp_tcps_err,
    sp_tcps_hdr_timeout
};

/*  RECEIVED state. */
static const struct sp_sink sp_tcps_state_received = {
    NULL,
    sp_tcps_hdr_sent,
    NULL,
    NULL,
    sp_tcps_err,
    sp_tcps_hdr_timeout
};

/*  ACTIVE state. */
static const struct sp_sink sp_tcps_state_active = {
    sp_tcps_received,
    sp_tcps_sent,
    NULL,
    NULL,
    sp_tcps_err,
    NULL
};

void sp_tcps_init (struct sp_tcps *self, struct sp_usock *usock)
{
    /*  Redirect the underlying socket's events to this state machine. */
    self->usock = usock;
    self->sink = &sp_tcps_state_start;
    self->original_sink = sp_usock_setsink (usock, &self->sink);

    /*  Start the header timeout timer. */
    sp_timer_init (&self->hdr_timeout, &self->sink, usock->cp);
    sp_timer_start (&self->hdr_timeout, 1000);

    /*  Send the protocol header. */
    sp_usock_send (usock, "\0\0SP\0\0\0\0", 8);

    /*  Receive the protocol header from the peer. */
    sp_usock_recv (usock, self->hdr, 8);
}

void sp_tcps_term ()
{
    sp_assert (0);
}

static void sp_tcps_hdr_received (const struct sp_sink **self,
    struct sp_usock *usock, size_t len)
{
    struct sp_tcps *tcps;

    tcps = sp_cont (self, struct sp_tcps, sink);

    sp_assert (len == 8);

    if (tcps->sink == &sp_tcps_state_sent) {
printf ("activated\n");
        tcps->sink = &sp_tcps_state_active;
        sp_timer_stop (&tcps->hdr_timeout);
        return;
    }

    if (tcps->sink == &sp_tcps_state_start) {
        tcps->sink = &sp_tcps_state_received;
        return;
    }

    /*  This event is not defined in other states. */
    sp_assert (0);
}

static void sp_tcps_hdr_sent (const struct sp_sink **self,
    struct sp_usock *usock, size_t len)
{
    struct sp_tcps *tcps;

    tcps = sp_cont (self, struct sp_tcps, sink);

    sp_assert (len == 8);

    if (tcps->sink == &sp_tcps_state_received) {
printf ("activated\n");
        tcps->sink = &sp_tcps_state_active;
        sp_timer_stop (&tcps->hdr_timeout);
        return;
    }

    if (tcps->sink == &sp_tcps_state_start) {
        tcps->sink = &sp_tcps_state_sent;
        return;
    }

    /*  This event is not defined in other states. */
    sp_assert (0);
}

static void sp_tcps_hdr_timeout (const struct sp_sink **self,
    struct sp_timer *timer)
{
    sp_assert (0);
}

static void sp_tcps_received (const struct sp_sink **self,
    struct sp_usock *usock, size_t len)
{
    sp_assert (0);
}

static void sp_tcps_sent (const struct sp_sink **self,
    struct sp_usock *usock, size_t len)
{
    sp_assert (0);
}

static void sp_tcps_err (const struct sp_sink **self,
    struct sp_usock *usock, int errnum)
{
    sp_assert (0);
}
