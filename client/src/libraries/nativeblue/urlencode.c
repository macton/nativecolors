#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------------------------------------
// Utility functions 
//   - Additional functions used both internally and provided to application
// --------------------------------------------------------------------------

/* Converts a hex character to its integer value */
static char from_hex(char ch) {
  return isdigit((int)ch) ? ch - '0' : tolower((int)ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
static char to_hex(char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

// --------------------------------------------------------------------------
// int strnurlencode( char* dest, int dest_max_len, const char* str, size_t str_max_len ) 
// int strnurldecode( char* dest, int dest_max_len, const char* str, size_t str_max_len ) 
//
// Convert to/from url encoded string
// Example:
// 
//  int   encode_len = strnurlencode( NULL, 0, str, len );
//  char* encode_str = (char*)alloca( encode_len+1 );
//
//  strnurlencode( encode_str, encode_len+1, str, len );
//
//  int   decode_len = strnurlencode( NULL, 0, encode_str, encode_len+1 );
//  char* decode_str = (char*)alloca( decode_len+1 );
//
//  strnurldecode( decode_str, decode_len+1, encode_str, encode_len+1 );
//
//  if (strncmp(decode_str,str,len) != 0)
//  {
//    // -- ERROR! strings should match.
//  }
// --------------------------------------------------------------------------

int strnurlencode( char* dest, int dest_max_len, const char* str, size_t str_max_len ) 
{
  char out[ str_max_len * 3 + 1 ];
  int  len  = 0;

  for  (int i=0;i<str_max_len;i++)
  {
    char c = str[i];;

    if (c == 0)
    {
      break;
    }

    int is_alnum      = isalnum( (int)c );
    int is_dash       = (c == '-');
    int is_underscore = (c == '_');
    int is_dot        = (c == '.');
    int is_tilde      = (c == '~');
    int is_pass_char  = is_alnum || is_dash || is_underscore || is_dot || is_tilde;
    
    if ( is_pass_char )
    {
      out[ len ] = str[i];
      len++;
    }
    else
    {
      out[ len+0 ] = '%';
      out[ len+1 ] = to_hex( c >> 4 );
      out[ len+2 ] = to_hex( c & 0x0f );
      len += 3;
    }
  }

  out[ len ] = 0;

  if ( dest )
  {
    strncpy( dest, out, dest_max_len );
  }

  return len;
}

int strnurldecode( char* dest, int dest_max_len, const char* str, size_t str_max_len ) 
{
  char out[ str_max_len + 1 ];
  int  len  = 0;

  for  (int i=0;i<str_max_len;i++)
  {
    char c = str[i];;

    if (c == 0)
    {
      break;
    }

    int is_hex = (c == '%');

    if ( is_hex )
    {
      if ( i < (str_max_len-2) )
      {
        char hi_c  = str[i+1];
        char lo_c  = str[i+2];
        char hi    = from_hex( hi_c ) << 4;
        char lo    = from_hex( lo_c );
        out[ len ] = hi | lo;
        i += 2;
      }
    }
    else
    {
      out[ len ] = c;
    }
    len++;
  }

  out[ len ] = 0;

  if ( dest ) 
  {
    strncpy( dest, out, dest_max_len );
  }

  return len;
}

