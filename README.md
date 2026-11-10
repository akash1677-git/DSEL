# DSEL — Data Security Engine for LLMs

A C-based prompt security prototype designed to identify suspicious prompt injection patterns and exposed credential-like strings using signature-based detection and risk scoring.

## Overview

DSEL (Data Security Engine for LLMs) analyses incoming prompts and classifies them into three security decisions:

- **ALLOW** — No known threat patterns detected.
- **FLAG** — A known injection signature was detected.
- **BLOCK** — The calculated risk score reached the blocking threshold.

The project demonstrates the use of fundamental data structures and algorithms in a practical security-oriented application.

## Features

- 48 predefined prompt injection signatures.
- 8 injection categories.
- Case-insensitive signature matching.
- Credential-like string detection.
- Risk-based threat classification.
- Interactive prompt processing.
- Input validation and memory cleanup.

## Data Structures Used

| Data Structure | Application |
|---|---|
| Hash Table | Stores predefined security rules |
| Stack | Accumulates risk scores |
| Circular Queue | Processes incoming prompts |
| Compressed Suffix Tree | Searches for known injection signatures |

## Injection Categories

1. Instruction Override
2. System Prompt Extraction
3. Safety Bypass
4. Role Manipulation
5. Jailbreak Attempts
6. Data Extraction
7. Embedded Instructions
8. Authority Manipulation

## Risk Scoring

| Detection | Risk Points |
|---|---:|
| Each distinct injection category | 40 |
| Credential-like pattern detected | 60 |

Multiple signatures belonging to the same category are counted only once.

### Decision Thresholds

| Risk Score | Decision |
|---|---|
| 0 | ALLOW |
| 1–59 | FLAG |
| 60 or above | BLOCK |

## Project Structure

```text
DSELG/
├── src/
│   ├── main.c
│   ├── detector/
│   ├── hash/
│   ├── stack/
│   ├── queue/
│   ├── suffixtree/
│   └── rules/
├── tests/
│   ├── detector/
│   ├── edge_test/
│   ├── hash/
│   ├── queue/
│   ├── stack/
│   └── suffixtree/
├── benchmarks/
├── comparison/
├── build/
├── .gitignore
└── README.md
```

## Compilation

Requires GCC and a C development environment.

Run the following command from the project root:

```bash
gcc -Wall -Wextra -fanalyzer -g \
src/main.c \
src/detector/detector.c \
src/hash/hash_table.c \
src/stack/stack.c \
src/queue/queue.c \
src/suffixtree/suffix_tree.c \
src/rules/rules.c \
-Isrc/detector \
-Isrc/hash \
-Isrc/stack \
-Isrc/queue \
-Isrc/suffixtree \
-Isrc/rules \
-o build/dsel.exe
```

## Running

On Windows PowerShell:

```powershell
.\build\dsel.exe
```

## Testing

The project includes separate test suites for:

- Detector functionality
- Edge cases
- Hash table
- Stack
- Queue
- Suffix tree
- Suffix tree stress testing

Previously recorded results:

- 39/39 functional tests passed.
- 10,000 suffix tree construction, search and cleanup cycles passed.

## Limitations

DSEL currently uses predefined signatures and heuristic credential detection. It does not understand the semantic meaning of every prompt and may miss novel or obfuscated injection attempts.

It is an educational security prototype, not a production-grade LLM security firewall.

## Technologies

- C
- GCC
- Data Structures and Algorithms
- Signature-Based Detection
- Static Analysis

## License

To be added.