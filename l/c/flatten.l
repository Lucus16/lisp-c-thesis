; Flatten annotated strings to a single string of C code

(include std)

; CONFIGURATION
(def indentation "\t")
(def newline "\n")

; FLATTENING C CODE TREE INTO STRING
; TODO: Handle overlong lines
; TODO: When lines are nested, put empty lines between the outer line and its
; neighbors
(def (step9 l) (if
    (no l) nil
    (= (car l) newline) (step9 (cdr l))
    (apply + l)))

(defu uline uindent uspaced ublock ulinetoken)

(def (indent' level tail)
  (if (= level 0) tail
    (indent' (dec level) (cons indentation tail))))

(def (line . l) (cons uline l))
(def (indent . l) (cons uindent l))
(def (spaced . l) (cons uspaced l))
(def (block . l) (cons ublock l))

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
  (cons ulinetoken (indent' level
      (flatten-c tree level (cons newline tail)))))

(def (flatten-c tree level tail) (if
    (no tree) tail
    (or (isstr tree) (ischar tree)) (cons-fixline tree tail)
    (not (ispair tree)) (error (+ "Expected string, found: " (repr tree)))
    (= (car tree) uindent) (flatten-c (cdr tree) (inc level) tail)
    (= (car tree) uline) (flatten-line (cdr tree) level tail)
    (= (car tree) uspaced) (flatten-spaced (cdr tree) level tail)
    (flatten-c (car tree) level (flatten-c (cdr tree) level tail))))

(def (step8 tree)
  (def result (flatten-c tree 0 nil))
  (if (= (car result) ulinetoken) (cdr result) result))

(def (parenthesize x) (list "(" x ")"))
(def (sepby sep l)
  (if (no l) nil
    (no (cdr l)) l
    (cons (car l) (cons sep (sepby sep (cdr l))))))

(def (commalist' l)
  (if (no l) nil
    (no (cdr l)) (list (car l))
    (cons (list (car l) ",") (commalist' (cdr l)))))

(def (commalist l)
  (cons uspaced (commalist' l)))

(def (cflatten x) (step9 (step8 x)))

(export line indent spaced block commalist parenthesize cflatten)
