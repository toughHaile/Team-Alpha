#pragma once 
#include <string>
#include <openssl/sha.h>
#include <chrono>
using namespace std;
class Blob { 
public: 
static string hash(const string& content){  
 auto salt = to_string(chrono::system_clock::now().time_since_epoch().count());  
 string salted = content + salt;           
 unsigned char digest[SHA_DIGEST_LENGTH];    
 SHA1((const unsigned char*)salted.c_str(), salted.size(), digest);    
 char hex[SHA_DIGEST_LENGTH*2+1];   
 for (int i = 0; i < SHA_DIGEST_LENGTH; i++)     
  sprintf(hex + i*2, "%02x", digest[i]);       
 return string(hex);   
}    
static string load(const string& hash) {    
 ifstream file(".minigit/objects/" + hash); 
 return string((istreambuf_iterator<char>(file)),      
  istreambuf_iterator<char>());
}
};
