#pragma once
#include <string>
#include <vector>
#include "Blob.hpp"

using namespace std;

class Diff {
public:
    static vector<string> compare(const string& hash1, const string& hash2) {
        vector<string> changes;
        // Implementation would:
        // 1. Load both commits
        // 2. Compare blob contents line-by-line
        // 3. Return unified diff format
        return changes;
    }
};
