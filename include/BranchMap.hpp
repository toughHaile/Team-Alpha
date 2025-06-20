#pragma once
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <algorithm>
#include "Commit.hpp"
#include "Logger.hpp"
#include "Diff.hpp"

using namespace std;

class BranchMap {
private:
    unordered_map<string, string> branches;
    string currentBranch = "main";

    struct MergeResult {
        unordered_map<string, string> files;
        vector<string> conflicts;
    };

    MergeResult threeWayMerge(
        const string& ourHash,
        const string& theirHash,
        const string& baseHash
    ) {
        MergeResult result;
        Commit ourCommit = Commit::load(ourHash);
        Commit theirCommit = Commit::load(theirHash);
        Commit baseCommit = baseHash.empty() ? Commit("", "", {}) : Commit::load(baseHash);

        // Collect all unique files from all three commits
        unordered_set<string> allFiles;
        auto collectFiles = [&](const Commit& commit) {
            for (const auto& [file, _] : commit.getBlobs()) {
                allFiles.insert(file);
            }
        };
        collectFiles(ourCommit);
        collectFiles(theirCommit);
        collectFiles(baseCommit);

        // Process each file
        for (const auto& file : allFiles) {
            bool inOurs = ourCommit.getBlobs().count(file);
            bool inTheirs = theirCommit.getBlobs().count(file);
            bool inBase = baseCommit.getBlobs().count(file);

            string ourContent = inOurs ? Blob::load(ourCommit.getBlobs().at(file)) : "";
            string theirContent = inTheirs ? Blob::load(theirCommit.getBlobs().at(file)) : "";
            string baseContent = inBase ? Blob::load(baseCommit.getBlobs().at(file)) : "";

            // Case 1: Added in theirs only
            if (!inOurs && inTheirs && !inBase) {
                result.files[file] = theirContent;
            }
            // Case 2: Added in ours only
            else if (inOurs && !inTheirs && !inBase) {
                result.files[file] = ourContent;
            }
            // Case 3: Modified in both.
            else if (inOurs && inTheirs) {
                if (ourContent == theirContent) {
                    result.files[file] = ourContent;
                }
                else {
                    string conflictContent;
                    if (inBase && ourContent != baseContent && theirContent != baseContent) {
                        // Real three-way merge
                        auto diffOurs = Diff::compare(baseContent, ourContent);
                        auto diffTheirs = Diff::compare(baseContent, theirContent);
                        
                        if (!hasOverlappingChanges(diffOurs, diffTheirs)) {
                            // Auto-merge non-conflicting changes
                            result.files[file] = mergeChanges(baseContent, diffOurs, diffTheirs);
                        } else {
                            conflictContent = generateConflictMarkers(ourContent, theirContent, baseContent);
                            result.conflicts.push_back(file);
                        }
                    } else {
                        conflictContent = generateConflictMarkers(ourContent, theirContent, baseContent);
                        result.conflicts.push_back(file);
                    }
                    
                    if (!conflictContent.empty()) {
                        result.files[file] = conflictContent;
                        Logger::error("CONFLICT: Merge conflict in " + file);
                    }
                }
            }
        }

        return result;
    }

    bool hasOverlappingChanges(const vector<string>& diff1, const vector<string>& diff2) {
        // Simplified check - in real implementation would analyze line ranges
        return !diff1.empty() && !diff2.empty();
    }

    string mergeChanges(const string& base, const vector<string>& diff1, const vector<string>& diff2) {
        // Simple concatenation merge - would be enhanced with proper line-based merging
        string merged = base;
        for (const auto& line : diff1) {
            if (line[0] == '+') merged += "\n" + line.substr(2);
        }
        for (const auto& line : diff2) {
            if (line[0] == '+') merged += "\n" + line.substr(2);
        }
        return merged;
    }

    string generateConflictMarkers(const string& ours, const string& theirs, const string& base) {
        return "<<<<<<< OURS\n" + ours + 
               "\n=======\n" + theirs + 
               "\n>>>>>>> THEIRS\n";
    }

public:
    // ..(existing branch management methods remain the same) ...

    string merge(const string& branchName, bool autoResolve = false) {
        if (branches.find(branchName) == branches.end()) {
            throw runtime_error("Branch not found: " + branchName);
        }

        string ourCommit = branches[currentBranch];
        string theirCommit = branches[branchName];
        string lca = Commit::findLCA(ourCommit, theirCommit);

        auto result = threeWayMerge(ourCommit, theirCommit, lca);
        
        if (!result.conflicts.empty()) {
            if (autoResolve) {
                Logger::log("Auto-resolving " + to_string(result.conflicts.size()) + " conflicts");
            } else {
                string conflictMsg = "Merge conflicts in files:\n";
                for (const auto& file : result.conflicts) {
                    conflictMsg += "  " + file + "\n";
                }
                conflictMsg += "Resolve conflicts then commit the result";
                throw runtime_error(conflictMsg);
            }
        }

        string mergeMsg = "Merge branch '" + branchName + "' into " + currentBranch;
        Commit mergeCommit = Commit::createMergeCommit(result.files, ourCommit, theirCommit);
        branches[currentBranch] = mergeCommit.getHash();
        
        return mergeCommit.getHash();
    }
};
