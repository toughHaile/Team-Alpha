#pragma once 
#include <string>
#include <openssl/sha.h>
#include <chrono>
#include <array>
using namespace std;
class Blob {
private: 
// Precomputed hex digits for fast hash conversion    
static constexpr array<char, 16> hexdigits =
{        '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'  
};
public: 
// Generates unique SHA-1 hash of content with timestamp salt    
static string hash(const string& content) {       
 SHA_CTX ctx;   
 SHA1_Init(&ctx);                
 thread_local string salt = to_string(   
 chrono::high_resolution_clock::now()         
 .time_since_epoch().count() 
 );      
 SHA1_Update(&ctx, content.c_str(), content.size());  
 SHA1_Update(&ctx, salt.c_str(), salt.size()); 
 array<unsigned char, SHA_DIGEST_LENGTH> digest;   
 SHA1_Final(digest.data(), &ctx);  
 // Convert binary digest to hex string  
 string hex(SHA_DIGEST_LENGTH * 2, '\0');
 for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {  
  hex[i*2]   = hexdigits[digest[i] >> 4];
  hex[i*2+1] = hexdigits[digest[i] & 0x0F]; 
 }              
 return hex;    } 
// Loads blob content from object database 
static string load(const string& hash) {  
ifstream file(".minigit/objects/" + hash, ios::binary);    
 return {istreambuf_iterator<char>(file), 
  istreambuf_iterator<char>()};
 
}
};
