[ title page with abstract and index ]

[ possibility: write thesis as a guide for the reader to follow to come to the
same conclusion. e.g.
'We (the writer and the reader) have identified' vs 'I have identified' ]

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
in addition, they take an environment parameter. Their result is not a piece of
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

[ In order to make all primitives available, the semantics of every part of the
language must be clear and simple, so that code that doesn't use safe
abstractions can still comply with the conventions relatively easily and thus
keep the program safe. ]

# Identified Issues

[ TODO: Introduce all concepts and issues in the introduction. ]

I have identified the following issues with the current state of the art of
programming languages:

- Abstraction is limited in most non-Lisp languages as they allow only specific
  abstractions, and no generic abstraction of arbitrary syntactical structures.
- Abstraction is further limited by the primitives a language chooses to expose.
- Languages that allow large amounts of abstraction suffer from performance
  penalties.
- Compilers are large, complicated programs with many interdependencies that
  make it hard to understand and modify.

# Goals

Based on the issues I have identified above, I have set the following goals for
this thesis. I want to build a programming language that:

- Can match Lisp in abstraction facilities.
- Can match C in primitives made available.
- Can match C in performance.
- Is easy to understand and easy to modify.

# The Solution

As my goals have been mostly defined in terms of C and Lisp, the most
straightforward thing to do would be to combine them in some way. This has
already been attempted several times, but I have found all of these attempts to
be lacking in some area.

The most obvious method of combination is to compile Lisp to C. In fact, after C
became an established programming language, this has been a fairly common
practice for beginning Lisp languages. The problem is that many Lisp concepts
don't match well onto C concepts in a performant manner. [ TODO: Expand ]

Another method I've seen a few times is to generate C code using Lisp. Rather
than transforming Lisp code to C code, the Lisp code is simply run in an
interpreter and its output is C code. This means Lisp concepts no longer need to
directly match C concepts. Instead, the Lisp code can generate C code of any
form in whatever way it sees fit. In order to make generating a C program not
too much of a hassle, a C code generation library is used.

[ Examples are cmera, TODO ]

By producing C code, we can be assured that we're capable of reaching C
performance, as well as having all its primitives available. By using Lisp to
generate this C code, we have available all its abstraction facilities, and
because we're not transforming the Lisp code to C code directly, we don't need
to make use of confusing macros for abstraction and can make use of the much
nicer fexprs instead.

However, all implementations of this idea I've seen so far are written in an
existing Lisp language, and these are often very large and complicated
languages. One of our goals is to build something that is easy to understand and
modify, something that is simple enough to be able to grasp it in its entirety
in a matter of hours. Using a large existing language means all used parts of
that language need to be learned first.

A better option is therefor to use a very small and simple Lisp. In order to
ensure it can be understood completely, we'll write it in C, which is already
required knowledge because it is our target language for code generation. At
this point, it can also serve as our primary example: Once we have a simple Lisp
interpreter in C and a simple code generation library in this Lisp, we can
generate the interpreter itself, coming full circle. On top of that, we can give
some simple examples of abstractions that this makes possible and that will
hopefully make our interpreter even clearer.

[ make all primitives available ]
[ keep it minimal ]
[ keep it simple ]
[ self hosting ]
[ avoid macros, prefer fexprs ]
[ keep it modular ]

# Implementation

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
- Split abstraction in layers, allowing programming at any level of abstraction
  in the same language
]

# References

[ TODO: bibtex ]

[cmera]: https://github.com/kiselgra/c-mera
