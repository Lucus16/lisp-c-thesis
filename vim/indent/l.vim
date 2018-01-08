setlocal lisp
setlocal lispwords=
"setlocal indentexpr=LIndent()

"function! LIndent()
"	if line('.') == 1
"		return 0
"	endif
"	let lnum = prevnonblank(line('.') - 1)
"
"	let paren_count = 0
"	while 1
"	endwhile
"endfunction
