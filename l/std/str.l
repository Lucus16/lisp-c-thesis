(def rawprint print)
(def rawrepr repr)

(def (join sep l)
  (if (no l) ""
    (no (cdr l)) (car l)
    (join sep (cons (+ (car l) sep (car (cdr l))) (cdr (cdr l))))))

(def (camel1 x)
  (+ (upper (slice x 0 1)) (lower (slice x 1 (len x)))))

(def (camel . xs)
  (join "" (map1 camel1 xs)))

(def (snake . xs)
  (join "_" (map1 lower xs)))

(def (str1 x)
  (if (isstr x) x
    (ischar x) (+ x)
    (repr x)))

(def (print . l)
  (rawprint (join " " (map1 str1 l))))

(def (println . l)
  (rawprint (+ (join " " (map1 str1 l)) "\n")))

(def (str . l)
  (if (no l) ""
    (no (cdr l)) (str1 (car l))
    (apply str (cons (+ (str1 (car l)) (str1 (car (cdr l)))) (cdr (cdr l))))))
