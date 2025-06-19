#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <vector>

class BranchExistsError : public std::runtime_error {
public:
    explicit BranchExistsError(const std::string& branchName)
        : std::runtime_error("Branch already exists: " + branchName) {}
};

class BranchNotFoundError : public std::runtime_error {
public:
    explicit BranchNotFoundError(const std::string& branchName)
        : std::runtime_error("Branch not found: " + branchName) {}
};

class BranchMap {
private:
    std::unordered_map<std::string, std::string> branches; // branchName → commitHash
    std::string currentBranch;

public:
    explicit BranchMap(std::string defaultBranch = "main")
        : currentBranch(std::move(defaultBranch)) {
        branches[currentBranch] = ""; // Initialize default branch
    }

    void createBranch(const std::string& name, const std::string& commitHash) {
        if (name.empty()) {
            throw std::invalid_argument("Branch name cannot be empty");
        }
        if (branches.find(name) != branches.end()) {
            throw BranchExistsError(name);
        }
        branches[name] = commitHash;
    }

    void switchBranch(const std::string& name) {
        if (branches.find(name) == branches.end()) {
            throw BranchNotFoundError(name);
        }
        currentBranch = name;
    }

    void deleteBranch(const std::string& name) {
        if (name == currentBranch) {
            throw std::runtime_error("Cannot delete the current branch");
        }
        if (branches.erase(name) == 0) {
            throw BranchNotFoundError(name);
        }
    }

    std::string getCurrentBranch() const noexcept {
        return currentBranch;
    }

    std::string getCommitHash(const std::string& branchName) const {
        auto it = branches.find(branchName);
        if (it == branches.end()) {
            throw BranchNotFoundError(branchName);
        }
        return it->second;
    }

    std::vector<std::string> getAllBranchNames() const {
        std::vector<std::string> names;
        for (const auto& pair : branches) {
            names.push_back(pair.first);
        }
        return names;
    }
};
