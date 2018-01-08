[ title page with abstract and index ]

# Introduction

[ GOALS:
- Minimal, easy to understand and easy to hack. Avoid hard-to-understand macros.
- A minimal Lisp interpreter written in C. One on one correspondence between C
  and Lisp functions.
- A minimal C code generation library written in Lisp. One on one correspondence
  between C syntax and S-expressions.
- A minimal Lisp interpreter in C, generated from Lisp code.
- Simple examples of abstractions and analyses made possible.
- Self-hosting, depends only on a C compiler and a previous version of the
  interpreter, not on any C code as that is generated.
]

# Simplicity

[ TODO: EXPAND
- Prefer F-expressions over macros.
]

# Interpreter

# C Generator

# Generated Interpreter

# Example Abstractions

# Related Work

[ TODO: Go through bookmarks ]

[ TODO: EXPAND
- [cmera] is another project that has similarly created a mapping from
  S-expressions to C syntax in order to use the power of Lisp for abstractions.
  They use Common Lisp and macros however, which I find much harder to read, as
  well as making self-hosting impossible.
]

# Conclusions

# Future Work

[ TODO: EXPAND
- Better self-hosting: Generate machine code, either from the C S-expressions or
  from another abstractions layer. A better step might be to build a simple VM.
- More and better abstractions and static code analyses. Make it possible to
  analyse existing C code as well by building a parser.
- An improved Lisp version, preferably in the direction of Clojure.
]

# References

[ TODO: bibtex ]

[cmera]: https://github.com/kiselgra/c-mera
