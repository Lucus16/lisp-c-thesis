syntax match lLineComment "\v\;.*"
syntax match lNumber "\v<\-?\d+>"
syntax match lChar "\v\'\\?.\'"
" Clojure style characters, if I choose to adopt it:
"syntax match lChar "\v\\(:?newline|space|tab|(:?u[0-9a-fA-F]{4})|.)"
syntax region lString start=/"/ skip="\v\\." end=/"/ oneline

hi default link lLineComment Comment
hi default link lNumber Number
hi default link lChar Character
hi default link lString String
