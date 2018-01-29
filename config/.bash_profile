alias egrep='egrep --color=auto'
alias fgrep='fgrep --color=auto'
alias g='git'
alias grep='grep --color=auto'
alias l='ls -CF'
alias la='ls -A'
alias ll='ls -alF'
alias rm='rm -r'
alias v='vim -p'
alias p='pwd'
alias make='make -r'

export CLICOLOR=1

MYPS='$(off=0; if [ ${#PWD} -gt 50 ]; then echo "...${PWD:$(( ${#PWD}/2 )):1000}"; else echo "${PWD}"; fi;)'

export PS1="\[\033[\[\033[0;31m\]\]$MYPS\[\033[0m\]$"

PATH="$PATH:$HOME/Projects/bash"
#$PATH="/usr/local/opt/make/libexec/gnubin:$PATH"
