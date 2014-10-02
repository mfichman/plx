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

#include "lexer.h"
#include "assert.h"
#include "ctype.h"

struct Lexer {
  char* pos;
  char ch;
  int line;
  int column;
  char* tokendata;
  int tokenlen; 
};

static void tokenappend(Lexer* self, char ch) {
	assert(!"not implemented");
}

static void error(Lexer* self, char const* msg) {
	assert(!"not implemented");
}


static void nextchar(Lexer* self) {
  self->pos++;
  self->ch = *self->pos;
}

static void nextline(Lexer* self) {
  self->column = 0;
  self->line++;
  nextchar(self); 
}

static void nexthex(Lexer* self) {
	assert(!"not implemented");
}

static void nextname(Lexer* self) {
  for (;;) {
    if (self->ch == '\0') {
      return;
    }
    else if (isalnum(self->ch)) {
      tokenappend(self, self->ch);
      return;
    }
    else {
      return;
    }
  }  
}

static void nextnumber(Lexer* self) {
  assert(!"not implemented");
}

static int checknext(Lexer* self, char expect) {
  if (*(self->pos+1) == expect) { 
    nextchar(self);
    return 1; 
  }
  else { return 0; }
}

static void nextstring(Lexer* self) {
  char const quote = self->ch;
  nextchar(self); /* skip quote */
  self->tokendata = self->pos;

  for (;;) {
    switch (self->ch) {
    case '\0': error(self, "unterminated string"); break;
    case '\n': case '\r': error(self, "newline in string"); break;
    case '\\': 
      nextchar(self);
      switch (self->ch) {
      case 'a': tokenappend(self, '\a'); break;
      case 'b': tokenappend(self, '\b'); break;
      case 'f': tokenappend(self, '\f'); break;
      case 'n': tokenappend(self, '\n'); break;
      case 'r': tokenappend(self, '\r'); break;
      case 't': tokenappend(self, '\t'); break;
      case 'v': tokenappend(self, '\v'); break;
      case 'x': nexthex(self); break;
      case '\\': break; /* skip */
      case '"': case '\'': tokenappend(self, self->ch); break;
      default:
        error(self, "invalid escape sequence"); break;
      }
    default:
      if (checknext(self, quote)) {
        return; /* string terminated */
      } 
      else {
        tokenappend(self, self->ch); 
        break;
      }
    }
  }
}

Lexer* Lexer_new(MemPool* pool) {
  Lexer* self = MemPool_alloc(pool, sizeof(Lexer));
  self->pos = 0;
  self->ch = 0;
  self->line = 0;
  self->column = 0;
  return self;
}

void Lexer_setpos(Lexer* self, char* data) {
  self->pos = data;
  self->ch = *data;
  self->line = 1;
  self->column = 1;
}

TokenType Lexer_nexttoken(Lexer* self) {
  for (;;) {
    switch (self->ch) {
    case '\0': return TT_EOF;
    case ':': nextchar(self); return ':';
    case '+': nextchar(self); return '+';
    case '*': nextchar(self); return '*';
    case '/': nextchar(self); return '/';
    case '|': nextchar(self); return '|';
    case '&': nextchar(self); return '&';
    case '^': nextchar(self); return '^';
    case '%': nextchar(self); return '%';
    case '<': 
      nextchar(self); 
      if (checknext(self, '=')) { return TT_LE; }
      else { return '<'; }
    case '>': nextchar(self); 
      if (checknext(self, '=')) { return TT_GE; }
      else { return '>'; }
    case '=': 
      nextchar(self); 
      if (checknext(self, '=')) { return TT_EQ; }
      else { return '='; }
    case '!': 
      nextchar(self); 
      if (checknext(self, '=')) { return TT_NE; }
      else { return TT_ERR; }
    case '.':
      nextchar(self);
      if (checknext(self, '.')) { return TT_CONCAT; }
      else { return TT_CONCAT; }
    case '"': case '\'':
      nextstring(self);
      return TT_STRING; 
    case '\n': case '\r': 
      nextline(self); 
      break;
    case ' ': case '\f': case '\t': case '\v': 
      nextchar(self); 
      break;
    default:
      if (isalpha(self->ch)) { nextname(self); }	
      else if (isdigit(self->ch)) { nextnumber(self); }
      else { error(self, "invalid character"); }
      break;
    }

  }
  return TT_ERR;
}


