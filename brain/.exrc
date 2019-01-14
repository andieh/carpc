if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <S-Tab> 
inoremap <C-Tab> 	
imap <silent> <Plug>IMAP_JumpBack =IMAP_Jumpfunc('b', 0)
imap <silent> <Plug>IMAP_JumpForward =IMAP_Jumpfunc('', 0)
inoremap <MiddleMouse> i<MiddleMouse>
map <silent>  :NERDTreeToggle
map <silent>  :YcmCompleter GoToDefinition
vmap <NL> <Plug>IMAP_JumpForward
nmap <NL> <Plug>IMAP_JumpForward
map  :pyf /usr/share/clang/clang-format.py
map <silent>  :so $MYVIMRC:echo "muahahahahaha - ENTER pressed - n00b! reloading config..."
map <silent>  :setlocal spell! spelllang=en_us
map <silent>  :setlocal spell! spelllang=de
nnoremap <silent>   :nohlsearch|:echo "yeeeessss - you found the SPACE key! (search highlight off!)"
nnoremap <silent> \q :QToggle
nnoremap <silent> \l :LToggle
nmap \ihn :IHN
nmap \is :IHS:A
nmap \ih :IHS
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
vmap <silent> <Plug>IMAP_JumpBack `<i=IMAP_Jumpfunc('b', 0)
vmap <silent> <Plug>IMAP_JumpForward i=IMAP_Jumpfunc('', 0)
vmap <silent> <Plug>IMAP_DeleteAndJumpBack "_<Del>i=IMAP_Jumpfunc('b', 0)
vmap <silent> <Plug>IMAP_DeleteAndJumpForward "_<Del>i=IMAP_Jumpfunc('', 0)
nmap <silent> <Plug>IMAP_JumpBack i=IMAP_Jumpfunc('b', 0)
nmap <silent> <Plug>IMAP_JumpForward i=IMAP_Jumpfunc('', 0)
map <F9> :!make -B
vnoremap <MiddleMouse> s<MiddleMouse>
imap 	 
imap <NL> <Plug>IMAP_JumpForward
imap \ihn :IHN
imap \is :IHS:A
imap \ih :IHS
cnoreabbr <expr> CP ((getcmdtype() is# ':' && getcmdline() is# 'WQA')?('cp'):('CP'))
cnoreabbr <expr> Cp ((getcmdtype() is# ':' && getcmdline() is# 'WQA')?('cp'):('Cp'))
cnoreabbr <expr> CN ((getcmdtype() is# ':' && getcmdline() is# 'WQA')?('cn'):('CN'))
cnoreabbr <expr> Cn ((getcmdtype() is# ':' && getcmdline() is# 'WQA')?('cn'):('Cn'))
cnoreabbr <expr> WQA ((getcmdtype() is# ':' && getcmdline() is# 'WQA')?('wqa'):('WQA'))
cnoreabbr <expr> WQa ((getcmdtype() is# ':' && getcmdline() is# 'WQa')?('wqa'):('WQa'))
cnoreabbr <expr> Wqa ((getcmdtype() is# ':' && getcmdline() is# 'Wqa')?('wqa'):('Wqa'))
cnoreabbr <expr> WQ ((getcmdtype() is# ':' && getcmdline() is# 'WQ')?('wq'):('WQ'))
cnoreabbr <expr> Wq ((getcmdtype() is# ':' && getcmdline() is# 'Wq')?('wq'):('Wq'))
cnoreabbr <expr> W ((getcmdtype() is# ':' && getcmdline() is# 'W')?('w'):('W'))
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=indent,eol,start
set expandtab
set fileencodings=ucs-bom,utf-8,default,latin1
set formatoptions=cqr,t
set helplang=en
set history=3500
set hlsearch
set ignorecase
set incsearch
set mouse=rnv
set pastetoggle=<F7>
set ruler
set shiftwidth=2
set showmatch
set smartindent
set smarttab
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc,.png,.jpg
set noswapfile
set tabpagemax=2048
set tabstop=2
set title
set undodir=~/.vim/undo_data/
set undofile
set undolevels=5000
set undoreload=5000
set viminfo='100,<10000,s10,h
" vim: set ft=vim :
