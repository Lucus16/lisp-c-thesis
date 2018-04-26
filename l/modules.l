((vau _ outer
    (def modules (ns))
    (def undefined (uniq))

    (def (import-path name path)
      (if (some path) path (+ (repr name) ".l")))

    (def (import-cached path)
      (if (= undefined (lookup modules path undefined))
        (bind modules path (evalfile path (+ outer (ns)))))
      (lookup modules path))

    (devau (import name path ?) env
      (bind env name (import-cached (import-path name path))))

    (def (copy-bindings source target names)
      (if (some names) (do
          (bind target (car names) (lookup source (car names)))
          (copy-bindings source target (cdr names))))
      target)

    (def (copy-ns source target)
      (copy-bindings source target (keys source))
      nil)

    (devau (include name path ?) env
      (copy-ns (import-cached (import-path name path)) env))

    (devau (export . names) env
      (copy-bindings env (ns) names))

    (copy-ns (export import include export) outer)))
