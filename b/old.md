[ TODO: NEED MORE EXAMPLES ]

\title{A Better Programming Language}
\date{\today}
\author{Lars Jellema}
\maketitle

# Abstract

Thousands of programming languages exist and many of them are in common use.
This makes it clear that no language has managed to become universal. This
makes sense, as designing a language that works well in all possible use cases
is a very difficult task. I consider what it would take to build such a language
and I show how a first attempt can be made fairly easily based on existing
technologies.

# A Better Language

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
either a literal value, like `3`, a name, like `append`, or a list of nested
S-expressions. Lists are denoted by surrounding the list with parentheses and
separating the elements with spaces. An example S-expression would be:

```
(symbol (another list) "a string" () 1337)
```

This S-expression is a list of five elements: First a symbol, then a list of two
elements with two symbols, then a string, an empty list and a number. Symbols
are used as the names of functions and variables. Note that a symbol does not
need to consist of alphanumeric characters, `+` and `<=` are symbols too. When
looking up a symbol in the environment results in a value v, we say the symbol
is bound to v at that point. The empty list is usually called nil and is also
often used to represent the absence of a useful value, like null does in other
languages.

Lists are evaluated by first evaluating their first element and then choosing
what to do with the remainder of the list based on this first element. Names are
evaluated by looking them up in some sort of environment and values simply
return themselves when they are evaluated. The primary data structure used when
programming in Lisp, is also a list, with fast access to its first element and
to the remainder of the list. It also has corresponding types for names and
obviously, values. This means programs and program snippets can themselves be
expressed as data.

```
(list (quote list) 3 "A string")
```

When evaluated, this S-expression does the following: It starts by evaluating
the first element of the list, which is `list`. This is looked up in the
environment and results in a function that is then applied to the remaining list
elements. The function in this case constructs a list of its arguments. However,
functions always evaluate their arguments first. `(quote list)` is evaluated
next, but `quote` is special: Instead of evaluating its argument, it returns its
argument unevaluated. That means the result is the symbol `list`. Because this
argument has now been evaluated, it won't be looked up in the environment and
the result is thus a symbol, not the function it is bound to. The next two
arguments return themselves when evaluated because they are simple values. The
result of the entire expression is `(list 3 "A string")`, a piece of data that
is also valid code and could be evaluated again if desired.

Lisp uses this ability to express programs as data in order to make arbitrary
abstractions possible. It does this with macros: Functions that take as input an
S-expression representing some abstraction and that give as output another
S-expression, a concrete implementation meant to take the place of the
abstraction. At compile-time, all such macros are run and all abstractions are
turned into concrete code, which the Lisp compiler understands. Here is an
example macro:

[ TODO: Use the following example instead, which highlights the value of a
primitive bind:

```
(defmacro (defun name params body)
  (quasiquote (def (unquote name)
      (fn (unquote params) (unquote body)))))
```

```
(devau (defun name params body) env
  (bind env name (mkfn params env body)))
```

]

```
(defmacro (if condition then else)
  (quasiquote (cond
      ((unqoute condition) (unqoute then))
	  (true (unqoute else)))))
```

Here, `defmacro` defines a macro which will be evaluated at compile-time. The
next list gives the name of the macro, `if` and the parameters, `condition`,
`then` and `else`. These will be bound to the three arguments that `if` receives
at compile-time, because this is a macro, these arguments will not be evaluated
before being passed as happens for functions. The body of the macro consists of
a `quasiquote` with some nested `unquote`s. This results in the expression in
the `quasiquote` unevaluated like in `quote`, except for the parts in `unquote`.
Thus, the symbols that are wrapped in `unquote` are looked up, which will result
in the arguments that were passed to the macro. For example, if the following
S-expression appears somewhere in the source code:

```
(if (no xs) nil (cdr xs))
```

Then during compilation, it will be replaced with:

```
(cond
  ((no xs) nil)
  (true (cdr xs)))
```

`cond` takes lists as argument and starts by evaluating the first element of the
first list. If that gives true, the result of the `cond` will be evaluating the
rest of that list. If false, it will try the next list. In most Lisps, one of
`cond` and `if` is defined in terms of the other.

## Limits of Lisp

Macros have a number of issues however: It is not trivial to write safe macros,
that work regardless of the context in which they are used. It is also often
quite hard to debug macros, or even understand what they do, because they
interleave two very similar looking S-expressions of which one runs at
compile time, in a completely different environment from the other, which runs
at run time. The `if` macro shown earlier is unsafe: If `cond` is redefined to
something else at the place where an `if` macro is used, it may do all kinds of
unexpected things, even though the call to `if` gives no indication that it
depends on what `cond` is bound to.

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
code that can be compiled, but rather a result value, similar to a function.
This means they need to run at run time, because the environment wouldn't be
available otherwise. Therefor, they don't allow the same compile-time
optimizations as macros do, but they allow all the same abstractions, in clearer
and more readable code.

For example, here is the same definition of `if` as an F-expression:

```
(devau (if condition then else) env
  (cond
    ((eval condition env) (eval then env))
    (true (eval else env))))
```

In this case, the environment at the definition of `if` is captured as in a
closure. This means `cond` is resolved to the value it had when `if` was defined
and not to the value it has when `if` is called. This restores lexical scope
even for syntactical abstractions.

[ syntactical abstraction is necessary ]

[ all primitives available is necessary ]

[ choosing abstractions top-down as in functional programming causes ugly
abstraction layers which need to translate concepts that don't translate well.
This ugliness will eventually show up to the user of the language, it cannot be
fully hidden. ]

[ kiss ]

[ describe current issues ]

[ pros and cons of dynamic languages ]

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
  make them hard to understand and modify.

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

By producing C code, we can be assured that we're capable of reaching C
performance, as well as having all its primitives available. By using Lisp to
generate this C code, we have available all its abstraction facilities, and
because we're not transforming the Lisp code to C code directly, we don't need
to make use of confusing macros for abstraction and can make use of the much
nicer fexprs instead.

[ Examples are cmera, TODO ]

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

At this point, it would be good to consider if there is still a need for Lisp:
If the target is C, then the advantage of Lisp being good at generating its own
code is lost. C is indeed a little harder to generate than Lisp, which is why
the last translation step should be from simple Lisp data structures to actual C
code. That way, most code manipulation will happen on simple data structures.
This can still be done in other languages however. The value of Lisp here lies
in that it can also abstract its own programs, and because performance isn't as
much of an issue in what is essentially the "compiler", the more elegant
F-expressions can be used.

In summary, the central idea of this thesis is to build a simple Lisp
interpreter and a simple C code generation library in that Lisp language, and
then to use that library to generate the C code of the Lisp interpreter. While
doing this it is important to constantly make sure that things stay simple and
modular, to make it easy to play around and adapt this language for other
purposes.

# Implementation

# Related Work

[ TODO: Go through bookmarks ]

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
