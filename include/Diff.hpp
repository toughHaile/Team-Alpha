#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "Blob.hpp"

using namespace std;

class Diff {
public:
    struct Edit {
        enum Type { KEEP, INSERT, DELETE } type;
        string content;
    };

    static vector<string> compare(const string& oldContent, const string& newContent) {
        vector<string> result;
        auto edits = computeEdits(oldContent, newContent);
        
        for (const auto& edit : edits) {
            switch (edit.type) {
                case Edit::INSERT:
                    result.push_back("+ " + edit.content);
                    break;
                case Edit::DELETE:
                    result.push_back("- " + edit.content);
                    break;
                case Edit::KEEP:
                    result.push_back("  " + edit.content);
                    break;
            }
        }
        
        return result;
    }

    static string coloredDiff(const string& oldContent, const string& newContent) {
        auto edits = computeEdits(oldContent, newContent);
        string result;
        
        for (const auto& edit : edits) {
            switch (edit.type) {
                case Edit::INSERT:
                    result += "\033[32m+" + edit.content + "\033[0m\n"; // Green
                    break;
                case Edit::DELETE:
                    result += "\033[31m-" + edit.content + "\033[0m\n"; // Red
                    break;
                case Edit::KEEP:
                    result += " " + edit.content + "\n";
                    break;
            }
        }
        
        return result;
    }

private:
    static vector<Edit> computeEdits(const string& oldStr, const string& newStr) {
        vector<string> oldLines = splitLines(oldStr);
        vector<string> newLines = splitLines(newStr);
        
        vector<vector<int>> dp(oldLines.size() + 1, vector<int>(newLines.size() + 1));
        
        // Build DP table for LCS
        for (int i = 1; i <= oldLines.size(); i++) {
            for (int j = 1; j <= newLines.size(); j++) {
                if (oldLines[i-1] == newLines[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        
        // Backtrack to find edits
        vector<Edit> edits;
        int i = oldLines.size(), j = newLines.size();
        
        while (i > 0 || j > 0) {
            if (i > 0 && j > 0 && oldLines[i-1] == newLines[j-1]) {
                edits.push_back({Edit::KEEP, oldLines[i-1]});
                i--;
                j--;
            } 
            else if (j > 0 && (i == 0 || dp[i][j-1] >= dp[i-1][j])) {
                edits.push_back({Edit::INSERT, newLines[j-1]});
                j--;
            }
            else if (i > 0 && (j == 0 || dp[i][j-1] < dp[i-1][j])) {
                edits.push_back({Edit::DELETE, oldLines[i-1]});
                i--;
            }
        }
        
        reverse(edits.begin(), edits.end());
        return edits;
    }

    static vector<string> splitLines(const string& str) {
        vector<string> lines;
        size_t start = 0, end = 0;
        
        while ((end = str.find('\n', start)) != string::npos) {
            lines.push_back(str.substr(start, end - start));
            start = end + 1;
        }
        
        if (start < str.length()) {
            lines.push_back(str.substr(start));
        }
        
        return lines;
    }
};
