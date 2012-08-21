require "mkmf"

$CFLAGS << " -pedantic -Wall -Wextra -Werror -Wno-unused-parameter -std=c99"

create_makefile "protobuf_coder"