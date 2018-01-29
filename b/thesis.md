[ title page with abstract and index ]

# Abstract

# Introduction

Thousands of programming languages exist nowadays, yet all of them have various
kinds of problems. New programming languages are created regularly. Some
programming languages aim to be very generic, while others are only useful in
some specific areas. Some languages introduce new features and some make
features mainstream. However, it is often very hard or impossible to introduce
such new features to existing programming languages.

C++ is an example of a language that tries to add many such new features. The
result is often that it ends up with ugly and confusing syntax and semantics.
This happens because C++ has decades of legacy features that it needs to stay
backwards-compatible with and because many of these features and design choices
are tightly interwoven and strongly dependent on each other. This makes it hard
to introduce new features, as it would require changes in many dependent parts
of the language. Thus, many features can not be implemented as elegantly in C++
as in new languages, and some things can not be implemented at all.

## Lisp

When talking about extensible languages, Lisp-like languages are often seen as
the forefront in this area. From this point, I will use Lisp to refer to the
family of Lisp-like languages. Lisp achieves impressive extensibility by making
their syntax extremely simple: All code consists of S-expressions and these are
either a literal value, like `3`, a name, like `append`, or a list of more
S-expressions.

Lists are evaluated by first evaluating their first element and then choosing
what to do with the remainder of the list based on this first element. Names are
evaluated by looking them up in some sort of environment and values simply
return themselves when they are evaluated. The primary data structure used when
programming in Lisp, is also a list, with fast access to its first element and
to the remainder of the list. It also has corresponding types for names and
obviously, values. This means programs and program snippets can themselves be
expressed as data.

Lisp uses this ability to express programs as data in order to make arbitrary
abstractions possible. It does this with macros: Functions that take as input an
S-expression representing some abstraction and that give as output another
S-expression, a concrete implementation meant to take the place of the
abstraction. At compile-time, all such macros are run and all abstractions are
turned into concrete code, which the Lisp compiler understands.

## Limits of Lisp

Macros have a number of issues however: It is not trivial to write safe macros,
that work regardless of the context in which they are used. It is also often
quite hard to debug macros, or even understand what they do, because they
interleave two very similarly looking S-expressions of which one runs at
compile time, in a completely different environment from the other, which runs
at run time.

While Lisp allows arbitrary abstractions of its primitives by using macros, it
is still limited by the primitives it makes available. For example, Clojure is
fundamentally limited by the capabilities of the JVM, it cannot make
abstractions that aren't based on the primitives the JVM provides. This means it
cannot support tail call optimization, has a significant startup time, and
cannot be used for low-level programming.

Some other Lisp languages do not run on a VM, but nonetheless do not expose all
primitives their implementations have access to. The reason for this is usually
simple: In order to generate efficient code, they want to structure their code
in a specific way so that they don't need to constantly check assumptions.
Allowing access to arbitrary primitives risks breaking that structure and thus
generating broken code.

However, exposing all primitives also has an advantage: There is no longer a
need for macros which build S-expressions which compile down to the right
primitives if you can call the primitives directly. This is how F-expressions
work: They take as parameters pieces of unevaluated code, just like macros, and
in addition, they take an environment paramater. Their result is not a piece of
code that can be compiled, but rather a result value, like a function. This
means they need to run at run time, because the environment wouldn't be
available otherwise. Therefor, they don't allow the same compile-time
optimizations as macros do, but they allow all the same abstractions, in clearer
and more readable code.

[ describe current issues ]

[ The following projects do allow access to all primitives: TODO ]

[ pros and cons of dynamic languages ]

[ the right balance between static and dynamic: using optional static analysis
and making primitives fit well in such analysis ]

[ macros are confusing, having language primitives available instead is better ]

[ In order to make all primitives available, the semantics of every part of the
language must be clear and simple, so that code that doesn't use safe
abstractions can still comply with the conventions relatively easily and thus
keep the program safe. ]

# Identified Issues

I have identified the following issues with the current state of the art of
programming languages:

- Abstraction is limited in most non-Lisp languages as they allow only specific
  abstractions, and no generic abstraction of arbitrary syntactical structures.
- Abstraction is further limited by the primitives a language chooses to expose.
- Languages that allow large amounts of abstraction suffer from performance
  penalties.
- Languages are often limited by the languages or frameworks they are built
  upon.

# Goals

[ GOALS:
- Combine the performance of C with the power of Lisp
- Modular, implementations of different features and data structures not
  strongly dependant on each other.
- All primitives available.
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
