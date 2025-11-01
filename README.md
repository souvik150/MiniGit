# MiniGit — In-Memory Version Control System

MiniGit is a lightweight, in-memory version control system that demonstrates how classic design patterns map onto a Git-like workflow. The project favours clean low-level design and separation of concerns over persistence or distributed features.

## At a Glance

- Pure C++17 implementation with a statically linked core library (`MiniGitCore`) and a thin CLI executable.
- In-memory repository tree built from composable filesystem nodes.
- Deterministic hooks that log every change into a `.history` file via the Observer pattern.
- Extensible strategy layer for diffing commits.
- Test harness (CTest) covering notification flows and log formatting.

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/MiniGit
```

Run the automated tests:

```bash
ctest --test-dir build --output-on-failure
```

## CLI Cheatsheet

Commands are designed to feel Git-inspired while remaining human readable:

| Command | Effect |
|---------|--------|
| `add file <name>` | Adds a new empty file to the active branch workspace. |
| `add folder <name>` | Adds a folder node to the workspace tree. |
| `commit <message>` | Captures a snapshot with the provided message. |
| `diff` | Shows the diff between the last two commits. |
| `revert <commitId | numericSuffix>` | Restores the workspace to a previous commit (numeric IDs are auto-prefixed with `commit_`). |
| `branch <name>` | Creates a branch from the current head. |
| `branch` | Lists all branches and highlights the active one. |
| `switch <name>` | Moves the workspace to another branch. |
| `log` | Prints the commit history for the active branch. |
| `exit` | Leaves the CLI loop. |

Every mutating command emits a structured notification that is appended to `.history`, e.g.:

```
[commit_hook] commit_1762023225512 add login flow
[branch_change_hook] switched feature/auth
```

The default diff strategy performs a structural tree comparison, reporting added, removed, and modified files/directories:

```
TreeDiff results:
+ docs/ (dir)
~ src/auth/service.cpp
```

<img width="2613" height="1650" alt="mermaid-diagram-2025-11-01-191740" src="https://github.com/user-attachments/assets/dfc4fadf-6f35-42fc-9cd5-33923c15408f" />

| Component | Responsibility | Notes |
|-----------|----------------|-------|
| **CLI** (`src/cli/CLI.cpp`) | Parses terminal input and delegates to `GitManager`. | Bootstraps the system exactly once, ensuring the `GitManager` singleton owns its dependencies. |
| **GitManager** (`src/cli/GitManager.cpp`) | Orchestrates all operations across branches, repositories, and observers. | Holds unique ownership (`std::unique_ptr`) of the branch database and notification service. Provides thin domain-centric helpers (`repository()`, `workingRoot()`) so higher-level commands stay clean. |
| **BranchDatabase** (`src/branching/BranchDatabase.cpp`) | Tracks all branches plus the active branch. | Throws on missing active branch, which allows `GitManager` to drop defensive null checks. |
| **Repository** (`src/repository/Repository.cpp`) | Encapsulates commit history, snapshot management, diff selection, and restore operations. | Enforces required collaborators (root snapshot + diff strategy) up front. |
| **Filesystem Nodes** (`src/core`) | Composite hierarchy (`Folder`, `File`) representing the workspace tree. | Guarantees cloning for snapshotting and supports polymorphic diffing. |
| **Notification Layer** (`src/observer`) | Publisher/subscriber infrastructure and `.history` sink. | `HistoryFileObserver` locks and appends lines with hook prefixes so multi-threaded emitters would remain safe. |

### Pattern Map & Rationale

- **Singleton (GitManager):** `GitManager::init` wires up the entire system and must exist exactly once. Enforcing a singleton keeps the CLI bootstrap simple and guarantees a single source of truth for branch state.
- **Memento (Commit snapshots):** `Repository::commit` clones the workspace tree into an immutable snapshot. The history serves as the caretaker, enabling safe restore without exposing internal structures.
- **Composite (Folder/File):** The in-memory filesystem tree treats files and folders uniformly via `IFileSystemItem`, letting commits clone or diff entire subtrees without special casing.
- **Command (CLI actions):** The CLI surface mirrors command semantics (add, commit, branch). While explicit command objects aren’t materialised yet, the design leaves clear seams to introduce them—each user action is already encapsulated in a dedicated method on `GitManager`.
- **Observer (NotificationService & HistoryFileObserver):** All state transitions broadcast hooks (commit, branch change, file/folder mutations, revert). Observers can be swapped or extended—tests register dummy observers to capture events without touching disk.
- **Strategy (DiffStrategy):** `Repository::setDiffStrategy` accepts algorithm choices (default `TreeDiffStrategy`, plus `LineDiffStrategy` and `WordDiffStrategy`). The strategy lives alongside the repository to keep diff evolution independent from commit logic.
- **Factory (BranchFactory):** `DefaultBranchFactory` encapsulates how branches materialise (fresh roots vs. cloned snapshots) while assigning the default diff strategy. Replacing it allows alternative branch bootstrap policies.

By keeping owned services (`BranchDatabase`, `NotificationService`) behind `std::unique_ptr`, MiniGit avoids implicit sharing and clarifies lifetimes—`GitManager` is the sole owner, which aligns with the “single orchestrator” mental model and simplifies testing.

## Testing Strategy

- **Observer Contract Test:** `tests/GitManagerObserverTests.cpp` exercises file/folder additions, commits, reverts, and branch mutations while verifying both captured events and `.history` formatting.
- Additional scenarios can reuse the core library (`MiniGitCore`) without spinning up the CLI, making unit-style testing straightforward.

Run `ctest --output-on-failure` after building to validate the notification pipeline end-to-end.

## Extending the System

- Swap in alternative diff strategies by implementing `IDiffStrategy` and switching via `Repository::setDiffStrategy`.
- Add new observers (e.g., analytics, webhooks) by inheriting from `IObserver` and registering them during bootstrap.
- Explore persistence by writing a branch factory that seeds repositories from disk or serialised snapshots.

---

**Author:** Souvik Mukherjee  
**License:** MIT  
