(def (reverse l)
  (def (reverse' l r)
    (if (no l) r
      (reverse' (cdr l) (cons (car l) r))))
  (reverse' l nil))

(def (mergeby op l1 l2 l)
  (if (and (no l1) (no l2)) (reverse l)
    (no l1) (mergeby op l1 (cdr l2) (cons (car l2) l))
    (no l2) (mergeby op (cdr l1) l2 (cons (car l1) l))
    (op (car l2) (car l1)) (mergeby op l1 (cdr l2) (cons (car l2) l))
    (mergeby op (cdr l1) l2 (cons (car l1) l))))

(def (sortby' op l l1 l2)
  (if (no l) (mergeby op (sortby op l1) (sortby op l2) nil)
    (sortby' op (cdr l) (cons (car l) l2) l1)))

(def (sortby op l)
  (if (no l) l
    (no (cdr l)) l
    (sortby' op l nil nil)))

(def sort sortby)

(export reverse sort)
