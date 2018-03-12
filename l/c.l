; C Generation

; TODO: nested lines in e.g. structs

; CONFIGURATION
(def indentation '\t')
(def newline '\n')

(def (step9 l) (apply + l))

(defu uline uindent uspaced ulinetoken)

(def (indent' level tail)
  (if (= level 0) tail
    (indent' (dec level) (cons indentation tail))))

(def (line . l) (cons uline l))
(def (indent . l) (cons uindent l))
(def (spaced . l) (cons uspaced l))

(def (cons-fixline head tail)
  (if (and (ispair tail) (= (car tail) ulinetoken))
    (cons head (cons newline (cdr tail)))
    (cons head tail)))

(def (flatten-spaced tree level tail) (if
    (no tree) tail
    (no (cdr tree)) (flatten-c (car tree) level tail)
    (flatten-c (car tree) level (cons-fixline ' '
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

(def (parenthesize x) (list '(' x ')'))
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

(def (commalist' l)
  (if (no l) nil
    (no (cdr l)) (list (car l))
    (cons (list (car l) ',') (commalist' (cdr l)))))

(def (commalist l)
  (cons uspaced (commalist' l)))

(def (each2 l f)
  (if (no l) nil
    (cons (f (l 0) (l 1)) (each2 (cdr (cdr l)) f))))

(def (paramlist params)
  (commalist (each2 params (fn (name type) (spaced name type)))))

(def (fragment parent params emitexpr env)
  (def result (new parent
      new (fn args (match (+ result (ns)) params args))
      emitf (fn (i) (eval emitexpr (+ env i)))
      map (fn (o f) (apply (result new)
          (map1 (fn (x) (f (lookup o x) TODO)) params)))))
  result)

(devau (deffrag (name . params) emitexpr) o
  (bind O name (fragment Fragment params emitexpr o)))

(def (asstmt x) (if (isa Expression x) ((O ExpressionStmt new) x) x))

; SIMPLE C CODE

(deffrag (TranslationUnit declns)
  (map1 [list (line) (emit _)] declns))

(deffrag (Identifier name)
  (str name))
(def Identifier (O Identifier))

(deffrag (FunctionDefinition returntype name params cstmt)
  (spaced (emit returntype)
    (list (emit name) (parenthesize (commalist (map1 emit params))))
    (emit cstmt)))

; Define multiple fragments with a specific parent object
(devau (deffrags parent . l) o
  (def parent (eval parent o))
  (each2 l (fn ((name . params) emitexpr)
      (bind O name (fragment parent params emitexpr o)))))

(deffrags PPDirective
  (IncludeDirective header) (line "#include " header))

(deffrags Expression
  (AssignmentExpr lhs rhs) (list (emit lhs) " = " (emit rhs))
  (PtrAccessExpr object member) (list (emit object) "->" (emit member))
  (AccessExpr object member) (list (emit object) "." (emit member))
  (CallExpr function args) (list
    (emit function) "(" (commalist (map1 emit args)) ")")
  (DerefExpr ptr) (list "*" (emit ptr))
  (SizeofExpr expr) (list "sizeof(" (emit expr) ")"))

(deffrags Statement
  (ExpressionStmt expr) (line (emit expr) ';')
  (CompoundStmt stmts) (list "{" (indent (map1 emit stmts)) "}")
  (ReturnStmt expr) (line "return " (emit expr) ";"))

(deffrags Specifier
  (StructSpec members) (list "struct {" (indent (map1 emit members)) "}"))

(deffrag (TypedefDecln type name)
  (line "typedef " (emit type) " " (emit name)))

(deffrag (PointerDeclr declr)
  (list "*" (emit declr)))

(deffrag (MemberDecln type name)
  (line (spaced (emit type) (emit name)) ';'))

(deffrag (ParamDecln type name)
  (list (emit type) " " (emit name)))

(deffrag (Declaration type declarators)
  (line (spaced (emit type) (commalist (map1 emit declarators))) ';'))

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
(defin C do (fn stmts ((O CompoundStmt new) (map asstmt stmts))))
(defin C id (vau (name) _ ((O Identifier new) name)))

(defin C include (vau (header) env
    (def header (eval-or-id header env))
    ((O IncludeDirective new) (if
        (isstr header) (repr header)
        (issymbol header) (+ '<' (repr header) '>')))))

(defin C call (vau (function . args) env
    ((O CallExpr new) (eval-or-id function env)
      (map1 [eval-or-id _ env] args))))

(defin C := (vau (name type value) env
    ((O Declaration new) (eval-or-id type env)
      (list ((O DeclaratorInitializer new) (eval-or-id name env)
          (eval-or-id value env))))))

(defin C struct (vau l env
    ((O StructSpec new) (each2 l (fn (name type)
          ((O MemberDecln new) (eval-or-id type env) (eval-or-id name env)))))))

(defin C defn (vau (name params returntype . body) env
    ((O FunctionDefinition new)
      (eval-or-id returntype env)
      (eval-or-id name env)
      (each2 params (fn (name type) ((O ParamDecln new)
            (eval-or-id type env) (eval-or-id name env))))
      ((O CompoundStmt new) (map1 [asstmt (eval-or-id _ env)] body)))))

(def cfile (vau declns _
    ((O TranslationUnit new) (map1 [eval _ C] declns))))

; PAIR EXAMPLE USAGE

(def pair.c (cfile
    (include "value.h")
    (include "meta.h")

    (typedef (struct
        _meta Meta
        car Value
        cdr Value) (ptr Pair))

    (defn pair_new (car Value cdr Value) Pair
      (:= nil nil nil)
      (:= pair Pair (call meta_new TYPE_PAIR (sizeof (deref pair))))
      (= (-> pair car) car)
      (= (-> pair cdr) cdr)
      (return pair))

    (defn pair_free (pair Pair) Value
      (call meta_free (-> pair car))
      (:= rest Value (-> pair cdr))
      (call free pair)
      (return rest))

    (defn pair_car (pair Pair) Value
      (return (-> pair car)))

    (defn pair_cdr (pair Pair) Value
      (return (-> pair cdr)))

    (defn pair_set_car (pair Pair value Value) void
      (call meta_free (-> pair car))
      (= (-> pair car) value))

    (defn pair_set_cdr (pair Pair value Value) void
      (call meta_free (-> pair cdr))
      (= (-> pair cdr) value))))

(printc pair.c)
