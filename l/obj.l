(def obj (ns))

(devau (new super . fields) e
  (+ (eval super e) (eval (cons ns fields) e)))

(devau (con super . params) e
  (fn args (match (new (eval super e)) params args)))

(def (isa type value)
  (if (not (isns value)) false
    (= type value) true
    (isa type (super value))))
