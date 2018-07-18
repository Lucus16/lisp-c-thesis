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

```
(defmacro (defun name params body)
  (quasiquote (def (unquote name)
      (fn (unquote params) (unquote body)))))
```

```
(devau (defun name params body) env
  (bind env name (mkfn params env body)))
```
