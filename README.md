# Banker's Algorithm: Deadlock Avoidance

Implementation of the Banker's Algorithm in C, kept in a GitHub repository and built automatically with GitHub Actions.

**Author:** Your Name | UID Your UID | Batch Your Batch

## What it does
- Reads the number of processes and resource types, then the Allocation matrix, the Maximum matrix and the Available vector.
- Calculates **Need = Max - Allocation**.
- Runs the **safety algorithm**, prints every step (Work vector) and the safe sequence, or reports an unsafe state.
- Handles **resource requests**: it checks Request <= Need and Request <= Available, grants the request provisionally, and keeps it only if the new state is safe. Otherwise it rolls back.

## Files
| File | Purpose |
|---|---|
| `banker.c` | the program |
| `sample_input.txt` | safe state plus three requests (granted, must wait, denied) |
| `sample_input_unsafe.txt` | an unsafe starting state |
| `sample_output.txt`, `sample_output_unsafe.txt` | output of the two samples |
| `.vscode/tasks.json` | build task for VS Code (Ctrl+Shift+B) |
| `.github/workflows/build.yml` | GitHub Actions: compiles and runs on every push |

## Run it in VS Code (Windows, needs gcc)
```
gcc -Wall -o banker banker.c
.\banker.exe
```
Run a sample without typing (PowerShell):
```
Get-Content sample_input.txt | .\banker.exe
```
Command Prompt or Linux/macOS: `banker < sample_input.txt` or `./banker < sample_input.txt`

## Sample result (default data)
Available = [3, 3, 2]. The state is **safe** with sequence **P2 -> P4 -> P5 -> P1 -> P3**.
- P2 requests [1, 0, 2]: granted, state stays safe.
- P5 requests [3, 3, 0]: must wait, not enough available.
- P1 requests [0, 2, 0]: denied, it would leave the system unsafe.

## Put it on GitHub
```
git init
git add .
git commit -m "Banker's Algorithm in C"
git branch -M main
git remote add origin https://github.com/<your-username>/banker-algorithm.git
git push -u origin main
```
Then open the **Actions** tab of the repository: the "Build and run Banker's Algorithm" workflow compiles and runs the program on GitHub's servers. You can also open the repository in **GitHub Codespaces** (Code > Codespaces) and run the same gcc commands there.
