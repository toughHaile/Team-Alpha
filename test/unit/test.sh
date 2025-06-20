#!/bin/bash
# Initialize
./minigit init

# Test basic workflow
echo "Hello" > file.txt
./minigit add file.txt
./minigit commit -m "First commit"

# Test branching
./minigit branch dev
./minigit checkout dev
echo "World" >> file.txt
./minigit commit -m "Dev commit"

# Test merge
./minigit checkout main
./minigit merge dev  # Should show conflict if file.txt changed in both

# View log
./minigit log
