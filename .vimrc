syntax on
set number
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'gmarik/Vundle.vim'

" operator-user
Plugin 'kana/vim-operator-user'

" clang-format
Plugin 'rhysd/vim-clang-format'

" cpp-enhanced-highlight
Plugin 'octol/vim-cpp-enhanced-highlight'

" NERD commenter
Plugin 'scrooloose/nerdcommenter'

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line
"

let g:clang_format#style_options = {
      \ "Language" : "Cpp",
      \ "BasedOnStyle" : "Google",
      \ "AccessModifierOffset" : -1,
      \ "ConstructorInitializerIndentWidth" : 4,
      \ "AlignEscapedNewlinesLeft" : "true",
      \ "AlignTrailingComments" : "true",
      \ "AllowAllParametersOfDeclarationOnNextLine" : "true",
      \ "AllowShortBlocksOnASingleLine" : "false",
      \ "AllowShortIfStatementsOnASingleLine" : "false",
      \ "AllowShortLoopsOnASingleLine" : "false",
      \ "AllowShortFunctionsOnASingleLine" : "false",
      \ "AlwaysBreakTemplateDeclarations" : "true",
      \ "AlwaysBreakBeforeMultilineStrings" : "true",
      \ "BreakBeforeBinaryOperators" : "false",
      \ "BreakBeforeTernaryOperators" : "true",
      \ "BreakConstructorInitializersBeforeComma" : "false",
      \ "BinPackParameters" : "true",
      \ "ColumnLimit" : 80,
      \ "ConstructorInitializerAllOnOneLineOrOnePerLine" : "false",
      \ "DerivePointerAlignment" : "true",
      \ "ExperimentalAutoDetectBinPacking" : "false",
      \ "IndentCaseLabels" : "true",
      \ "IndentWrappedFunctionNames" : "false",
      \ "IndentFunctionDeclarationAfterType" : "false",
      \ "MaxEmptyLinesToKeep" : 1,
      \ "KeepEmptyLinesAtTheStartOfBlocks" : "false",
      \ "NamespaceIndentation" : "None",
      \ "ObjCSpaceAfterProperty" : "false",
      \ "ObjCSpaceBeforeProtocolList" : "false",
      \ "PenaltyBreakBeforeFirstCallParameter" : 1,
      \ "PenaltyBreakComment" : 300,
      \ "PenaltyBreakString" : 1000,
      \ "PenaltyBreakFirstLessLess" : 120,
      \ "PenaltyExcessCharacter" : 1000000,
      \ "PenaltyReturnTypeOnItsOwnLine" : 200,
      \ "PointerAlignment" : "Left",
      \ "SpacesBeforeTrailingComments" : 2,
      \ "Cpp11BracedListStyle" : "false",
      \ "Standard" : "Auto",
      \ "IndentWidth" : 2,
      \ "TabWidth" : 2,
      \ "UseTab" : "Never",
      \ "BreakBeforeBraces" : "Attach",
      \ "SpacesInParentheses" : "false",
      \ "SpacesInAngles" : "false",
      \ "SpaceInEmptyParentheses" : "false",
      \ "SpacesInCStyleCastParentheses" : "false",
      \ "SpacesInContainerLiterals" : "true",
      \ "SpaceBeforeAssignmentOperators" : "true",
      \ "ContinuationIndentWidth" : 4,
      \ "SpaceBeforeParens" : "ControlStatements",
      \ "DisableFormat" : "false"}

" map to <Leader>cf in C++ code
autocmd FileType c,cpp,objc nnoremap <buffer><Leader>cf :<C-u>ClangFormat<CR>
autocmd FileType c,cpp,objc vnoremap <buffer><Leader>cf :ClangFormat<CR>
" if you install vim-operator-user
autocmd FileType c,cpp,objc map <buffer><Leader>x <Plug>(operator-clang-format)
" Toggle auto formatting:
nmap <Leader>C :ClangFormatAutoToggle<CR>

:set tabstop=2 shiftwidth=2 expandtab
