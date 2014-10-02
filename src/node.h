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

#ifndef NODE_H
#define NODE_H

#include "util.h"

typedef enum NodeType {
  NT_BLOCK,
  NT_VAR,
  NT_FUNCTION,
  NT_TYPE,
  NT_INTERFACE,
  NT_MODULE,
  NT_BINARYOP,
  NT_UNARYOP,
  NT_CALL,
  NT_NAME,
  NT_LITERAL,
  NT_FIELD,
  NT_ASSIGN,
} NodeType;

typedef struct Node {
  NodeType type; 
  SrcLoc srcloc;
  struct Node* parent;
} Node;

typedef struct Block {
  NodeType type; 
  SrcLoc srcloc;
  Node* parent;
  Node* node;
} Block;

typedef struct Name {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  char const* name;
} Name;

typedef struct Var {
  NodeType type; 
  SrcLoc srcloc;
  Node* parent;
  char const* name;
  Node* node;
} Var;

typedef struct Function {
  NodeType type; 
  SrcLoc srcloc;
  Node* parent;
  Name* argtype;
  Name* argname;
  Name* returntype;
  Block* block; 
} Function; 

typedef struct Type {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  Var* value;
} Type;

typedef struct Interface {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  Name* fieldtype;
  Name* fieldname;
} Interface;

typedef struct Module {
  NodeType type; 
  SrcLoc srcloc;
  Node* parent;
  Var* value;
} Module;

typedef struct BinaryOp {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  Node* left;
  Node* right;
} BinaryOp;

typedef struct UnaryOp {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  Node* node;
} UnaryOp;

typedef struct Call {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  Node* node;
  Node* arg;
} Call;

typedef struct Literal {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  char const* value;
} Literal;

typedef struct Field {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  Node* node;
  char const* name;
} Field;

typedef struct Assign {
  NodeType type;
  SrcLoc srcloc;
  Node* parent;
  Node* left;
  Node* right;
} Assign;

#endif
