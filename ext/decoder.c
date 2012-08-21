#include "decoder.h"
#include "protobuf_coder.h"

/**
 * Reads from buffer one VARINT and shifts the pointer.
 *
 * buffer - pointer to the buffer pointer
 * buffer_size - pointer to the buffer size
 *
 * Returns decoded VARINT.
 *
 */
static inline uint64_t read_varint( uint8_t **buffer, long *buffer_size )
{
    uint64_t result = 0;
    int      shift  = 0;

    do
    {
        if ( *buffer_size <= 0 )
            rb_raise( rb_eProtobufCoderDecodingError, "can't read varint: no more data in buffer" );

        result |= (**buffer & 0x7F) << shift;
        shift += 7;

        *buffer = *buffer + 1;
        *buffer_size = *buffer_size - 1;
    }
    while ( shift < 64 && *(*buffer - 1) & 0x80 ); // One extra subtraction is ok

    return result;
}

/**
 * Reads from buffer given number of bytes and shifts the pointer.
 *
 * buffer - pointer to the buffer pointer
 * buffer_size - pointer to the buffer size
 * count - number of bytes to read
 *
 * Returns ruby string with bytes.
 *
 */
static inline VALUE read_bytes( uint8_t **buffer, long *buffer_size, long count )
{
    if ( *buffer_size < count )
        rb_raise( rb_eProtobufCoderDecodingError, "can't read %ld bytes: buffer contains only %ld bytes", count, *buffer_size );

    // Copy slice to ruby string
    VALUE result = rb_str_buf_new( count );
    rb_str_resize( result, count );
    MEMCPY( RSTRING_PTR( result ), *buffer, uint8_t, count );

    *buffer = *buffer + count;
    *buffer_size = *buffer_size - count;

    return result;
}

/**
 * Decodes protobuf message.
 *
 * buffer - pointer to the buffer with message data
 * buffer_size - size of the buffee
 *
 * Returns ruby array with decoded fields.
 *
 */
static VALUE decode_protobuf( uint8_t *buffer, long buffer_size )
{
    // Will return array of fields
    VALUE message = rb_ary_new();

    while ( buffer_size > 0 )
    {
        // New field
        VALUE field = rb_struct_new( rb_sProtobufCoderField, Qnil, Qnil, Qnil );

        // Read field tag
        uint64_t tag = read_varint( &buffer, &buffer_size );

        uint64_t field_number = tag >> 3;
        uint64_t wire_type    = tag & 0x07;

        rb_struct_aset( field, ID2SYM( rb_intern( "number" )), INT2FIX( field_number ));
        rb_struct_aset( field, ID2SYM( rb_intern( "type" )), INT2FIX( wire_type ));

        switch ( wire_type )
        {
            case WT_VARINT:
                // Read VARINT
                rb_struct_aset( field, ID2SYM( rb_intern( "value" )), INT2NUM( read_varint( &buffer, &buffer_size )));
                break;

            case WT_STRING:
                // First - read one VARINT with string length, then read "length" bytes
                rb_struct_aset( field, ID2SYM( rb_intern( "value" )), read_bytes( &buffer, &buffer_size, read_varint( &buffer, &buffer_size )));
                break;

            case WT_64BIT:
                // Read 8 bytes
                rb_struct_aset( field, ID2SYM( rb_intern( "value" )), read_bytes( &buffer, &buffer_size, 8 ));
                break;

            case WT_32BIT:
                // Read 4 bytes
                rb_struct_aset( field, ID2SYM( rb_intern( "value" )), read_bytes( &buffer, &buffer_size, 4 ));
                break;

            default:
                rb_raise( rb_eProtobufCoderDecodingError, "invalid wire type: %lld", wire_type );
        }

        rb_ary_push( message, field );
    }

    return message;
}

/**
 * Decodes protobuf message from given string.
 *
 * @param [String] string string with protobuf data
 * @return [Array] array of decoded fields
 */
VALUE protobuf_coder_decode( VALUE module, VALUE string )
{
    Check_Type( string, T_STRING );
    return decode_protobuf( (uint8_t *) RSTRING_PTR( string ), RSTRING_LEN( string ));
}

/**
 * Decodes protobuf message from givent FFI pointer.
 *
 * @param [#address] pointer pointer to the protobuf data
 * @param [Integer] offset buffer offset
 * @param [Integer] length buffer size (wo/ offset)
 * @return [Array]  array of decoded fields
 */
VALUE protobuf_coder_decode_from_pointer( VALUE module, VALUE pointer, VALUE offset, VALUE length )
{
    char *ptr = (char *) NUM2ULL( rb_funcall( pointer, rb_intern_const( "address" ), 0 ));
    return decode_protobuf( (uint8_t *) ptr + NUM2LONG( offset ), NUM2LONG( length ));
}
