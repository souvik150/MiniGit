# MiniGit — In-Memory Version Control System

MiniGit is a lightweight, in-memory version control system built to illustrate essential software design patterns through a practical, simplified Git-like experience.

## Features

- Add files and folders to a virtual file system (Composite pattern)
- Commit snapshots of the repository state (Memento pattern)
- Revert to previous commits (Command and Memento)
- Create and switch branches (Singleton and Branching model)
- Multiple diff strategies to compare commits (Strategy pattern)
- Notification hooks after commits (Observer pattern)
- Command-line interface to interact with the system

## Design Highlights

This project focuses on clear architectural design over persistence or scalability:

- **Singleton**: Manages a single instance of the GitManager orchestrator
- **Memento**: Commits act as snapshots of repository state for rollback
- **Composite**: Files and folders form a unified file tree structure
- **Command**: User actions encapsulated as command objects for extensibility
- **Observer**: Notification service to simulate hooks and listeners
- **Strategy**: Supports different algorithms for diffing files

<img width="2613" height="1650" alt="mermaid-diagram-2025-11-01-191740" src="https://github.com/user-attachments/assets/dfc4fadf-6f35-42fc-9cd5-33923c15408f" />

## Usage

Run the CLI and use commands like:

- `addFile <name> <content>`
- `addFolder <name>`
- `commit <message>`
- `diffLast`
- `revert <commitId>`
- `createBranch <name>`
- `switchBranch <name>`

## Learning Outcomes

This project is ideal for those wanting to deepen their understanding of system design patterns and their application in building real-world software architectures.

---

**Author:** Souvik Mukherjee  
**License:** MIT  
