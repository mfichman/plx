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
#include "stdio.h"

#define TOKENLENMAX 4096

struct Lexer {
  char* pos;
  char ch;
  int line;
  int column;
  int tokenlen; 
  char tokendata[TOKENLENMAX];
};

static void printtoken(Lexer* self, TokenType token) {
  switch (token) {
  case TT_ERR: printf("err"); break;
  case TT_EOF: printf("eof"); break;
  case TT_NE: printf("!="); break;
  case TT_GE: printf(">="); break;
  case TT_LE: printf("<="); break;
  case TT_EQ: printf("=="); break;
  case TT_STRING: printf("string(%.*s)", self->tokenlen, self->tokendata); break;
  case TT_CONCAT: printf("'..'"); break;
  case TT_NAME: printf("name(%.*s)", self->tokenlen, self->tokendata); break;
  case TT_NUMBER: printf("number(%.*s)", self->tokenlen, self->tokendata); break;
  default: printf("'%c'", token); break;
  }
}

static void error(Lexer* self, char const* msg) {
  fprintf(stderr, "%d:%d: %s\n", self->line, self->column, msg);
}

static void savechar(Lexer* self, char ch) {
  assert(self->tokenlen < (TOKENLENMAX-1));
  self->tokendata[self->tokenlen] = ch;
  self->tokenlen++;
}

static void nextchar(Lexer* self) {
  self->pos++;
  self->ch = *self->pos;
  self->column++;
}

static void nextcharsave(Lexer* self, char ch) {
  savechar(self, ch);
  nextchar(self);
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
      nextcharsave(self, self->ch);
    }
    else {
      return;
    }
  }  
}

static void nextnumber(Lexer* self) {
  int hasdecimal = 0;
  for (;;) {
    if (self->ch == '\0') {
      return;
    }
    else if (isdigit(self->ch)) {
      nextcharsave(self, self->ch);
    }
    else if (self->ch == '.' && !hasdecimal) {
      nextcharsave(self, self->ch);
      hasdecimal = 1; 
    }
    else {
      return;
    }
  }
}

static int checknext(Lexer* self, char expect) {
  if (*self->pos == expect) { 
    nextchar(self);
    return 1; 
  }
  else { return 0; }
}

static void nextcomment(Lexer* self) {
  nextchar(self);
  for (;;) {
    switch (self->ch) {
    case '\n': case '\r': break;
    default: nextcharsave(self, self->ch); break;
    }
  }
}

static void nextstring(Lexer* self) {
  char const quote = self->ch;
  nextchar(self); /* skip quote */

  while (self->ch != quote) {
    switch (self->ch) {
    case '\0': 
      error(self, "unterminated string"); 
      nextchar(self);
      return;
    case '\n': case '\r': 
      error(self, "newline in string"); 
      nextchar(self);
      return;
    case '\\': 
      nextchar(self);
      switch (self->ch) {
      case 'a': nextcharsave(self, '\a'); break;
      case 'b': nextcharsave(self, '\b'); break;
      case 'f': nextcharsave(self, '\f'); break;
      case 'n': nextcharsave(self, '\n'); break;
      case 'r': nextcharsave(self, '\r'); break;
      case 't': nextcharsave(self, '\t'); break;
      case 'v': nextcharsave(self, '\v'); break;
      case 'x': nexthex(self); break;
      case '\\': break; /* skip */
      case '"': case '\'': nextcharsave(self, self->ch); break;
      default:
        error(self, "invalid escape sequence"); 
        nextchar(self);
        return;
      }
    default:
      if (self->ch == quote) {
        return; /* string terminated */
      } 
      else {
        nextcharsave(self, self->ch); /* string char */
        break;
      }
    }
  }
  nextchar(self);
}

Lexer* Lexer_new(MemPool* pool) {
  Lexer* self = MemPool_alloc(pool, sizeof(Lexer));
  self->pos = 0;
  self->ch = 0;
  self->line = 0;
  self->column = 0;
  self->tokenlen = 0;
  return self;
}

void Lexer_setpos(Lexer* self, char* data) {
  self->pos = data;
  self->ch = *data;
  self->line = 1;
  self->column = 1;
}

TokenType Lexer_debugtoken(Lexer* self) {
  /* Same as Lexer_nexttoken, but prints the tokens as it reads */
  TokenType type = Lexer_nexttoken(self);
  printtoken(self, type);
  return type;
}

TokenType Lexer_nexttoken(Lexer* self) {
  self->tokenlen = 0;

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
    case '(': nextchar(self); return '(';
    case ')': nextchar(self); return ')';
    case '{': nextchar(self); return '{';
    case '}': nextchar(self); return '}';
    case '#': nextcomment(self); return TT_COMMENT;
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
      else if (isdigit(self->ch)) { 
        savechar(self, '.');
        nextnumber(self);
        return TT_NUMBER;  
      }
      else { return '.'; }
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
      if (isalpha(self->ch)) { 
        nextname(self); 
        /* FIXME: Check for reserved words & return correct token
         * Need to add an interned string table for all parsed token data
         */
        return TT_NAME; 
      }	
      else if (isdigit(self->ch)) {   
        nextnumber(self); 
        return TT_NUMBER; 
      }
      else {
        error(self, "invalid character"); 
        nextchar(self);
      }
      break;
    }

  }
  return TT_ERR;
}

