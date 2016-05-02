# plx

Programming language X (I can't think of a good name at the moment).

This language is based on a simple idea: a "static" Lua. The idea is to use type inference and structural typing. 
When you declare a type using Lua's table constructor syntax, i.e.:

```
x = {
  a = 1,
  b = 2,
}
```

Then the type of "x"  corresponds roughly to a C declaration:

```
struct TypeOfX {
   double a;
   double b;
}
```

In this way, the compiler can statically warn you if you try to access `x.c`, for example.

The type each function arg accepts is determined by looking at how the arg is used, much
like C++ templates. For example:

```
function add(x, y) 
   return x.a + y.b
end
```

This function assumes that `x` has a member `a` that's a number, and like wise for `y.b`.
Multiple implementations of `add` can be compiled when a type os "generic" and it cannot
be inferred. For example:

```
function list(a) 
   return {a}
end
```

In this case, `list` is equivalent to the following C++ function:

```
template <typename T>
std::vector<T> list(T t) {
   std::vector<T> out = {t};
   return out;
}
```

