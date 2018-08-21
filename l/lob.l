(include std)
(include c)
(include c/std)

; I shouldn't be using templates because they're just another form of quoting
; and I want to functionally combine pieces of syntax, not by building shadow
; worlds.

; One solution is to use the convention that every vau has a corresponding
; function to allow programmatic calling. This can't be a standard
; transformation because every vau wraps its function differently. Using a
; simple convention like a mk- prefix would also result in shorter code.

; Definitions should not always be added to the same environment as where
; they're evaluated. The definition below should be evaluated in a fixed
; environment but the ids it produces should be added to a domain specific
; environment. In addition the let and surrounding code should not be evaluated
; in a scope with C bindings as they can easily conflict.
(def (getter type member)
  ((+ C (ns
        function (mkid (snake type "get" member))
        type (mkid (camel type))
        instance (mkid (lower type))
        member (mkid (lower member))))
    (defn function (list instance type) Value
      (return (-> instance member)))))

(def (getter type member) (let
    (function (snake type "get" member)
      type (camel type)
      instance (lower type)
      member (lower member))
    (defn function (instance type) Value
      (return (-> instance member)))))

(def (getter type member) (ctemplate
    (function (snake type "get" member)
      type (camel type)
      instance (lower type)
      member (lower member))
    (defn function (instance type) Value
      (return (-> instance member)))))

; Evaluate something in the C namespace with some extra bindings
(devau (ctemplate defs body) env
  (eval body (+ C (eval (cons ns defs) env))))

(def (getter type member) (let
    ((C defn) gettername
      (list ((C decln) instancename instancetype)) membertype
      ((C return) ((C ->) instancename membername)))))

(def (setter type member) (ctemplate
    (function (snake type "set" member)
      type (camel type)
      instance (lower type)
      member (lower member))
    (defn function (instance type value Value) void
      (call meta_free (-> instance member))
      (= (-> instance member) value))))

(def (freeer type members)
  (def (freecaller instance member) (using stdc
      (call meta_free (-> instance member))))
  (ctemplate
    (function (snake type "free")
      type (camel type)
      instance (lower type)
      freecalls (map1 [freecaller instance _] ???))
    (defn function (instance type) Value
