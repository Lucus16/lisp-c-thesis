; C Generation

(include std)
(include obj)

(include c/flatten)

(def Fragment (new obj))
(def Constant (new Fragment))
(def Specifier (new Fragment))
(def Statement (new Fragment))
(def Expression (new Fragment))
(def Declaration (new Fragment))
(def PPDirective (new Fragment))

(defns O)

(def (each2 l f)
  (if (no l) nil
    (cons (f (l 0) (l 1)) (each2 (cdr (cdr l)) f))))

(def (each3 l f)
  (if (no l) nil
    (cons (f (l 0) (l 1) (l 2)) (each3 (cdr (cdr (cdr l))) f))))

(def (paramlist params)
  (commalist (each2 params (fn (name type) (spaced name type)))))

(def (fragsonly x)
  (if (islist x) (flatten (map1 fragsonly x))
    (isa Fragment x) (list x)
    nil))

(def (fragargs x)
  (fragsonly (map1 [lookup x _] (x paramsf))))

(def (fragment parent params emitexpr env)
  (def result (new parent
      paramsf params
      new (fn args (match (+ result (ns)) params args))
      emitf (fn (i) (eval emitexpr (+ env i)))
      childf fragargs))
  result)

(def (emit x)
  ((x emitf) x))

(def (printc x)
  ((defaults print) (cflatten (emit x))))

(def (str-include x)
  (if (isstr x) (repr x)
    (issymbol x) (+ "<" (repr x) ".h>")
    (error (+ "Cannot be used as include: " (repr x)))))

(def (subfrags x)
  ((x childf) x))

(def (preorder x)
  (cons x (flatten (map1 preorder (subfrags x)))))

(devau (getdefault ns name default) env
  (lookup (eval ns env) name (eval default env)))

(def (genheaders xs) (then xs
    (map1 [preorder _])
    flatten
    (map1 [getdefault _ headers nil])
    (keep some)
    (map1 str-include)
    (sort <)
    dedup
    (map1 (O IncludeDirective new))
    ((O IncludeBlock new))))

(devau (deffrag (name . params) emitexpr) o
  (bind O name (fragment Fragment params emitexpr o)))

(def (asstmt x) (if (isa Expression x) ((O ExpressionStmt new) x) x))

; SIMPLE C CODE

(deffrag (TranslationUnit declns)
  (map1 [list (line) (emit _)] declns))

(deffrag (Identifier name object headers)
  name)
(def Identifier (O Identifier))

(deffrag (Comment lines)
  (map1 [line "// " _] lines))

(deffrag (FunctionDefinition returntype name params cstmt)
  (line (spaced (emit returntype)
    (list (emit name) (parenthesize (commalist (map1 emit params))))
    (emit cstmt))))

; Define multiple fragments with a specific parent object
(devau (deffrags parent . l) o
  (def parent (eval parent o))
  (each2 l (fn ((name . params) emitexpr)
      (bind O name (fragment parent params emitexpr o)))))

(deffrags PPDirective
  (PragmaOnceDirective) "#pragma once"
  (DefineDirective name value) (line "#define " (emit name) " " (emit value))
  (IncludeDirective header) (line "#include " header)
  (IncludeBlock includes) (map1 emit includes))

(deffrags Expression
  (AssignmentExpr lhs rhs) (spaced (emit lhs) "=" (emit rhs))
  (PtrAccessExpr object member) (list (emit object) "->" (emit member))
  (AccessExpr object member) (list (emit object) "." (emit member))
  (CallExpr function args) (list
    (emit function) "(" (commalist (map1 emit args)) ")")
  (IndexExpr array index) (list (emit array) "[" (emit index) "]")
  (PostIncrementExpr expr) (list (emit expr) "++")
  (PostDecrementExpr expr) (list (emit expr) "--")
  (PreIncrementExpr expr) (list "++" (emit expr))
  (PreDecrementExpr expr) (list "--" (emit expr))
  (SizeofExpr expr) (list "sizeof(" (emit expr) ")")
  (AddressExpr expr) (list "&" (emit expr))
  (DerefExpr ptr) (list "*" (emit ptr))
  (NegativeExpr expr) (list "-" (emit expr))
  (EqualityExpr lhs rhs) (spaced (emit lhs) "==" (emit rhs))
  (InequalityExpr lhs rhs) (spaced (emit lhs) "!=" (emit rhs))
  (BitwiseNotExpr expr) (list "~" (emit expr))
  (BitwiseAndExpr lhs rhs) (spaced (emit lhs) "&" (emit rhs))
  (BitwiseOrExpr lhs rhs) (spaced (emit lhs) "|" (emit rhs))
  (BitwiseXorExpr lhs rhs) (spaced (emit lhs) "^" (emit rhs))
  (LogicalNotExpr expr) (list "!" (emit expr))
  (LogicalAndExpr lhs rhs) (spaced (emit lhs) "&&" (emit rhs))
  (LogicalOrExpr lhs rhs) (spaced (emit lhs) "||" (emit rhs))
  (ConditionalExpr cond then else) (spaced
    (emit cond) "?" (emit then) ":" (emit else))
  (IntegerLiteral value) (repr value)
  (StringLiteral value) (repr value))

