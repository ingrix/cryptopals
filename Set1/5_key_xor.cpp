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

std::vector< char> ReadFile( const char *filename)
{
  std::ifstream in( filename, std::ios::binary | std::ios::ate);
  std::streampos end_p( in.tellg());
  // seek to beginning
  in.seekg(0, std::ios::beg);
  std::vector< char> buf( end_p);
  if(!in.read(buf.data(),end_p))
  {
    buf.clear();
    std::cout << " Could not read file \"" << filename << "\"" << std::endl;
  }
  in.close();
  return buf;
}

std::vector< char> xor_encrypt( std::string message, std::string key)
{
  std::vector< char> enc;
  if(message.empty() || key.empty())
  {
    return enc;
  }
  enc.reserve(message.length());
  size_t key_len( key.length());
  for( size_t i( 0), end_i( message.length()); i < end_i; ++i)
  {
    size_t kp( i % key_len);
    enc.push_back(message[i] ^ key[kp]);
  }
  return enc;
}

int main( int argc, char **argv)
{
  if( argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " \"input\""<<std::endl;
    return -1;
  }
  std::vector< char> buf( ReadFile( argv[1]));
  std::string input( argv[1]);
  std::cout << "Read \"" << buf.data() << "\"" << std::endl;
  std::string message( buf.data());
  std::string key("ohcrap");
  std::cout << "Key: \"" << key << "\"" << std::endl;
  std::vector< char> enc( xor_encrypt( message, key));
  std::cout << "Encrypted: ";
  std::cout << std::setfill('0') << std::setw(2);
  for( size_t i( 0); i < enc.size(); ++i)
  {
    std::cout << std::hex << (int)enc[i];
  }
  std::cout << std::endl;

  return 0;

}
