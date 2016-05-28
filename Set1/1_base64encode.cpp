#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const std::string hex_chars( "0123456789abcdef");
static const std::string base64chars( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=");

unsigned char chars_to_hex( char high, char low)
{
  size_t p1=hex_chars.find(tolower(high));
  size_t p2=hex_chars.find(tolower(low));
  assert( p1 != std::string::npos);
  assert( p2 != std::string::npos);
  return p1 << 4 | p2;
}

std::vector< unsigned char> str_to_hex( std::string str)
{
  std::vector< unsigned char> bytes;
  if( str.length() % 2)
  {
    std::cout << "String must be 0-padded hexadecimal" << std::cout;
    return bytes;
  }
  for( size_t i( 0), end( str.length() - 1); i < end; i+=2)
  {
    bytes.push_back( chars_to_hex( str[i], str[i+1]));
  }
  return bytes;
}

std::vector< unsigned char> b64encode( std::vector< unsigned char> q)
{
  std::vector< unsigned char> result;
  int padding = q.size() % 3;
  for( int i( 0); i < padding; ++i)
  {
    q.push_back( (unsigned char)0x00);
  }
  
  for( int i( 0); i < q.size(); i+=3)
  {
    unsigned char b[ 3];
    b[0]=q[i];
    b[1]=q[i+1];
    b[2]=q[i+2];
    result.push_back(b[0]>>2);
    result.push_back(((b[0]&0x3) << 4)|(b[1]>>4));
    result.push_back(((b[1]&0x0F) << 2)|(b[2]>>6));
    result.push_back(b[2]&0x3F);

  }

  return result;
}


int main()
{
  std::string query( "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
  std::vector< unsigned char> hex_query( str_to_hex( query));
  std::vector< unsigned char> res( b64encode( hex_query));

  for( size_t i( 0); i < hex_query.size(); ++i)
  {
    printf("%02x",hex_query[i]);
  }
  printf("\n");

  for( size_t i( 0); i < res.size(); ++i)
  {
    printf("%c", base64chars[res[i]]);
  }
  printf("\n");
  return 0;

}
