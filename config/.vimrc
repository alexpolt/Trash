scriptencoding utf-8
set encoding=utf-8

set nu
set expandtab
set tabstop=2
set shiftwidth=2
set t_Co=256
color zoria
set textwidth=120
set colorcolumn=101
highlight ColorColumn ctermbg=darkgray
set switchbuf=usetab,split
set hidden
set nowrap
set ruler
set nocompatible
set backspace=indent,eol,start
set showcmd
set autoindent
set smartindent
set ignorecase

set sidescroll=5
set listchars=eol:¬,tab:»·,trail:·,extends:>,precedes:<
"set listchars=eol:¬,tab:→→,tab:»·,trail:·,extends:>,precedes:<

nnoremap Q gq}
inoremap <C-U> <C-G>u<C-U>

set path=.,**
let g:loaded_netrw=1
let g:loaded_netrwPlugin=1

map <F3> :ls<CR>
map <F6> :shell<CR>
map <C-F5> :make!<CR>
map <F5> :!./main.exe<CR>
map <F10> :bd<CR>
map <C-F10> :bd!<CR>
map <F9> :tab new<CR>:set nobuflisted\|setlocal buftype=nofile\|setlocal bufhidden=hide\|setlocal noswapfile<CR>
        \:set syntax=c<CR>:0r !find . -maxdepth 6 \( -name \*.h -o -name \*.cpp \) -printf '\%p "\%f" \%s\n'
"map <F10> :lgetbuffer<CR>:lopen<CR><C-W>H:vertical resize 30<CR>

nmap <F2> :update<CR>
vmap <F2> <Esc><F2>gv
imap <F2> <C-O><F2>

map gf <C-w>gf
map <C-W>x :bd<CR>
map <C-X> :qall!<CR>

map <TAB> <C-PageDown>
map <S-TAB> <C-PageUp>

map [1;5A 10<Up>
map [1;5B 10<Down>
imap [1;5A <C-O>10<Up>
imap [1;5B <C-O>10<Down>

map [5~ <PageUp>zz
map [6~ <PageDown>zz
imap [5~ <C-O><PageUp><c-O>zz
imap [6~ <C-O><PageDown><C-O>zz
 
