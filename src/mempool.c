/*
 * Copyright (c) 2014 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "mempool.h"
#include "stdlib.h"
#include "assert.h"

struct MemPool {
  size_t size;
  char* pool;
  char* free;
};

MemPool* MemPool_new(size_t size) {
  MemPool* self = malloc(sizeof(MemPool));
  self->size = size;
  self->pool = malloc(size);
  self->free = self->pool;
  return self;
}

void* MemPool_alloc(MemPool* self, size_t bytes) {
  void* block = self->free;
  self->free += bytes;
  assert(((self->free-self->pool) < self->size) && "out of memory");
  return block;
}

void MemPool_free(MemPool* self) {
  free(self->pool);
  free(self);
}

