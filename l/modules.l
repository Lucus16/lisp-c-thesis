((vau _ outer
    (def modules (ns))
    (def undefined (uniq))

    (def (import-path name path)
      (if (some path) path (+ (repr name) ".l")))

    (def (import-cached path)
      (if (= undefined (lookup modules path undefined))
        (bind modules path (evalfile path (+ outer (ns)))))
      (lookup modules path))

    (def (reversed l r)
      (if (no l) r
        (reversed (cdr l) (cons (car l) r))))

    (def oldkeys keys)
    (def (keys x) (reversed (oldkeys x) nil))

    (def (each l f)
      (if (some l) (do
          (f (car l))
          (each (cdr l) f))))

    (def (reload)
      (def paths (keys modules))
      (each paths (fn (path) (bind modules path undefined))))

    (devau (import name path ?) env
      (bind env name (import-cached (import-path name path))))

    (def (copy-bindings source target names)
      (each names (fn (name) (bind target name (lookup source name))))
      target)

    (def (copy-ns source target)
      (copy-bindings source target (keys source))
      nil)

    (devau (include name path ?) env
      (copy-ns (import-cached (import-path name path)) env))

    (devau (export . names) env
      (copy-bindings env (ns) names))

    (copy-ns (export reload import include export) outer)))
