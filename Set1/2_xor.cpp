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

std::vector< unsigned char> xor_vector( std::vector< unsigned char> q1, std::vector< unsigned char> q2)
{
  std::vector< unsigned char> res;
  if( q1.size() != q2.size())
  {
    return res;
  }

  res = std::vector< unsigned char>( q1.size());
  for( size_t i( 0); i < q1.size(); ++i)
  {
    res[i]=q1[i]^q2[i];
  }
  return res;
}

int main()
{
  std::string q1( "1c0111001f010100061a024b53535009181c");
  std::string q2( "686974207468652062756c6c277320657965");
  std::vector< unsigned char> hex_q1( str_to_hex( q1));
  std::vector< unsigned char> hex_q2( str_to_hex( q2));
  std::vector< unsigned char> res( xor_vector( hex_q1, hex_q2));

  for( size_t i( 0); i < res.size(); ++i)
  {
    printf("%02x",res[i]);
  }
  printf("\n");
  return 0;

}