(deffrags Statement
  (ExpressionStmt expr) (line (emit expr) ";")
  (CompoundStmt stmts) (list "{" (indent (map1 emit stmts)) "}")
  (ReturnStmt expr) (line "return " (emit expr) ";"))

(deffrags Specifier
  (StructSpec members) (list "struct {" (indent (map1 emit members)) "}")
  (EnumSpec members) (list "enum {" (indent (map1 emit members)) "}"))

(deffrag (TypedefDecln type name)
  (line "typedef " (emit type) " " (emit name) ";"))

(deffrag (PointerDeclr declr)
  (list "*" (emit declr)))

(deffrag (MemberDecln type name)
  (line (spaced (emit type) (emit name)) ";"))

(deffrag (ParamDecln type name)
  (list (emit type) " " (emit name)))

(deffrag (Declaration type declarators)
  (line (spaced (emit type) (commalist (map1 emit declarators))) ";"))

(deffrag (DeclaratorInitializer declarator initializer)
  (list (emit declarator) " = " (emit initializer)))

(def (eval-convert code env)
  (if (issymbol code) (do
      (def sym (lookup env code nil))
      (if (isa Fragment sym) sym ((O Identifier new) (repr code) nil nil)))
    (isint code) ((O IntegerLiteral new) code)
    (isstr code) ((O StringLiteral new) code)
    (eval code env)))

; Give short names to trivial constructors
(defns C)
((vau l o (each2 l (fn (cname oname)
        (bind C cname ((lookup O oname) new))))))

; These trivial constructors convert unknown symbols to Identifiers
((vau l o (each2 l (fn (cname oname)
        (bind C cname (vau args env
            (apply ((lookup O oname) new) (map1 [eval-convert _ env] args)))))))
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
(defin C id (vau (name) _ ((O Identifier new) name nil nil)))
(defin C comment (fn lines ((O Comment new) lines)))

(defin C define (vau (name value) env
    ((O DefineDirective new) ((O Identifier new) (str name) nil nil)
      (eval-convert value env))))

(defin C include (vau headers _
    ((O IncludeBlock new)
      (map1 [(O IncludeDirective new) (str-include _)] headers))))

(defin C call (vau (function . args) env
    ((O CallExpr new) (eval-convert function env)
      (map1 [eval-convert _ env] args))))

(defin C := (vau (name type value) env
    ((O Declaration new) (eval-convert type env)
      (list ((O DeclaratorInitializer new) (eval-convert name env)
          (eval-convert value env))))))

(defin C struct (vau l env
    ((O StructSpec new) (each2 l (fn (name type)
          ((O MemberDecln new) (eval-convert type env) (eval-convert name env)))))))

(defin C defn (vau (name params returntype . body) env
    ((O FunctionDefinition new)
      (eval-convert returntype env)
      (eval-convert name env)
      (each2 params (fn (name type) ((O ParamDecln new)
            (eval-convert type env) (eval-convert name env))))
      ((O CompoundStmt new) (map1 [asstmt (eval-convert _ env)] body)))))

(def cfile (vau declns env
    (def env (+ env C))
    (def declns (map1 [eval _ env] declns))
    ((O TranslationUnit new) (cons (genheaders declns) declns))))

(def hfile (vau declns env
    (def env (+ env C))
    (def declns (map1 [eval _ env] declns))
    ((O TranslationUnit new) (cons ((O PragmaOnceDirective new))
        (cons (genheaders declns) declns)))))

(devau (defc name body) _ (eval body C))

(export O C Fragment
  subfrags preorder genheaders cfile hfile printc)
