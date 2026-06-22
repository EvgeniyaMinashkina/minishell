alias vg='valgrind \
--leak-check=full \
--show-leak-kinds=all \
--track-fds=yes \
--suppressions=readline.supp'