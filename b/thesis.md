- all programming languages limited, there are always domains where it is
  necessary to switch to another language. After all, different problem domains
  often have different relevant abstractions and sometimes those are
  incompatible. However, they often also have some abstractions that are just
  reimplementations of the same thing.
- A programming language is a language and it should offer generic abstractions.
  In natural language, we can talk about parts of that language itself, meta.
- Current programming languages allow you to express any computation, but
  sometimes with significant runtime overhead. They allow you to express any
  abstraction using just functions, but with significant runtime overhead. Want
  to efficiently express any binary and any abstraction at compile time.
  Essentially want functions to generate binary.
- When you're writing a complicated macro, it is generally because you want to
  express an abstraction at compile time, and it is complicated enough that you
  need a full programming language to describe it.
- needed
	- syntactical abstractions
		- Fexprs allow syntactical abstraction but should only be used for that.
		  Logical abstraction should be implemented in functions.
	- all primitives available
		- abstraction over syntax shouldn't be necessary. In almost all cases,
		  what you really want, is to abstract over the underlying
		  implementations of the syntax.
		- for every primitive, create a single underlying function that
		  implements the logic and any number of front-end primitives / fexprs /
		  functions that only wrap the original. This makes sure further
		  abstraction can always be expressed in terms of the logical function
		  while regular use can make use of syntactic sugar.
		- At the lowest level, this means being able to generate any instruction
		  for the processor. However, it is also needed at higher levels, in
		  order to integrate any new ideas at the appropriate level of
		  abstraction.
		- Examples:
			- C allows all low level primitives: It allows machine code for a
			  function to be given directly and it also allows assembly to be
			  written inline, however, it does not allow good compile-time
			  abstraction of these.
			- Lisp allows the highest level primitives: You can abstract over
			  any piece of syntax, but you can't easily abstract over the
			  underlying concepts that are used to implement that syntax.
	- build abstractions bottom-up
		- top-down abstractions cause ugly / mismatched abstraction layers in
		  the middle which causes ugly semantics.
	- keep it simple and orthogonal
- possible implementation
	- By generating C code using Lisp
	- By generating assembly using Lisp

```
(defmacro (defun name params body)
  (quasiquote (def (unquote name)
      (fn (unquote params) (unquote body)))))
```

```
(devau (defun name params body) env
  (bind env name (mkfn params env body)))
```

\title{All Primitives Available}
\date{\today}
\author{Lars Jellema}
\maketitle

# Abstract

[ TODO

As most programming languages are essentially Turing-complete, programming
language design may seem like a solved problem. Many new languages are still
developed, but their differences are fairly superficial: They just introduce
some new abstractions to solve yet another problem commenly encountered during
development. I have tried many of these languages and come to the conclusion
that something deeper is missing: The ability to add such abstractions within
the language itself. I describe what I'm missing exactly, what it would enable,
and how to implement it by making all primitives available.

Right now, if the abstractions you want don't all fit in the same language, you
can either give up on some of them or build a new language. New languages are
inevitably a lot like existing languages, otherwise they would be completely
unintelligable. However, all of these similar features need to be basically
reimplemented from scratch, which makes building a new language a lot of work.
By building a modular language, where abstractions on every level can be built
upon, it can be made vastly easier to build new similar languages or extend such
languages with new abstractions.

Turing completeness hasn't stopped the need for new languages, more abstractions
are needed. Turing-completeness only allows arbitrary abstractions at run time
and we can't afford run time overhead. Many abstractions don't fit within
existing languages, so new languages need to be built. Building new languages is
a time consuming process, many parts of existing languages need to be
reimplemented. Why can't new abstractions be added to existing languages?
Languages aren't built for it, they may have a lot of specific abstractions
available, like C++ and Rust, but often don't have facilities to make generic
abstractions.

]

# Introduction
