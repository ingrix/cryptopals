#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sstream>
#include <algorithm>

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

std::string hex_to_str( std::vector< unsigned char> v)
{
  std::stringstream ss;
  for( size_t i( 0); i < v.size(); ++i)
  {
    if( isprint(v[i]))
    {
      ss << v[i];
    }
  }
  return ss.str();
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

int score_v( std::vector< unsigned char> v)
{
  int score( 0);
  for( size_t i( 0); i < v.size(); ++i)
  {
    if( isalpha( v[i]))
    {
      ++score;
    }
  }
  return score;
}

int score_v_2( std::vector< unsigned char> v)
{
  int score( 0);
  std::string freq("ETAOIN SRHLDCU");
  std::vector< int> counts( freq.length(), 0);
  for( size_t i( 0); i < v.size(); ++i)
  {
    unsigned char up(toupper(v[i]));
    if(freq.find(up) != std::string::npos)
    {
      ++score;
    }
    else if(!isalpha(v[i]))
    {
      --score;
    }
  }
  return score;
}

int main()
{
  std::string q1( "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
  std::vector< unsigned char> hex_q1( str_to_hex( q1));
  std::vector< unsigned char> best_v;
  std::string best_str;
  int best_score( 0);
  for( unsigned char c( 0x00); c < 0xFF; ++c)
  {
    std::vector< unsigned char> single_char( hex_q1.size(), (unsigned char) c);
    std::vector< unsigned char> xor_res( xor_vector( hex_q1, single_char));

    std::string res_str( hex_to_str( xor_res));
    //size_t num_E( std::count( res_str.begin(), res_str.end(), 'E'));
    int score = score_v( xor_res);
    //int score = score_v_2( xor_res);
    if( score > best_score)
    {
      std::cout << " Intermediate res (char=" << (int)c << ") : " << score << ", "  << res_str << std::endl;
      best_score = score;
      best_v = xor_res;
      best_str = res_str;
    }
  }
  std::cout << " Best string: " << best_str << std::endl;
  printf("\n");
  return 0;

}
