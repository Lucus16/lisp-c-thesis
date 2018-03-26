(devau (quote x) _ x)
(def (quoted x) (list quote x))
(devau [. code] e (eval (list fn (quote (_)) code) e))

(def (!= x y) (not (= x y)))
(def (list . l) l)
(def (id x) x)
(def (const x) (fn _ x))
(def (eqx x) [= x _])
(def (swapargs f) (fn (x y) (f y x)))
(def (inc n) (+ n 1))
(def (un f) (fn l (not (apply f l))))
;(def (dec n) (+ n (- 1)))
;(devau (apply f l) e (eval (cons f (map1 quoted (eval l e))) e))
;(devau (using inner . body) outer (eval body (+ outer inner)))

(def (islist x) (or (ispair x) (no x)))

(def (range n)
  (def (range' n r)
    (if (< n 1) r
      (range' (dec n) (cons (dec n) r))))
  (range' n nil))

(def (len x)
  (def (len' x n)
    (if (no x) n
      (len' (cdr x) (+ 1 n))))
  (len' x 0))

(def (join sep l)
  (if (no l) ""
    (no (cdr l)) (car l)
    (join sep (cons (+ (car l) sep (car (cdr l))) (cdr (cdr l))))))

(def (revjoin sep l)
  (def (revjoin' l r)
    (if (no l) r
      (no (cdr l)) (cons (car l) r)
      (revjoin' (cdr l) (cons sep (cons (car l) r)))))
  (apply + (revjoin' l nil)))

(def (map1 f l)
  (if (no l) nil
    (cons (f (car l)) (map1 f (cdr l)))))

(def (cars l) (map1 car l))
(def (cdrs l) (map1 cdr l))

(def (all f l) (if
    (no l) true
    (not (f (car l))) false
    (all f (cdr l))))

(def (any f l) (if
    (no l) false
    (f (car l)) true
    (any f (cdr l))))

(def (map f)
  (def (result . ls) (if
      (not (all some ls)) nil
      (cons (apply f (cars ls)) (apply result (cdrs ls)))))
  result)

(def (revmap f l)
  (def (revmap' l r)
    (if (no l) r
      (revmap' (cdr l) (cons (f (car l)) r))))
  (revmap' l nil))

(def (enumerate l)
  (def (enumerate' l n)
    (if (no l) nil
      (cons (list n (car l)) (enumerate' (cdr l) (inc n)))))
  (enumerate' l 0))

(def (zip2 a b)
  (if (or (no a) (no b)) nil
    (cons (list (car a) (car b)) (zip2 (cdr a) (cdr b)))))

(def (reverse l)
  (def (reverse' l r)
    (if (no l) r
      (reverse' (cdr l) (cons (car l) r))))
  (reverse' l nil))

(def (merge-by op l1 l2 l)
  (if (and (no l1) (no l2)) (reverse l)
    (no l1) (merge-by op l1 (cdr l2) (cons (car l2) l))
    (no l2) (merge-by op (cdr l1) l2 (cons (car l1) l))
    (op (car l2) (car l1)) (merge-by op l1 (cdr l2) (cons (car l2) l))
    (merge-by op (cdr l1) l2 (cons (car l1) l))))

(def (sort-by' op l l1 l2)
  (if (no l) (merge-by op (sort-by op l1) (sort-by op l2) nil)
    (sort-by' op (cdr l) (cons (car l) l2) l1)))

(def (sort-by op l)
  (if (no l) l
    (no (cdr l)) l
    (sort-by' op l nil nil)))

(def (sort l)
  (sort-by < l))

(def (dedup l)
  (if (no l) l
    (no (cdr l)) l
    (= (l 0) (l 1)) (dedup (cdr l))
    (cons (car l) (dedup (cdr l)))))

(def (> . l)
  (apply < (reverse l)))

(def (each l f)
  (if (no l) nil
    (do (f (car l))
      (each (cdr l) f))))

(def (each2 l f)
  (if (no l) nil
    (no (cdr l)) (error "Expected even number of args, got 1")
    (do (f (l 0) (l 1))
      (each2 (cdr (cdr l)) f))))

(def rawprint print)
(def oldrepr repr)

(def (nsrepr e)
  (def (recur l)
    (if (no l) nil
      (+ (list " " (repr (car l)) " " "...")
        (recur (cdr l)))))
  (+ "(ns" (apply + (cons "" (recur (keys e)))) ")"))

(def (listrepr l)
  (def (recur l)
    (if (no l) (list ")")
      (cons " " (cons (repr (car l)) (recur (cdr l))))))
  (if (no l) "()"
    (no (cdr l)) (+ "(" (repr (car l)) ")")
    (+ "(" (repr (car l)) (apply + (recur (cdr l))))))

(def (repr x)
  (if (isns x) (nsrepr x)
    (ispair x) (listrepr x)
    (oldrepr x)))

(def (strx x)
  (if (isstr x) x
    (ischar x) (+ x)
    (repr x)))

(def (print . l)
  (rawprint (+ (join " " (map1 strx l)) "\n")))

(def (str . l)
  (if (no l) ""
    (no (cdr l)) (strx (car l))
    (apply str (cons (+ (strx (car l)) (strx (car (cdr l)))) (cdr (cdr l))))))

(def (dyn)
  (devau (result . args) env
    (if (no args) (getdyn result)
      (eval (cons withdyn (cons result args)) env)))
  result)

(devau (defns name . stmts) e
  (eval (list def name (cons ns stmts)) e))

(devau (defin ns name . body) e
  (bind (eval ns e) name (eval (cons do body) e)))

(devau (defu . names) e (map1 [bind e _ (uniq)] names) nil)

(def lazy
  (defu noresult)
  (vau (x) e
    (def result (var noresult))
    (fn ()
      (if (= noresult (result)) (result (eval x e)))
      (result))))

; Trace return value
(devau (tracer . l) e
  (def r (eval l e))
  (print r)
  r)

; TODO: Trace every call recursively
(devau (trace . l) e
  (def f (eval (car l) e))
  (if (isfn f) (do
      (def args (eval (cons list (cdr l)) e))
      (print (cons (car l) args))
      (def result (apply f args))
      (print (cons (car l) args) " => " result)
      result)
    (do
      (print l)
      (eval l e))))
