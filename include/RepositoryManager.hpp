#pragma once
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include "Blob.hpp"
#include "Logger.hpp"

using namespace std;

class RepositoryManager {
private:
    unordered_map<string, string> stagedFiles;
    string currentBranch = "main";

    bool dirExists(const filesystem::path& path) { 
        return filesystem::exists(path); 
    }
    
    void createDirs(const filesystem::path& path) { 
        filesystem::create_directories(path); 
    }

public:
    class RepoError : public runtime_error {
    public:
        RepoError(const string& msg) : runtime_error("[RepoManager] " + msg) {}
    };

    bool init() {
        if (dirExists(".minigit")) {
            Logger::log("Init failed: Repository exists", "System");
            throw RepoError("Already initialized");
        }

        try {
            createDirs(".minigit/objects");
            createDirs(".minigit/refs/heads");
            
            ofstream head(".minigit/HEAD");
            if (!head) throw RepoError("HEAD creation failed");
            head << "ref: refs/heads/main\n";
            
            Logger::log("Repository initialized", "System");
            return true;
        } catch (const filesystem::filesystem_error& e) {
            throw RepoError("Filesystem error: " + string(e.what()));
        }
    }

    void add(const string& filename) {
        if (!dirExists(filename)) {
            Logger::log("Add failed: " + filename, "System");
            throw RepoError("File not found");
        }

        ifstream file(filename, ios::binary);
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        string hash = Blob::hash(content + to_string(time(nullptr)));
        stagedFiles[filename] = hash;
        
        Logger::log("Staged: " + filename + " (" + hash.substr(0, 6) + ")", "System");
    }

    string getHEAD() const {
        if (currentBranch.empty()) return "";
        return "refs/heads/" + currentBranch;
    }

    const unordered_map<string, string>& getStagedFiles() const {
        return stagedFiles;
    }
}; 
