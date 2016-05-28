#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <fstream>

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
  std::string freq("ETAOIN SRHLDCU");
  std::vector< int> counts( freq.length(), 0);
  for( size_t i( 0); i < v.size(); ++i)
  {
    unsigned char up( toupper(v[i]));
    int p( freq.find(up));
    if( p != std::string::npos)
    {
      score += (freq.length() - p + 1);
    }
    else if(!isalpha(up))
    {
      --score;
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

std::vector< unsigned char> single_char_xor( std::vector< unsigned char> v, unsigned char c)
{
  std::vector< unsigned char> single_char( v.size(), (unsigned char) c);
  return xor_vector( v, single_char);
}

std::pair< std::vector< unsigned char>, int> get_best_char_xor( std::vector< unsigned char> v)
{
  std::pair< std::vector< unsigned char>, int> best( std::vector< unsigned char>(), 0);
  for( int i( 0); i <= 0xFF; ++i)
  {
    unsigned char c( i);
    std::vector< unsigned char> new_v( single_char_xor( v, c));
    //int new_score( score_v( new_v));
    int new_score( score_v_2( new_v));
    if( new_score > best.second)
    {
      best.first = new_v;
      best.second = new_score;
    }
  }
  return best;
}

int main()
{
  std::ifstream in;
  in.open("4_input.txt");
  int best_score_diff = 0;
  std::string best_str;
  std::string best_str_xor;
  while( !in.eof())
  {
    std::string str;
    std::getline( in, str);
    std::vector< unsigned char> encoded_str( str_to_hex( str));
    int base_score( score_v( encoded_str));
    std::pair< std::vector< unsigned char>, int> best( get_best_char_xor( encoded_str));
    if( best.second > base_score)
    {
      //std::cout << hex_to_str( best.first) << " score " << best.second << std::endl;
    }
    int diff( best.second - base_score);
    //if( diff > best_score_diff)
    if( best.second > best_score_diff)
    {
      best_score_diff = best.second;
      best_str = hex_to_str( encoded_str);
      best_str_xor = hex_to_str( best.first);
      std::cout << " Intermediate candidate (score : " << best.second << "): " << best_str << std::endl;
      std::cout << "  Originally encoded as " << best_str_xor << std::endl;
    }

  }
  std::cout << " Best candidate (score : " << best_score_diff << "): " << best_str << std::endl;
  std::cout << "  Originally encoded as " << best_str_xor << std::endl;
  return 0;

}
