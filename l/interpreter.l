(defun (append x y) (if
    (isnil x) y
    (cons (car x) (append (cdr x) y))))

(defun (zip x y) (if
    (not (ispair x)) nil
    (not (ispair y)) nil
    (cons (cons (car x) (car y)) (zip (cdr x) (cdr y)))))

(defun (assoc key env) (if
    (eq (car (car env)) key) (cdr (car env))
    (assoc key (cdr env))))

(defun (eval expr env) (if
    (issymbol expr) (assoc expr env)
    (ispair expr) ((eval (car expr) env) (cdr expr) env)
    expr))

(defun (eval-list l env) (if
    (isnil l) nil
    (cons (eval (car l) env) (eval-list (cdr l) env))))

(defun (wrap-default f) (fn (expr env) (f . (eval-list expr env))))

(defun (default-if args env) (if
    (isnil args) nil
    (isnil (cdr args)) (eval (car args) env)
    (eval (car args) env) (eval (car (cdr args)) env)
    (default-if (cdr (cdr args)) env)))

(defun (default-fn args env) (wrap-default
    (fn l (eval (car (cdr args)) (append
          (zip (car args) l) env)))))

(def defaults (list
    (cons `quote (fn (expr env) expr))
    (cons `if default-if)
    (cons `fn default-fn)
    (cons `car (wrap-default car))
    (cons `cdr (wrap-default cdr))
    (cons `cons (wrap-default cons))
    (cons `eq (wrap-default eq))
    (cons `issymbol (wrap-default issymbol))
    (cons `ispair (wrap-default ispair))
    (cons `isnil (wrap-default isnil))
    (cons `eval (wrap-default eval))
    (cons `defaults defaults)))
