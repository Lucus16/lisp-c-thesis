(def pair.c (cfile
    (include "value.h")
    (include "meta.h")

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
