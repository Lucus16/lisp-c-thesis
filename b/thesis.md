\title{All Primitives Available}
\date{\today}
\author{Lars Jellema}
\maketitle

# Abstract

[ TODO ]

# Introduction

Most programming languages are Turing-complete, which means at some level, they
are equivalent to each other. Any of these languages can emulate any other and
thus solve the same problems that the others can solve. Of course, this does
come paired with some run time overhead. Usually, these problems can also be
translated more efficiently, by dropping the emulation layer. If speed were the
only thing that mattered, only a few of the fastest languages would be in common
use. Instead, we have hundreds of languages in common use. That's because
something else is more important than speed: Abstraction.

Programming languages are generally selected for a problem based on whether they
provide the right abstractions for it. Again, due to Turing-completeness, any
abstraction can be emulated at run time, at the cost of speed. The value of a
programming language is then in what abstractions it implements efficiently. So
one would expect that the most common languages are the ones with the best
abstractions.

This is true to some extent. For example, C++, one of the most common languages,
is known for being fast as well as offering many kinds of abstractions. In
addition to functions, classes, namespaces and many other abstractions, it also
has templates, which allow you to parameterize the types in these abstractions.
Templates are even (accidentally) Turing-complete. But despite all that, there
are still many other languages commonly in use, so it's clearly still lacking
something.

There is one family of languages that is often called one of the best in terms
of abstraction facilities: The family of Lisp-like languages. I'll use "Lisp" to
refer to this family from here on. There are many variants, but they are all
based on the same principles. One of those is that they often have
Turing-completeness at compile time, intentionally. Lisp helps me explain a
number of concepts, so I'll give an introduction to it in the next section. Feel
free to skip it if you already know how Lisp works.

# Introduction to Lisp

The first principle that Lisp is based on is that it has a very simple syntax:
All data consists of S-expressions and these are either a literal value, like
`3`, a symbol, like `append`, or a list of nested S-expressions. Lists are
denoted by surrounding the list with parentheses and separating its elements
with spaces. An example S-expression would be:

```
(symbol (another list) "a string" () 1337)
```

This S-expression is a list of five elements: First a symbol, then a list of two
symbols, then a string, an empty list and a number. Lists are used as the
primary means of composing data and this is where Lisp derives its name from:
LISt Processor. The empty list is usually called `nil` and is often used to
represent the absence of a useful value, like `null` does in other languages.
Symbols are unique, named items that allow for symbolic expressions, which is a
somewhat more abstract way of thinking than using concrete values. Symbols do
not need to consist of just alphanumeric characters, symbols like `+` and `<=`
are valid as well.

In Lisp, all code also consists of S-expressions: Symbols represent names for
functions and values, while lists represent function calls and expressions. Any
S-expression can be evaluated: A symbol is evaluated by looking it up in some
sort of environment and this results in a value which is said to be bound to it.
Function bind their arguments to their respective parameters when called, and
then evaluate their body in this new environment. Lists are evaluated by first
evaluating their first element and then proceeding based on the result. Most of
the time, that result will be a function, in which case the remaining elements
will also be evaluated and passed to it as arguments. There are also some
applicatives that don't necessarily evaluate the remaining elements. One of them
is usually called `quote` and it returns its argument unevaluated. This allows
us to pass literal lists and symbols to functions that normally evaluate all
arguments. All values that aren't lists or symbols evaluate to themselves, so
they don't need to be quoted.

Because Lisp uses the same types and syntax for both code and data,

```
(defmacro (defun name params body)
  (quasiquote (def (unquote name)
      (fn (unquote params) (unquote body)))))
```

```
(devau (defun name params body) env
  (bind env name (mkfn params env body)))
```
