#ifndef _PROTOBUF_CODER_H_
#define _PROTOBUF_CODER_H_

#include <ruby.h>

// Protobuf wire types (for internal use)
enum wire_type {
    WT_VARINT = 0,
    WT_64BIT  = 1,
    WT_STRING = 2,
    WT_32BIT  = 5
};

extern VALUE rb_mProtobufCoder;
extern VALUE rb_mProtobufCoderWireType;
extern VALUE rb_sProtobufCoderField;
extern VALUE rb_eProtobufCoderEncodingError;
extern VALUE rb_eProtobufCoderDecodingError;

#endif
