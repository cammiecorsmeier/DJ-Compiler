# DJ Compiler (djc)

## Overview
This project implements a compiler for the **Diminished Java (DJ)** programming language. It includes lexical analysis, parsing, abstract syntax tree (AST) construction, type checking, and code generation for execution on the **Diminished Instruction Set Machine (DISM)**. The compiler processes DJ source code (`.dj` files) and produces corresponding DISM code (`.dism` files).

## Features
- **Lexical Analysis (Assignment II)**
  - Implements a **DJ lexer** using `flex`
  - Recognizes all valid DJ tokens
  - Prints tokenized output for debugging

- **Parsing (Assignment III)**
  - Implements a **DJ parser** using `bison`
  - Uses a **context-free grammar (CFG)** to define DJ’s syntax
  - Ensures **operator precedence and associativity**

- **AST Construction (Assignment IV)**
  - Builds **Abstract Syntax Trees (ASTs)** from parsed DJ code
  - Outputs a formatted AST representation

- **Type Checking (Assignment V)**
  - Implements a **symbol table** for variable and class scope management
  - Ensures **type correctness** of DJ programs

- **Code Generation (Assignment VI)**
  - Converts DJ ASTs into **DISM assembly code**
  - Implements **short-circuit evaluation** and **null-pointer checks**
  - Produces `.dism` files for execution in `sim-dism`

## Installation & Compilation
### Prerequisites
- **Linux environment** (e.g., `cselx##.csee.usf.edu` machines)
- **GCC** version compatible with `bison` and `flex`

### Compilation Steps
```sh
# Step 1: Generate the lexer
flex dj.l

# Step 2: Generate the parser
bison -v dj.y

# Step 3: Compile the compiler
sed -i '/extern YYSTYPE yylval/d' dj.tab.c

# If including all components:
make
```

## Usage
### Running the Compiler
```sh
./djc source.dj
```
- This compiles `source.dj` into `source.dism`.
- If errors exist, the compiler reports them and exits.

### Running the DISM Code
```sh
./sim-dism source.dism
```
- Executes the compiled DISM code.

## Example Execution
```sh
$ ./djc good1.dj
$ ./sim-dism good1.dism
Simulation completed with code 0 at PC=13.
```

## Test Files
To verify the correctness of the compiler, the following test cases are included:

### **Valid DJ Programs:**
- `good1.dj` – Basic syntax, variable declaration, and assignment
- `good2.dj` – Expressions and function calls
- `good3.dj` – Loops and conditionals

### **Invalid DJ Programs:**
- `bad1.dj` – Syntax errors
- `bad2.dj` – Type errors
- `bad3.dj` – Undeclared variables, missing semicolons

### **Expected DISM Output:**
- `good1.dism` – Compiled output of `good1.dj`
- `good2.dism` – Compiled output of `good2.dj`
- `good3.dism` – Compiled output of `good3.dj`

## Error Handling
- **Lexical/Syntax errors**: Reported with accurate line numbers.
- **Type errors**: Ensures proper type-checking rules.
- **Runtime errors**:
  - Detects **null-pointer dereferencing**.
  - Ensures **stack and heap memory constraints**.

## References
- Assignment details: [USF Compilers Course](http://www.cse.usf.edu/~ligatti/compilers/24/)
- DISM Simulator: [DISM Resources](http://www.cse.usf.edu/~ligatti/compilers/24/as1/dism/sim-dism/)

