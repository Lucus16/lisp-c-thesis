; DOT FIELD ACCESS
(def a (ns b 2 c 3))
(assert= a.b 2)
(assert= a.c 3)

; SLICING
(slice xs start end)

; ARRAYS
; - Work exactly like lists, except with fast random-access and order is cdr car

; STACK UNWINDING
; - Needed for reducing memory leaks when handling errors
; - Needed for stack traces
