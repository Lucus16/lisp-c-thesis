### Namespaces

- `(bind ns name value)` - modify the top frame of `ns` to resolve `name` to
  `value`.
- `(lookup ns name)` - look up `name` in `ns` and throw error if undefined.
- `(lookup ns name default)` - look up `name` in `ns` and return `default` if
  undefined.
- `(eval code ns)` - evaluate `code` in `ns`.
- `(frame ns)` - return a new namespace frame on top of `ns`.
