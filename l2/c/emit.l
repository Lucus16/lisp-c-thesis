; Flatten a tree of C text and hints into a single string.

; TODO: Split overlong lines into multiple lines.

; CONFIGURATION
(def indentation "\t")
(def newline "\n")

(defu uline uindent uspaced ulinetoken)

(def (indent level tail)
  (if (= level 0) tail
    (indent (dec level) (cons indentation tail))))

(def (cons-fixline head tail)
  (if (and (ispair tail) (= (car tail) ulinetoken))
    (cons head (cons newline (cdr tail)))
    (cons head tail)))

(def (flatten-spaced tree level tail) (if
    (no tree) tail
    (no (cdr tree)) (flatten-c (car tree) level tail)
    (flatten-c (car tree) level (cons-fixline " "
        (flatten-spaced (cdr tree) level tail)))))

(def (flatten-line tree level tail)
  (def tail (if (and (ispair tail) (= (car tail) ulinetoken))
      (cdr tail) tail))
  (cons ulinetoken (indent level
      (flatten-c tree level (cons newline tail)))))

(def (flatten-c tree level tail) (if
    (no tree) tail
    (or (isstr tree) (ischar tree)) (cons-fixline tree tail)
    (not (ispair tree)) (error (+ "Expected string, found: " (repr tree)))
    (= (car tree) uindent) (flatten-c (cdr tree) (inc level) tail)
    (= (car tree) uline) (flatten-line (cdr tree) level tail)
    (= (car tree) uspaced) (flatten-spaced (cdr tree) level tail)
    (flatten-c (car tree) level (flatten-c (cdr tree) level tail))))

(def (flatten tree)
  (def result (flatten-c tree 0 nil))
  (def result (if (= (car result) ulinetoken)
      (cdr result)
      result))
  (apply + result))

(ns
  (line . l) (cons uline l)
  (indent . l) (cons uindent l)
  (spaced . l) (cons uspaced l)
  (emitc x) (flatten (emit x))
  (printc x) (print (flatten (emit x))))
