#include "encoder.h"
#include "protobuf_coder.h"

static inline void write_varint( VALUE output, uint64_t value )
{
    uint8_t byte;

    while ( 1 )
    {
        rb_str_modify_expand( output, 1 );

        byte = value & 0x7f;
        value >>= 7;

        if ( value == 0 )
        {
            RSTRING_PTR( output )[ RSTRING_LEN( output ) - 1 ] = byte;
            break;
        }
        else
        {
            RSTRING_PTR( output )[ RSTRING_LEN( output ) - 1 ] = ( byte | 0x80 );
        }
    }
}

static inline void write_bytes( VALUE output, uint8_t *buffer, long count )
{
    rb_str_modify_expand( output, count );
    MEMCPY( RSTRING_PTR( output ) + ( RSTRING_LEN( output ) - count ), buffer, uint8_t, count );
}

VALUE protobuf_coder_encode( VALUE module, VALUE fields )
{
    Check_Type( fields, T_ARRAY );

    VALUE result = rb_str_buf_new( 0 );

    for ( long i = 0; i < RARRAY_LEN( fields ); i++ )
    {
         VALUE field = rb_ary_entry( fields, i );

         // Unpack field
         int   field_number = NUM2INT( rb_struct_aref( field, ID2SYM( rb_intern( "number" ))));
         int   field_type   = NUM2INT( rb_struct_aref( field, ID2SYM( rb_intern( "type" ))));
         VALUE field_value  = rb_struct_aref( field, ID2SYM( rb_intern( "value" )));

         uint64_t tag = (field_number << 3) | field_type;

         write_varint( result, tag );

         switch ( field_type )
         {
             case WT_VARINT:
                 write_varint( result, NUM2LL( field_value ));
                 break;

             case WT_STRING:
                 write_varint( result, RSTRING_LEN( field_value ));
                 write_bytes( result, (uint8_t *) RSTRING_PTR( field_value ), RSTRING_LEN( field_value ));
                 break;

             case WT_64BIT:
                 // Write 8 bytes
                 write_bytes( result, (uint8_t *) RSTRING_PTR( field_value ), 8 );
                 break;

             case WT_32BIT:
                 // Write 4 bytes
                 write_bytes( result, (uint8_t *) RSTRING_PTR( field_value ), 4 );
                 break;

             default:
                 rb_raise( rb_eProtobufCoderEncodingError, "invalid wire type: %d", field_type );
         }
    }

    return result;
}
