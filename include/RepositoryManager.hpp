#pragma once
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include "Blob.hpp"
#include "Commit.hpp"
using namespace std;

class RepositoryManager {
private:
    unordered_map<string, string> stagedFiles;
    unordered_map<string, string> fileHashes; // Cache for file changes
    string currentBranch = "main";

    // Filesystem check that doesn't throw
    bool pathExists(const filesystem::path& p) noexcept {
        error_code ec;
        return filesystem::exists(p, ec);
    }

public:
    // Custom exception for repository errors
    class RepoError : public runtime_error {
    public: 
        RepoError(const string& msg) : runtime_error("[RepoManager] " + msg) {}
    };

    // Initializes new repository structure
    bool init() {
        if (pathExists(".minigit")) {
            throw RepoError("Already initialized");
        }

        try {
            filesystem::create_directories(".minigit/objects");
            filesystem::create_directories(".minigit/refs/heads");
            
            ofstream head(".minigit/HEAD");
            if (!head) throw RepoError("HEAD creation failed");
            head << "ref: refs/heads/main\n";
            
            return true;
        } catch (const filesystem::filesystem_error& e) {
            throw RepoError("Filesystem error: " + string(e.what()));
        }
    }

    // Stages file changes with content hashing
   void add(const string& filename) {
    // Validate file existence (noexcept filesystem check)
    if (!pathExists(filename)) {
        throw RepoError("File not found: " + filename);
    }

    // Compute hash only if file changed (optimization)
    string currentHash = [&]() {
        ifstream file(filename, ios::binary);
        if (!file) throw RepoError("Cannot open file: " + filename);
        
        // Read content and hash with timestamp salt
        string content((istreambuf_iterator<char>(file)), 
                     istreambuf_iterator<char>());
        return Blob::hash(content + to_string(time(nullptr)));
    }();
    
    // Skip staging if unchanged (performance)
    if (fileHashes[filename] != currentHash) {
        stagedFiles[filename] = currentHash;
        fileHashes[filename] = currentHash;
        Logger::log("Staged: " + filename + " (" + currentHash.substr(0, 6) + ")");
    }
}

    // Displays commit history from HEAD
    void log() const {
        string current = getHEAD();
        while (!current.empty()) {
            Commit commit = Commit::load(current);
            cout << "commit " << commit.getHash() << "\n"
                 << "Date: " << ctime(&commit.timestamp)
                 << "    " << commit.getMessage() << "\n\n";
            current = commit.getParent();
        }
    }

    // Accessors
    string getHEAD() const { 
        return currentBranch.empty() ? "" : "refs/heads/" + currentBranch; 
    }
    const unordered_map<string, string>& getStagedFiles() const { 
        return stagedFiles; 
    }
};
