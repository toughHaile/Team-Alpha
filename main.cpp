#include <iostream>
#include <string>
#include <vector>
#include "RepositoryManager.hpp"
#include "Commit.hpp"
#include "BranchMap.hpp"
#include "StagingArea.hpp"
#include "Diff.hpp"
#include "Logger.hpp"

using namespace std;

void printHelp() {
    cout << "MiniGit - A minimal version control system\n"
         << "Usage: minigit <command> [options]\n\n"
         << "Commands:\n"
         << "  init               Initialize new repository\n"
         << "  add <file>         Stage file for commit\n"
         << "  commit -m <msg>    Commit staged files\n"
         << "  branch [name]      List/create branches\n"
         << "  checkout <branch>  Switch branches\n"
         << "  merge <branch>     Merge branch into current\n"
         << "  log                Show commit history\n"
         << "  status             Show changed/staged files\n"
         << "  help               Show this help\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    string command = argv[1];
    RepositoryManager repoManager;
    BranchMap branchMap;
    StagingArea stagingArea;

    try {
        if (command == "init") {
            repoManager.init();
            branchMap.createBranch("main", "");
            cout << "Initialized empty MiniGit repository\n";
        }
        else if (command == "add") {
            if (argc < 3) throw runtime_error("No file specified");
            for (int i = 2; i < argc; i++) {
                string filename = argv[i];
                string content = Blob::load(filename);
                string hash = Blob::hash(content);
                stagingArea.stage(filename, hash);
            }
        }
        else if (command == "commit") {
            if (argc < 4 || string(argv[2]) != "-m") 
                throw runtime_error("Commit message required (-m)");
            
            string message = argv[3];
            const auto& stagedFiles = stagingArea.getStagedFiles();
            if (stagedFiles.empty()) throw runtime_error("No changes staged");
            
            string parentHash = branchMap.getCurrentBranch().empty() ? "" : 
                              branchMap.getBranchHead(branchMap.getCurrentBranch());
            
            Commit commit(message, parentHash, stagedFiles);
            commit.save();
            stagingArea.clear();
            
            cout << "[" << commit.getHash().substr(0, 6) << "] " << message << "\n";
        }
        else if (command == "branch") {
            if (argc == 2) {
                // List branches
                cout << "Available branches:\n";
                for (const auto& branch : branchMap.listBranches()) {
                    cout << (branch == branchMap.getCurrentBranch() ? "* " : "  ") 
                         << branch << "\n";
                }
            } 
            else {
                // Create new branch
                string newBranch = argv[2];
                string currentHead = branchMap.getBranchHead(branchMap.getCurrentBranch());
                branchMap.createBranch(newBranch, currentHead);
                cout << "Created branch " << newBranch << "\n";
            }
        }
        else if (command == "checkout") {
            if (argc < 3) throw runtime_error("Branch name required");
            string target = argv[2];
            branchMap.checkout(target);
            cout << "Switched to branch '" << target << "'\n";
        }
        else if (command == "merge") {
            if (argc < 3) throw runtime_error("Branch to merge required");
            string otherBranch = argv[2];
            string newCommit = branchMap.merge(otherBranch);
            cout << "Merged " << otherBranch << " into " 
                 << branchMap.getCurrentBranch() << "\n";
            cout << "New commit: " << newCommit.substr(0, 6) << "\n";
        }
        else if (command == "log") {
            repoManager.log();
        }
        else if (command == "status") {
            cout << "On branch " << branchMap.getCurrentBranch() << "\n\n";
            cout << "Staged changes:\n";
            for (const auto& [file, _] : stagingArea.getStagedFiles()) {
                cout << "  " << file << "\n";
            }
        }
        else if (command == "help") {
            printHelp();
        }
        else {
            throw runtime_error("Unknown command: " + command);
        }
    } 
    catch (const exception& e) {
        Logger::error(e.what());
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
} 
