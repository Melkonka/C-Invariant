# C++ Invariant lib for primitive types
Warning: Don't use this lib. It's untested, unfinished etc.

Developed with Clang 3.8 Windows version.

C++17 features (like Fold Expressions) are used in the lib. In theory it should work:

Clang 3.6+

GCC 6+

MSVC: The latest msvc doesn't support fold expressions, so it won't work with it.

This lib is aim to provide a zero overhead cross-platform way to write self documenting code by expressing invariants with types. It's also aimed to optimize away redundant checks.

For example, instead of this:

 void f (int i) {assert (i >= 0 && i <= 255);}
 
Write this:

 void f (bounded_i<0, 255> i) {}
 
The latter ensure our invariant through type information, thus it can be used to eliminate redundant checks.
It's also way better to see the precondition in a declaration than in the definition.

Most things are customizable:

The way to fail is defined by a FailPolicy class, which should only provide a static assert() function.

The invariants are policies with check(T) functions, a type will be able to handle multiple invariants.

It's only working with primitive types, because i don't know any way to ensure the invariant on user defined types.
