#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include "Blob.hpp"
#include "Logger.hpp"

namespace fs = std::filesystem;

class StagingArea {
private:
    unordered_map<string, string> stagedFiles;
    vector<string> removedFiles;

public:
    void stage(const string& filename) {
        if (!fs::exists(filename)) throw runtime_error("File not found");
        ifstream file(filename, ios::binary);
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        stagedFiles[filename] = Blob::hash(content);
    }

    void stageForRemoval(const string& filename) {
        removedFiles.push_back(filename);
    }

    const auto& getStagedFiles() const { return stagedFiles; }
    const auto& getRemovedFiles() const { return removedFiles; }
    void clear() { stagedFiles.clear(); removedFiles.clear(); }
};
