#pragma once
#include <string>
#include <unordered_map>
#include <ctime>
#include <fstream>
#include "Blob.hpp"
using namespace std;

class Commit {
private:
    string commitHash;
    string parentHash;
    string message;
    time_t timestamp;
    unordered_map<string, string> blobs; // filename -> blob hash
    
    // Thread-local cache for loaded commits
    static unordered_map<string, Commit>& getCache() {
        thread_local unordered_map<string, Commit> cache;
        return cache;
    }

public:
    // Creates new commit with staged files and parent reference
    Commit(const string& msg, const string& parent, 
           const unordered_map<string, string>& stagedBlobs)
        : message(msg), parentHash(parent), timestamp(time(nullptr)) 
    {
        string commitData;
        commitData.reserve(message.size() + parentHash.size() + 20);
        commitData.append(message).append(parentHash)
                 .append(to_string(timestamp));
        
        for (const auto& [file, hash] : stagedBlobs) {
            commitData.append(file).append(hash);
        }
        
        commitHash = Blob::hash(commitData);
        blobs = stagedBlobs;
    }

    // Loads commit from object database (cached)
    static Commit load(const string& hash) {
        auto& cache = getCache();
        if (auto it = cache.find(hash); it != cache.end()) {
            return it->second;
        }

        ifstream file(".minigit/objects/" + hash);
        if (!file) throw runtime_error("Commit not found");
        
        // Parse metadata line: parentHash|timestamp|message
        string metaLine;
        getline(file, metaLine);
        size_t sep1 = metaLine.find('|');
        size_t sep2 = metaLine.rfind('|');
        
        // Parse file entries: filename|blobHash
        unordered_map<string, string> loadedBlobs;
        string line;
        while (getline(file, line)) {
            size_t sep = line.find('|');
            if (sep != string::npos) {
                loadedBlobs[line.substr(0, sep)] = line.substr(sep + 1);
            }
        }

        Commit loaded(metaLine.substr(sep2 + 1), 
                     metaLine.substr(0, sep1), 
                     loadedBlobs);
        cache[hash] = loaded;
        return loaded;
    }

    // Writes commit data to object database
    void save() const {
        ofstream file(".minigit/objects/" + commitHash);
        file << parentHash << "|" << timestamp << "|" << message << "\n";
        for (const auto& [file, hash] : blobs) {
            file << file << "|" << hash << "\n";
        }
    }

    // Finds lowest common ancestor of two commits
    static string findLCA(const string& hash1, const string& hash2) {
        unordered_set<string> visited;
        string current = hash1;
        while (!current.empty()) {
            visited.insert(current);
            current = load(current).getParent();
        }
        
        current = hash2;
        while (!current.empty()) {
            if (visited.count(current)) return current;
            current = load(current).getParent();
        }
        return "";
    }

    // Accessors
    string getHash() const { return commitHash; }
    string getParent() const { return parentHash; }
    const unordered_map<string, string>& getBlobs() const { return blobs; }
    time_t getTimestamp() const { return timestamp; }
    string getMessage() const { return message; }
}; 
