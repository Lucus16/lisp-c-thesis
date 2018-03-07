; C Generation

; TODO: nested lines in e.g. structs

; CONFIGURATION
(def indentation "\t")
(def newline "\n")

(def (step9 l) (apply + l))

(defu uline uindent uspaced)

(def (indent' level tail)
  (if (= level 0) tail
    (indent' (dec level) (cons indentation tail))))

(def (line . l) (cons uline l))
(def (indent . l) (cons uindent l))
(def (spaced . l) (cons uspaced l))

(def (step8a tree level tail) (if
    (no tree) tail
    (not (ispair tree)) (step8b tree tail)
    (= (car tree) uline) (indent' level (step8b (cdr tree) (cons newline tail)))
    (= (car tree) uindent) (step8a (cdr tree) (inc level) tail)
    (= (car tree) uspaced) (if
      (no (cdr tree)) nil
      (no (cdr (cdr tree))) (step8a (tree 1) level tail)
      (step8a (tree 1) level (cons " "
          (step8a (cons uspaced (cdr (cdr tree))) level tail))))
    (step8a (car tree) level (step8a (cdr tree) level tail))))

(def (step8b tree tail) (if
    (no tree) tail
    (isstr tree) (cons tree tail)
    (not (ispair tree)) (error (+ "Expected string, found: " (repr tree)))
    (step8b (car tree) (step8b (cdr tree) tail))))

(def (step8 tree) (step8a tree 0 nil))

(def (parenthesize x) (list "(" x ")"))
(def (sepby sep l)
  (if (no l) nil
    (no (cdr l)) l
    (cons (car l) (cons sep (sepby sep (cdr l))))))

(def (emit x)
  ((x emitf) x))

(def (printc x) (print (step9 (step8 (emit x)))))

; SETUP

(def Fragment (new obj))
(def Constant (new Fragment))
(def Specifier (new Fragment))
(def Statement (new Fragment))
(def Expression (new Fragment))
(def Declaration (new Fragment))
(def PPDirective (new Fragment))

(defns O)

(def (spacelist l)
  (if (no l) nil
    (no (cdr l)) (list (car l))
    (cons (car l) (cons " " (commalist (cdr l))))))

(def (commalist l)
  (if (no l) nil
    (no (cdr l)) (list (car l))
    (cons (list (car l) ", ") (commalist (cdr l)))))

(def (each2 l f)
  (if (no l) nil
    (cons (f (l 0) (l 1)) (each2 (cdr (cdr l)) f))))

(def (paramlist params)
  (commalist (each2 params (fn (name type) (list name " " type)))))

(def (fragment parent params emitexpr env)
  (def result (new parent
      new (fn args (match (+ result (ns)) params args))
      emitf (fn (i) (eval emitexpr (+ env i)))
      map (fn (o f) (apply (result new)
          (map1 (fn (x) (f (lookup o x) TODO)) params)))))
  result)

(devau (deffrag (name . params) emitexpr) o
  (bind O name (fragment Fragment params emitexpr o)))

(def (asstmt x) (if (isa Expression x) (ExpressionStmt x) x))

; SIMPLE C CODE

(deffrag (TranslationUnit declns)
  (map1 [list (emit _) (line) (line)] declns))

(deffrag (Identifier name)
  (str name))
(def Identifier (O Identifier))

(deffrag (FunctionDefinition returntype name params cstmt)
  (list (emit returntype) (emit name) (map1 emit params) (emit cstmt)))

; Define multiple fragments with a specific parent object
(devau (deffrags parent . l) o
  (def parent (eval parent o))
  (each2 l (fn ((name . params) emitexpr)
      (bind O name (fragment parent params emitexpr o)))))

(deffrags Expression
  (AssignmentExpr lhs rhs) (line (emit lhs) " = " (emit rhs))
  (PtrAccessExpr object member) (list (emit object) "->" (emit member))
  (AccessExpr object member) (list (emit object) "." (emit member))
  (CallExpr function args) (list
    (emit function) "(" (commalist (map1 emit args)) ")")
  (DerefExpr ptr) (list "*" (emit ptr))
  (SizeofExpr expr) (list "sizeof(" (emit expr) ")"))

(deffrags Statement
  (CompoundStmt stmts) (list "{" (indent (map1 emit stmts)) "}")
  (ReturnStmt expr) (line "return " (emit expr) ";"))

(deffrags Specifier
  (StructSpec members) (list "struct {" (indent (map1 emit members)) "}"))

(deffrag (TypedefDecln type name)
  (line "typedef " (emit type) " " (emit name)))

(deffrag (PointerDeclr declr)
  (list "*" (emit declr)))

(deffrag (MemberDecln type name)
  (list (emit type) " " (emit name)))

(deffrag (ParamDecln type name)
  (list (emit type) " " (emit name)))

(deffrag (Declaration type declarators)
  (list (emit type) (commalist (map1 emit declarators))))

(deffrag (DeclaratorInitializer declarator initializer)
  (list (emit declarator) " = " (emit initializer)))

(def (eval-or-id code env)
  (if (issymbol code)
    (lookup env code ((O Identifier new) code))
    (eval code env)))

; Give short names to trivial constructors
(defns C)
((vau l o (each2 l (fn (cname oname)
        (bind C cname ((lookup O oname) new))))))

; These trivial constructors convert unknown symbols to Identifiers
((vau l o (each2 l (fn (cname oname)
        (bind C cname (vau args env
            (apply ((lookup O oname) new) (map1 [eval-or-id _ env] args)))))))
  = AssignmentExpr
  . AccessExpr
  -> PtrAccessExpr
  ptr PointerDeclr
  typedef TypedefDecln
  return ReturnStmt
  deref DerefExpr
  sizeof SizeofExpr)

; Create some non-trivial constructors
(defin C do (fn stmts ((O CompoundStmt new) stmts)))
(defin C id (vau (name) _ ((O Identifier new) name)))

(defin C call (vau (function . args) env
    ((O CallExpr new) (eval-or-id function env)
      (map1 [eval-or-id _ env] args))))

(defin C := (vau (name type value) env
    ((O Declaration new) (eval-or-id type env)
      (list ((O DeclaratorInitializer new) (eval-or-id name env)
          (eval-or-id value env))))))

(defin C struct (vau l env
    ((O StructSpec new) (each2 l (fn (type name)
          ((O MemberDecln new) (eval-or-id type env) (eval-or-id name env)))))))

(defin C defn (vau (name params returntype . body) env
    ((O FunctionDefinition new)
      (eval-or-id returntype env)
      (eval-or-id name env)
      (each2 params (fn (type name) ((O ParamDecln new)
            (eval-or-id type env) (eval-or-id name env))))
      ((O CompoundStmt new) (map1 [eval-or-id _ env] body)))))

(def cfile (vau declns _
    ((O TranslationUnit new) (map1 [eval _ C] declns))))

; PAIR EXAMPLE USAGE

(def pair.c (cfile
    (typedef (struct
        _meta Meta
        car Value
        cdr Value) (ptr Pair))

    (defn pair_new (car Value cdr Value) Pair
      (:= pair Pair (call meta_new TYPE_PAIR (sizeof (deref pair))))
      (= (-> pair car) car)
      (= (-> pair cdr) cdr)
      (return pair))

    (defn pair_free (pair Pair) Value
      (call meta_free (-> pair car))
      (:= rest Value (-> pair cdr))
      (call free pair)
      (return rest))))

(printc pair.c)
