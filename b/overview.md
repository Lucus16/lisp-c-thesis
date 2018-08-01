Title: All Primitives Available

# Introduction
Programming is about abstraction
Still need new languages

# Introduction to Lisp
Lisp syntax is just an ast
Example: give C code with corresponding ast
Lisp programs are data
Example: program that constructs another program
Lisp macros transform programs
Example: defun
Lisp has syntactic abstraction
Lisp macros are a pain

# Problem
Need for macros indicates need for Turing-completeness at compile time
Lisps primitives are its syntax and nothing below it
Syntactic abstraction is a hack
Syntactic abstraction is not good enough
Key insight: primitives are as important as abstractions

# Solution
All Primitives Available
Fexprs are an exposed primitive
Example: defun as fexpr
Fexprs are clearer than macros
Fexprs are just syntactic sugar
Quote no longer needed
Functions can abstract anything
Don't hide logic in fexprs
Example: defun in terms of mkfn, mkfn in terms of mktype

# Considerations
All Primitives Available forces you to make internals clean
Simple
Orthogonal
Choose abstractions bottom up

# Implementation
Building on Lisp
Using Assembly as a first abstraction layer
Using C as a first abstraction layer
Existing implementations have large and complicated dependencies
Self hosting and its own example

# Conclusions
