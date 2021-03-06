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

#include "node.h"
#include "memory.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
  char* file = argv[1];
  char* buf = 0;
  long buflen = 0;
  FILE* fd = fopen(file, "r");
  TokenType token = 0;
  MemPool* pool = MemPool_new(1 << 30);
  Lexer* lexer = Lexer_new(pool);

  fseek(fd, 0, SEEK_END);
  buflen = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  buf = malloc(buflen);
  fread(buf, 1, buflen, fd);

  Lexer_setpos(lexer, buf);
  
  while (token != TT_EOF) {
    token = Lexer_debugtoken(lexer);
    printf("\n");
  }


  return 0;
}
