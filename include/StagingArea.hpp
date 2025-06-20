 #pragma once 
#include <unordered_map> 
#include <string> 
 
using namespace std; 
 
class StagingArea { 
private: 
    unordered_map<string, string> stagedFiles; // filename -> blobHash 
 
public: 
    void stage(const string& filename, const string& hash) { 
        // Unique: Prevent duplicate staging 
        if (stagedFiles.find(filename) != stagedFiles.end()) 
            throw runtime_error("File already staged: " + filename); 
        stagedFiles[filename] = hash; 
    } 
 
    const unordered_map<string, string>& getStagedFiles() const { 
        return stagedFiles; 
    } 
 
    void clear() { 
        stagedFiles.clear(); 
    } 
};
