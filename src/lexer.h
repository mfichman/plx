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

#ifndef LEXER_H
#define LEXER_H

#include "mempool.h"

typedef struct Lexer Lexer;

typedef enum Token {
  TT_RESERVED = 255,
  TT_ERR,
  TT_EOF,
  TT_NE,
  TT_GE,
  TT_LE,
  TT_EQ,
  TT_STRING,
  TT_CONCAT
} TokenType;

Lexer* Lexer_new(MemPool* pool);
void Lexer_setpos(Lexer* self, char* data);
TokenType Lexer_nexttoken(Lexer* self);
char* Lexer_tokendata(Lexer* self);
size_t Lexer_tokenlen(Lexer* self);

#endif
