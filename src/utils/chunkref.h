/*
    Copyright (c) 2013 250bpm s.r.o.

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

#ifndef NN_CHUNKREF_INCLUDED
#define NN_CHUNKREF_INCLUDED

#define NN_CHUNKREF_MAX 32

#include "chunk.h"

#include <stdint.h>
#include <stddef.h>

/*  This class represents a reference to a data chunk. It's either an actual
    reference to data allocated on the heap, or if short enough, it may store
    the data in itself. While user messages are not often short enough to store
    them inside the chunkref itself, SP protocol headers mostly are and thus
    we can avoid additional memory allocation per message. */

struct nn_chunkref {
    uint8_t ref [NN_CHUNKREF_MAX];
};

/*  Initialise the chunkref. The actual storage will be either on stack (for
    small messages, or will be allocated via nn_chunk object. */
void nn_chunkref_init (struct nn_chunkref *self, size_t size);

/*  Create a chunkref from an existing chunk object. */
void nn_chunkref_init_chunk (struct nn_chunkref *self, struct nn_chunk *chunk);

/*  Deallocate the chunk. */
void nn_chunkref_term (struct nn_chunkref *self);

/*  Get the unerlying chunk. If it doesn't exist (small messages) it allocates
    one. Chunkref points to empty chunk after the call. */
struct nn_chunk *nn_chunkref_getchunk (struct nn_chunkref *self);

/*  Moves chunk content from src to dst. dst should not be initialised before
    calling this function. After the call, dst becomes initialised and src
    becomes uninitilised. */
void nn_chunkref_mv (struct nn_chunkref *dst, struct nn_chunkref *src);

/*  Copies chunk content from src to dst. dst should not be initialised before
    calling this function. */
void nn_chunkref_cp (struct nn_chunkref *dst, struct nn_chunkref *src);

/*  Returns the pointer to the binary data stored in the chunk. */
void *nn_chunkref_data (struct nn_chunkref *self);

/*  Returns the size of the binary data stored in the chunk. */
size_t nn_chunkref_size (struct nn_chunkref *self);

/*  Trims n bytes from the beginning of the chunk. */
void nn_chunkref_trim (struct nn_chunkref *self, size_t n);

#endif

