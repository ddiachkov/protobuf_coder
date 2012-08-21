#ifndef _DECODER_H_
#define _DECODER_H_

#include <ruby.h>

VALUE protobuf_coder_decode( VALUE module, VALUE string );
VALUE protobuf_coder_decode_from_pointer( VALUE module, VALUE pointer, VALUE offset, VALUE length );

#endif
