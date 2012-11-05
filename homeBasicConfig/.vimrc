set nu
:syntax on
":color desert
:set background=light

:set autoindent
:set tabstop=4
":set 
:set history=1000
:set nocompatible
:set smartindent
:set shiftwidth=4
:set showmatch
:set ruler
:set incsearch

if has("autocmd")
	filetype indent on
endif

" Set encoding and language.
set encoding=utf-8
set langmenu=zh_CN.UTF-8
language message zh_CN.UTF-8


set fileencodings=ucs-bom,utf-8,cp936,gb18030,big5,euc-jp,euc-kr,latin1
"
set fileencoding=utf-8
"
source $VIMRUNTIME/delmenu.vim
"
source $VIMRUNTIME/menu.vim
