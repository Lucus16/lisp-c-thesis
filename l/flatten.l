; Flatten a tree of tokens to a single string
;
; First, flatten and convert line, spaced, block and indent to tokens
; Then, remove redundant tokens
; Then, concatenate everything

(include std)

; CONFIGURATION
(def indentation "\t")
(def newline "\n")
(def emptyline (+ newline newline))

(def (doindent level tail)
  (if (= level 0) tail
    (doindent (dec level) (cons indentation tail))))

(def (line . l) (cons flatten-line l))
(def (spaced . l) (cons flatten-spaced l))
(def (indent . l) (cons flatten-indent l))
(def (block . l) (cons flatten-block l))

(def (flatten-line tree level tail)
  (cons newline (doindent level (flatten-c tree level (cons newline tail)))))

(def (flatten-spaced tree level tail) (if
    (no tree) tail
    (no (cdr tree)) (flatten-c (car tree) level tail)
    (flatten-c (car tree) level (cons " "
        (flatten-spaced (cdr tree) level tail)))))

(def (flatten-indent tree level tail)
  (flatten-c tree (inc level) tail))

(def (flatten-block tree level tail)
  

(def (flatten-c tree level tail) (if
    (no tree) tail
    (or (isstr tree) (ischar tree)) (cons tree tail)
    (not (ispair tree)) (error (+ "Expected string, found: " (repr tree)))
    (isfn (car tree)) ((car tree) (cdr tree) level tail)
    (flatten-c (car tree) level (flatten-c (cdr tree) level tail))))

(def (step9 l) (apply + l))

; FLATTENING C CODE TREE INTO STRING
; TODO: Handle overlong lines
; TODO: When lines are nested, put empty lines between the outer line and its
; neighbors

(def (step8 tree)
  (def result (flatten-c tree 0 nil))
  (if (= (car result) ulinetoken) (cdr result) result))

(def (parenthesize x) (list "(" x ")"))

(def (sepby sep l)
  (if (no l) nil
    (no (cdr l)) l
    (cons (car l) (cons sep (sepby sep (cdr l))))))

(def (printc x) ((defaults print) (step9 (step8 (emit x)))))

(export line spaced block parenthesize flatten-c printc)
