\# GrammarParser



A C++ lexical analyzer and parser designed to analyze context-free grammars (CFGs). The engine processes input grammars from standard input and performs critical compiler design computations—including compiling Terminal/Non-Terminal appearance lists, determining symbol Nullability, computing \*\*FIRST\*\* and \*\*FOLLOW\*\* sets, and applying predictive transformations like \*\*Left-Factoring\*\*.



\## Features



\* \*\*Complete Lexical Pipeline:\*\* Fully custom `InputBuffer` and `LexicalAnalyzer` to tokenise complex grammar definitions (handles operators like `->`, `|`, `\*`, `#`).

\* \*\*Nullability Computation:\*\* Iteratively analyzes grammar rules to find all nullable non-terminals.

\* \*\*FIRST \& FOLLOW Generators:\*\* Computes full compiler-design lookup sets required for building LL(1) parsers.

\* \*\*Grammar Transformation:\*\* Includes automated modules to left-factor overlapping grammar production rules.



\---



\## Repository Structure



\* \*\*`inputbuf.h` / `inputbuf.cc`\*\*: Implements a low-level character and string stream buffer managing input data streams with single-character unget mechanics.

\* \*\*`lexer.h` / `lexer.cc`\*\*: Tokenizes grammar strings into discrete syntactic structures (`ID`, `ARROW`, `STAR`, `HASH`, `OR`).

\* \*\*`parser.h` / `parser.cpp`\*\*: Core architectural engine handling rule storage, grammar validation, and computation of analytical tasks.

\* \*\*`project2.cc`\*\*: Entry point executable that dispatches processing tasks based on command-line arguments.



\---



\## Getting Started



\### Prerequisites

\* A standard C++ compiler supporting \*\*C++11\*\* or higher (e.g., `g++` or `clang++`).

\* Make or a standard shell execution environment.



\### Compilation

Compile all core source files together via your terminal:



```bash

g++ -std=c++11 project2.cc parser.cpp lexer.cc inputbuf.cc -o grammarparser



\## How to Run



The program reads the grammar representation from standard input (`stdin`) and accepts an integer argument specifying the execution task (numbered `1` through `6`):



```bash

./grammarparser \[task\_number] < path\_to\_grammar\_file.txt



Tasks \& Supported FeaturesThe parser executes distinct tasks based on the command-line argument provided:  Task 1 (Symbols List): Prints all Terminals, followed by all Non-Terminals, strictly in their order of appearance.  Task 2 (Nullability Analysis): Computes and lists all Non-Terminals that can derive the empty string.  Task 3 (FIRST Sets): Computes the foundational FIRST sets for every Non-Terminal in the grammar.  Task 4 (FOLLOW Sets): Evaluates the grammar and generates corresponding FOLLOW token sets.  Task 5 (Left-Factoring): Rewrites and prints the grammar rules cleanly after performing structural left-factoring.  Task 6 (Eliminate Left Recursion): Contains stubs to eliminate left recursion from the grammar.

