#include "protobuf_coder.h"
#include "encoder.h"
#include "decoder.h"

void Init_protobuf_coder()
{
    // module ProtobufCoder
    rb_mProtobufCoder = rb_define_module( "ProtobufCoder" );

    // module ProtobufCoder::WireType with wire type constants
    rb_mProtobufCoderWireType = rb_define_module_under( rb_mProtobufCoder, "WireType" );

    rb_define_const( rb_mProtobufCoderWireType, "VARINT", INT2FIX( 0 ));
    rb_define_const( rb_mProtobufCoderWireType, "F64BIT", INT2FIX( 1 ));
    rb_define_const( rb_mProtobufCoderWireType, "STRING", INT2FIX( 2 ));
    rb_define_const( rb_mProtobufCoderWireType, "F32BIT", INT2FIX( 5 ));

    // structure ProtobufCoder::Field
    rb_sProtobufCoderField = rb_struct_define( NULL, "number", "type", "value", NULL );
    rb_define_const( rb_mProtobufCoder, "Field", rb_sProtobufCoderField );

    // ProtobufCoder::EncodingError and ProtobufCoder::DecodingError exceptions
    rb_eProtobufCoderEncodingError = rb_define_class_under( rb_mProtobufCoder, "EncodingError", rb_eStandardError );
    rb_eProtobufCoderDecodingError = rb_define_class_under( rb_mProtobufCoder, "DecodingError", rb_eStandardError );

    // module methods
    rb_define_module_function( rb_mProtobufCoder, "decode", protobuf_coder_decode, 1 );
    rb_define_module_function( rb_mProtobufCoder, "decode_from_pointer", protobuf_coder_decode_from_pointer, 3 );
    rb_define_module_function( rb_mProtobufCoder, "encode", protobuf_coder_encode, 1 );

    return;
}
