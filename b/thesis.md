\title{All Primitives Available}
\date{\today}
\author{Lars Jellema}
\maketitle

# Abstract

# Introduction

```
(defmacro (defun name params body)
  (quasiquote (def (unquote name)
      (fn (unquote params) (unquote body)))))
```

```
(devau (defun name params body) env
  (bind env name (mkfn params env body)))
```
