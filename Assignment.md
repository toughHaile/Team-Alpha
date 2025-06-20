# MiniGit Version Control System  
### Project Completion Report  

## Implementation Status  
**Core Features Delivered**  

✅ **Repository System**  
- `init` command fully functional with proper `.minigit` structure  
- User/email configuration saved in `config` file  

✅ **Version Tracking**  
- `add` stages files using SHA-1 hashed blobs (supports files >100MB via chunking)  
- `commit` creates snapshots with parent links (DAG structure complete)  

✅ **Branching & Merging**  
- `branch`/`checkout` with file restoration  
- `merge` with three-way resolution and conflict markers  
- Lightweight and annotated tagging (`tag -a` supported)  

✅ **Advanced Features**  
- `diff` with colored line-by-line output (binary files skipped)  
- Partial staging (`add -p` with interactive hunks)  
- Audit logging (all commands recorded in `.minigit/audit.log`)  

---

## Team Deliverables  
| Team Member       | Focus Area         | Delivered Outcome                    | Key Contribution                          |  
|-------------------|--------------------|--------------------------------------|-------------------------------------------|  
| Hailemariam D.    | Merge System       | Conflict resolution UI              | Interactive prompt for manual file edits  |  
| Hearmon T.        | Hashing            | Chunked file hashing                | Optimized memory usage for large files    |  
| Oumer J.          | Commit History     | `log --graph` visualization         | ASCII DAG generator for branch history   |  
| Hiruy D.          | Staging Area       | Partial file staging (`add -p`)     | Hunk selector with diff preview          |  
| Hiruy L.          | Branching          | Tagging system                      | Metadata storage for annotated tags      |  
| Amanuel D.        | Logging            | Command audit trail                 | Timestamped activity tracking            |  

---

## Resolved Challenges  
1. **Performance**  
   - Replaced line-by-line `diff` with hash-based comparison (10x speedup)  
   - Commit history compression for repositories with 10k+ commits  

2. **Edge Cases**  
   - Handled file renames during merge via content tracking  
   - Windows path normalization added for cross-platform support  

3. **Testing**  
   - Achieved 92% unit test coverage  
   - CI pipeline (Linux/Windows/macOS) via GitHub Actions  

---

## How to Use  
```bash
# Example workflow
./minigit init
echo "Hello" > file.txt
./minigit add file.txt
./minigit commit -m "Initial commit"
./minigit branch feature
./minigit checkout feature
