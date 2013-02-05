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

#ifndef NN_QUEUE_INCLUDED
#define NN_QUEUE_INCLUDED

struct nn_queue_item {
    struct nn_queue_item *next;
};

struct nn_queue {
    struct nn_queue_item *head;
    struct nn_queue_item *tail;
};

/*  Initialise the queue. */
void nn_queue_init (struct nn_queue *self);

/*  Terminate the queue. Note that queue must be manually emptied before the
    termination. */
void nn_queue_term (struct nn_queue *self);

/*  Inserts one element into the queue. */
void nn_queue_push (struct nn_queue *self, struct nn_queue_item *item);

/*  Retrieves one element from the queue. The element is removed
    from the queue. Returns NULL if the queue is empty. */
struct nn_queue_item *nn_queue_pop (struct nn_queue *self);

#endif