Title: All Primitives Available

# Introduction
Programming languages suck
C++ has limited abstraction

[ what is my thesis trying to fix? Express any binary. ]

What if I want something that doesn't quite fit the model of the language I'm
using? I'm basically on my own, some languages might allow me to insert
arbitrary assembly, but I'd have to recreate much of the plumbing that makes the
language work. Extending the compiler is usually even harder.

# Lisp
Lisp has simple syntax
Lisp programs are data
Lisp macros transform programs
Lisp has syntactic, unlimited abstraction

# Limits of Lisp
Lisp macros are a pain
Clojure has limited primitives
Lisps have limited primitives

# All Primitives Available
All primitives available
Fexprs are an exposed primitive
Fexprs are better than macros
Fexprs are just syntactic sugar
Quote no longer needed
Functions can abstract anything
Don't hide logic in fexprs

# Limit of F-Expressions
Fexprs have run time overhead
Fexprs can't express arbitrary machine code

# Compile time F-expressions
Aren't these just macros
Compile time everything

# Goals

# The Solution
Combine C and Lisp
Compiling Lisp to C sucks [incomplete]
Generate C using Lisp
has all the advantages
Existing implementations have large and complicated dependencies
Self hosting and its own example



Prefer C over Assembly in this thesis
