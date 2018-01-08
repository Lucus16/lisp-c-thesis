; expression wrapping depends on indentation level

; Although we want to avoid tree manipulation when there are better options, for
; the last few steps, this is the best option:
; 6. Build ns tree
; 7. Build text tree with line, indent and list nodes
; 8. Flatten text tree and insert indentation and newlines
; 9. Call + to create single string object

; node:
;   part - part of C syntax
;   includes - list of libraries to include
; part:
;   emit - emit code
;   emith - emit header code

;(defc bool.c
;  (include constants.h meta.h uniq.h)
;  (deftype Bool Uniq)
;  (defn bool_new ((: b bool)) Bool
;    (meta_refer (if b BOOL_TRUE BOOL_FALSE))))

; CONFIGURATION
(def indentation "\t")
(def newline "\n")

(def (step9 l) (apply + l))

(defu uline uindent uspaced)

(def (indent' level tail)
  (if (= level 0) tail
    (indent' (dec level) (cons indentation tail))))

(def (step8a tree level tail) (if
    (no tree) tail
    (not (ispair tree)) (error (+ "Expected line, found: " tree))
    (= (car tree) uline) (indent' level (step8b (cdr tree) (cons newline tail)))
    (= (car tree) uindent) (step8a (cdr tree) (inc level) tail)
    (step8a (car tree) level (step8a (cdr tree) level tail))))

(def (step8b tree tail) (if
    (no tree) tail
    (isstr tree) (cons tree tail)
    (not (ispair tree)) (error (+ "Expected string, found: " tree))
    (step8b (car tree) (step8b (cdr tree) tail))))

(def (step8 tree) (step8a tree 0 nil))
(def (line . l) (cons uline l))
(def (indent . l) (cons uindent l))
(def (spaced . l) (cons uspaced l))

(def (parenthesize l) (cons "(" (+ l (list ")"))))
(def (sepby sep l)
  (if (no l) nil
    (no (cdr l)) l
    (cons (car l) (cons sep (sepby sep (cdr l))))))

(def (emit t) ((t form emit) t))

(devau (emitter body) outer (fn (inner) (eval body (+ outer inner))))

(defns C)
(defns forms)

(defns exprform
  emit (emitter ((form emit') args)))

(def (mkexpr (name . params))
  (def form (+ exprform (ns)))
  (bind forms name form)
  (bind C name (fn args (ns
        form form
        args (match (ns) params args)))))

((vau defs (each defs mkexpr))
  (id ident)
  (lit literal)
  (index ptr expr)
  (call func . args)
  (dot expr name)
  (arrow expr name)
  (post-inc expr)
  (post-dec expr)
  (pre-inc expr)
  (pre-dec expr)
  (addr expr)
  (deref expr)
  (pos expr)
  (neg expr)
  (not expr)
  (bnot expr)
  (sizeof expr)
  (alignof expr)
  (cast type expr)
  (mul . args)
  (div . args)
  (mod a b)
  (add . args)
  (sub . args)
  (shl a b)
  (shr a b)
  (lt a b)
  (gt a b)
  (le a b)
  (ge a b)
  (eq a b)
  (ne a b)
  (band . args)
  (bxor . args)
  (bor . args)
  (and . args)
  (or . args)
  (ifexpr cond then else)
  (assign name value))

((vau defs env (each2 defs (fn (name expr)
        (defin (lookup forms name) emit'
          (eval (list emitter expr) env)))))
  id todo
  lit todo
  index (list ptr "[" expr "]")
  call (list func "(" (sepby ", " args) ")")
  dot (list expr "." name)
  arrow (list expr "->" name)
  post-inc (list expr "++")
  post-dec (list expr "--")
  pre-inc (list "++" expr)
  pre-dec (list "--" expr)
  addr (list "&" expr)
  deref (list "*" expr)
  pos (list "+" expr)
  neg (list "-" expr)
  not (list "!" expr)
  bnot (list "~" expr)
  sizeof (list "sizeof(" expr ")")
  alignof (list "_Alignof(" expr ")")
  cast (list "(" type ")" expr)
  mul (sepby " * " args)
  div (sepby " / " args)
  mod (list a " % " b)
  add (sepby " + " args)
  sub (sepby " - " args)
  shl (list a " << " b)
  shr (list a " >> " b)
  lt (list a " < " b)
  gt (list a " > " b)
  le (list a " <= " b)
  ge (list a " >= " b)
  eq (list a " == " b)
  ne (list a " != " b)
  band (sepby " & " args)
  bxor (sepby " ^ " args)
  bor (sepby " | " args)
  and (sepby " && " args)
  or (sepby " || " args)
  ifexpr (list cond " ? " then " : " else)
  assign (list name " = " value))

((vau levels env (each (enumerate levels) (fn ((n l))
        (each l (fn (x) (defin (lookup forms x) precedence n))))))
  (id literal)
  (index call dot arrow)
  (post-inc post-dec)
  (pre-inc pre-dec addr deref pos neg not bnot sizeof alignof)
  (cast)
  (mul div mod)
  (add sub)
  (shl shr)
  (lt gt le ge)
  (eq ne)
  (band)
  (bxor)
  (bor)
  (and)
  (or)
  (ifexpr)
  (assign))

(defin forms typedef (ns
    emit (emitter (line "typedef " (emit value) " " (str name) ";"))))

(defin C typedef (fn (value name) (ns
      form (forms typedef)
      name name
      value value)))

(devau (deftype name value) env (ns
    form typedef
    name name
    value (eval value env)))

(defns primtype
  emit (emitter name))

(defns (mkprimtype name . libs)
  form primtype
  name name
  includes libs)

(defns tlstmts
  deftype deftype)

(defns primtypes
  void      (mkprimtype "void")
  char      (mkprimtype "char")
  int       (mkprimtype "int")
  size_t    (mkprimtype "size_t"    "stddef.h")
  ptrdiff_t (mkprimtype "ptrdiff_t" "stddef.h")
  wchar_t   (mkprimtype "wchar_t"   "stddef.h")
  u8        (mkprimtype "uint8_t"   "stdint.h")
  s8        (mkprimtype "int8_t"    "stdint.h")
  u16       (mkprimtype "uint16_t"  "stdint.h")
  s16       (mkprimtype "int16_t"   "stdint.h")
  u32       (mkprimtype "uint32_t"  "stdint.h")
  s32       (mkprimtype "int32_t"   "stdint.h")
  u64       (mkprimtype "uint64_t"  "stdint.h")
  s64       (mkprimtype "int64_t"   "stdint.h")
  f32       (mkprimtype "float")
  f64       (mkprimtype "double"))

(def C (+ tlstmts primtypes))

(devau (printc x) _ (print (step9 (step8 (emit (eval x C))))))
