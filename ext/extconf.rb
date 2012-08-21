require "mkmf"

$CFLAGS << " -std=c99 -pedantic -Wall -Wextra -Werror -Wno-unused-parameter -Wno-declaration-after-statement "

create_makefile "protobuf_coder"