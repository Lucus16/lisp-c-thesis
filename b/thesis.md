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

```
(list (quote slice) "Hello, world!" 0 5)
```

When evaluated, this S-expresssion does the following: It starts by evaluating
the first element of the first list, which is the symbol `list`. Symbols are
looked up in the environment. In the standard environment, `list` resolves to a
function that returns a list of its arguments. Because it is a function, all the
arguments are first evaluated. The first argument is a list with the special
`quote`. It returns its argument unevaluated, so the result is the symbol
`slice`. The next three arguments are values which evaluate to themselves.
Finally, the list function builds the following list:

`(slice "Hello, world!" 0 5)`

This S-expression can also be evaluated: It would take a slice from the string
equal to `"Hello"`, and this shows the power of Lisp: It is very easy to write
programs that write programs. Lisp uses this ability in something called macros,
which are quite different from the macros used in C. Lisp macros are functions
that write parts of your program for you during compile time. They usually take
one or more pieces of code as arguments and return some new piece of code based
on them. The reason Lisp macros are so flexible is that they are Turing
complete, and that they work on something simple: The syntax of Lisp is chosen
to look just like its abstract syntax tree.

```
(defmacro (defun name params body)
  (quasiquote (def (unquote name)
      (lambda (unquote params) (unquote body)))))
```

Here, `defmacro` defines a macro which will be evaluated at compile-time. Its
first argument defines the name of the new macro, `defun`, and the names of its
parameters, `name`, `params`, and `body`. These will be bound to the three
arguments that any invocation of `defun` will receive. The body of the macro
consists of a `quasiquote` with some nested `unqoute`s. `quasiquote` returns its
code inside unevaluated, except for any nested `unquote`s, which are evaluated
in the same environment that the `quasiquote` expression is. `def` binds a name,
its first argument, to a value, its second argument. `lambda` returns a new
function that first binds its arguments to the parameters given in the first
argument of `lambda` and then executes its second argument in the new
environment. For example, `(defun double (x) (mul x 2))` would be transformed
into `(def double (lambda (x) (mul x 2)))`. In this case, the transformation is
quite simple, but it allows you to define functions just a little easier, and
defining functions is something you do a lot in Lisp.

# Syntactic Abstraction

The abstraction that macros offer is unlike other abstractions: It doesn't
abstract over some specific concepts in the language, but rather over its
syntax. This means macros can be used to abstract over any concept in the
language that has some corresponding syntax. This makes syntactic abstraction
quite powerful. It also has a number of drawbacks however. The syntax of a
language is often not designed to be abstracted over, even in Lisp. The macro
defined above can give some very unexpected behavior if `def` or `lambda` is
bound to something different. A call to the macro gives no indication that it
depends on those bindings however. Macros can also be quite confusing to read
because they interleave two or more S-expressions that are evaluated at
different times in different environments.

Macros were originally invented to allow programmers to write their own
abstractions. Instead of needing to extend the compiler for every specific kind
of abstraction they might want to use, they made it possible to describe these
abstractions purely in terms of the syntax of the language, something which they
were already used to. This solution has the advantage of being easy to implement
and learn, but it also has drawbacks. Not only do macros have difficulty dealing
with the different contexts, but they are also still limited to the concepts of
the language which have corresponding syntax. Many useful concepts that exist in
the compiler do not. An example of this is the intermediate representations that
most compilers use. Some concepts are best expressed in terms of those
intermediate representations, rather than in terms of the existing syntax in a
language.

# All Primitives Available

The real issue programming languages have is not the lack of abstractions.
Turing-completeness at compile time gives the maximum amount of abstraction
possible. What programming languages, including Lisp lack, is sufficient
primitives for those abstractions. Lisp offers all its highest level primitives,
in the form of its syntax, but offers very little beyond that. Just the syntax
is not enough for integrating a new feature with the language properly. C on the
other hand, offers the lowest level of primitives: Inline assembly or even
machine code. While you can build anything you want with these, you still need
to reimplement large parts of a C compiler if you want it to fit well in the
language. For a truly extensible language, with support for any compile time
abstraction, it is important to have primitives at all levels of the language
available.

```
(devau (defun name params body) env
  (bind env name (mkfn params env body)))
```
