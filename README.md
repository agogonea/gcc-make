# GCC and Makefiles — Hands-on Session

This repository contains a set of progressive exercises designed to introduce the GCC compiler and GNU Make on Linux.

The goal is to understand not only how to compile C programs, but also what happens during the build process and how Makefiles automate repetitive compilation tasks.

## Learning Objectives

By the end of this lab, you should be able to:

- Compile and link C programs using GCC.
- Understand the main stages of the compilation process.
- Recognize the difference between compiler warnings and errors.
- Use compiler flags such as `-Wall`, `-Wextra`, `-I`, and `-o`.
- Understand why some programs require additional libraries during linking.
- Compile programs split across multiple source files.
- Read, understand, and write basic Makefiles.
- Understand targets, prerequisites, recipes, and variables.
- Use Make to avoid unnecessary recompilation.
- Build programs that use separate source and header directories.

## Prerequisites

You will need:

- A Linux environment.
- GCC.
- GNU Make.
- A terminal and a text editor.

Check that GCC and Make are installed:

```bash
gcc --version
make --version
```

If either command is unavailable, install the corresponding package using your distribution's package manager.

```bash
sudo apt install build-essential
```

## Introductory Theory

### 1. What is GCC?

GCC stands for **GNU Compiler Collection**. It is one of the most widely used toolchains on Linux for compiling C, C++, and several other programming languages.

Although we often say “compile the program with GCC,” building an executable normally involves several stages.

### 2. The Four Main Build Stages

1. **Preprocessing (`-E`)**  
   The preprocessor handles directives beginning with `#`, such as `#include` and `#define`, and expands macros. The result is expanded C source code.

2. **Compilation (`-S`)**  
   The compiler translates the preprocessed C code into assembly language for the target architecture. The output is usually a `.s` file.

3. **Assembly (`-c`)**  
   The assembler translates assembly instructions into machine-code data and produces an object file, usually `.o` on Linux. An object file is not usually a complete executable: some symbols may still need to be resolved.

4. **Linking**  
   The linker combines object files and resolves references to functions and data, including references to libraries. The result is an executable when all required symbols can be resolved.

GCC can run all these stages with a single command. The options above let you stop at an intermediate stage and inspect the result.

### 3. Useful GCC Options

| Option | Purpose |
| --- | --- |
| `-o <name>` | Sets the name of the output file. Without it, GCC commonly produces `a.out` for an executable. |
| `-Wall` | Enables a broad set of useful compiler warnings. |
| `-Wextra` | Enables additional warnings beyond those selected by `-Wall`. |
| `-g` | Includes debugging information, useful when debugging with tools such as `gdb`. |
| `-I<dir>` | Adds a directory to the search path for header files included by the source code. |
| `-L<dir>` | Adds a directory to the library search path used by the linker. |
| `-l<name>` | Requests a library during linking. For example, `-lm` requests the math library on typical Linux systems. |

**Good practice:** compile with `-Wall -Wextra` and investigate the warnings. These options do not detect every possible problem, and warnings are not errors unless you explicitly configure the compiler to treat them as errors.

A header file declares interfaces; it does not necessarily provide the compiled implementation. That is why a program can compile but fail during linking if a required function or library cannot be found.

### 4. Why Do We Need Make?

For a project with many source files, remembering and rerunning every GCC command becomes tedious. GNU Make reads a file usually named `Makefile` and decides which build commands need to run based on the targets, prerequisites, and file modification times described by its rules.

This supports **incremental builds**: when the dependencies are described correctly, Make can rebuild only the outputs affected by a change.

### 5. Anatomy of a Makefile Rule

A typical rule has this structure:

```makefile
target: prerequisites
<TAB>recipe
```

- **Target:** the file or action the rule describes.
- **Prerequisites:** files or targets that must be up to date before the recipe runs.
- **Recipe:** the shell command or commands Make executes to build the target.
- **TAB:** recipe lines must begin with a tab in a traditional Makefile; spaces alone will usually cause an error.

A target named `clean` commonly removes generated files. Since it usually does not create a file literally named `clean`, it is commonly declared as a `.PHONY` target.

Make only makes correct incremental-build decisions when the prerequisites accurately describe the dependencies. A missing dependency can cause Make to leave an output out of date without rebuilding it.

---

## General Workflow

**For every exercise, follow these steps in order.**

1. **Compile manually with GCC.** Do not use Make yet. Start from the source files and compile the program directly from the terminal.
2. **Inspect the result.** Read compiler warnings and errors carefully. If compilation succeeds, run the executable and observe its behavior.
3. **Understand the build process.** Identify which source files, header files, and libraries are involved.
4. **Analyze the Makefile.** Read the existing rules and determine how they correspond to the manual compilation command.
5. **Experiment with Make.** Run `make`, modify a source file, run `make` again, and observe what gets rebuilt.
6. **Clean up.** Use the available `clean` target and verify that generated executables or object files are removed.

Do not skip the manual compilation step. Understanding the command that builds a program makes it much easier to understand what a Makefile automates.

---

## Exercise 1 — Single-File Program

Directory: [`01-single-file/`](01-single-file/)

This exercise introduces GCC, compiler warnings, the stages of compilation, and the distinction between compilation and linking.

### Step 1: Compile manually

Navigate to the exercise directory:

```bash
cd 01-single-file
```

Try compiling the program without additional warning flags:

```bash
gcc main.c -o program
```

Observe the output. Does the command succeed? Are any warnings displayed?

Next, enable `-Wall`:

```bash
gcc -Wall main.c -o program
```

Then enable additional warnings with `-Wextra`:

```bash
gcc -Wall -Wextra main.c -o program
```

Compare the output of the commands. Investigate the warnings and identify which parts of the source code cause them.

> Note: warnings are not errors by default. GCC may still produce an executable even when it reports warnings. The exact warnings can vary by GCC version and configuration.

### Step 2: Enable the math example

Uncomment the parts of `main.c` that use `<math.h>` and `pow()`, then compile again:

```bash
gcc -Wall -Wextra main.c -o program
```

If linking fails, inspect the error message. Identify the unresolved symbol and consider why including a header does not necessarily provide the implementation of a function.

Experiment with the GCC command until the program builds successfully. On typical Linux systems, the math library can be linked with `-lm`:

```bash
gcc -Wall -Wextra main.c -o program -lm
```

Once it builds, run it:

```bash
./program
```

Provide two numbers when prompted and observe the output.

### Step 3: Explore the compilation stages

GCC normally runs several stages behind the scenes. You can ask it to stop after a particular stage and save the intermediate output.

Run these commands from the `01-single-file/` directory. For the most useful comparison, do this after enabling the math example.

#### 3.1 Preprocessing

```bash
gcc -E main.c -o main.i
cat main.i
```

The `-E` option runs only the preprocessor. The output includes expanded header contents and processed macros. The resulting file may be very long because standard headers expand into many lines.

Look for the expansion of `GREETING` and observe what happens to the `#include` directives.

#### 3.2 Compilation to assembly

```bash
gcc -S main.c -o main.s
cat main.s
```

The `-S` option preprocesses and compiles the source into assembly language, but does not assemble or link it.

Inspect the output. Can you find references corresponding to your functions or calls to library functions?

Assembly syntax and symbol names depend on the target architecture and compiler version.

#### 3.3 Assembly to an object file

```bash
gcc -c main.c -o main.o
file main.o
nm main.o
```

The `-c` option compiles and assembles the source file into an object file, but does not link an executable.

An object file is binary, so `cat main.o` is not a useful way to inspect it. Instead:

- `file` identifies the file format and architecture.
- `nm` lists symbols defined or referenced by the object file.
- Optionally, use `objdump -d main.o` to inspect its disassembly.

Look for symbols that are defined in your source and symbols that still need to be resolved by the linker.

#### 3.4 Linking

First, try linking the object file without explicitly adding the math library:

```bash
gcc main.o -o program
```

If this fails because `pow` cannot be resolved, link with the math library:

```bash
gcc main.o -o program -lm
```

The `-l` option asks the linker to use a library; `-lm` selects the math library on typical Linux systems. Keeping the library option after the object file is a good habit, because linkers commonly process inputs from left to right.

Run the resulting executable:

```bash
./program
```

### Step 4: Connect the stages

Discuss the following questions:

- What does the preprocessor do with `#include` and `#define`?
- What is the difference between a source file, an assembly file, and an object file?
- Why can the compiler accept a call to `pow()` while the linker still fails?
- What does `-lm` change?
- Why are compiler warnings different from linker errors?
- Which stage does each of `-E`, `-S`, and `-c` stop after?

The overall process can be summarized as:

```text
main.c --preprocess--> main.i --compile--> main.s
       --assemble--> main.o --link--> program
```

GCC can run all the required stages in one command, but these options let you inspect the intermediate results.

### Step 5: Analyze the Makefile

Open `Makefile` and answer the following questions:

- What does the `all` target represent?
- What is the purpose of the `program` target?
- What does `main.c` mean in the prerequisites of `program`?
- What does the recipe passed to GCC do?
- Why is there a `clean` target?
- How could you modify the Makefile to use the warning flags and any additional linker options required by the program?

Run the Makefile and compare its behavior with your manual compilation command.

### Topics to discuss

- Preprocessing, compilation, assembly, and linking.
- Compiler warnings versus errors.
- The purpose of `-Wall` and `-Wextra`.
- Header files versus library implementations.
- The purpose of `-E`, `-S`, `-c`, and `-o`.
- Inspecting text output, object files, and symbols.

---

## Exercise 2 — Multiple Source Files

Directory: [`02-multiple-files/`](02-multiple-files/)

This exercise introduces programs split across multiple source files and the role of header files.

### Step 1: Compile manually

Navigate to the exercise directory:

```bash
cd ../02-multiple-files
```

Inspect these files:

- `main.c`
- `math_ops.c`
- `math_ops.h`

Determine which source files must be compiled and linked to produce the complete program.

Compile the program directly with GCC, without using Make:

```bash
gcc -Wall -Wextra main.c math_ops.c -o app
```

Run the executable:

```bash
./app
```

Confirm that the program produces the expected result.

Experiment with compiling only one source file at a time. Observe the errors and explain why compiling an individual file is not necessarily sufficient to produce the final executable.

### Step 2: Analyze the Makefile

Open `Makefile` and answer the following questions:

- What do the `CC` and `CFLAGS` variables represent?
- What does `$(CC)` mean in a recipe?
- What does `$(CFLAGS)` expand to?
- Why does the `all` target depend on `main.c`, `math_ops.c`, and `frumos`?
- What happens when the `frumos` target is built?
- Why might the executable be rebuilt even when only one source file has changed?
- What happens if `math_ops.h` changes but neither `.c` file is modified?
- What is the difference between a target that represents a file and a target used only to perform an action?

Run `make`, inspect the generated files, and run `make` a second time. Compare the output of both executions.

### Further investigation

Consider how the Makefile could be improved to:

- Express the actual dependencies of the executable.
- Track changes to header files.
- Avoid rebuilding the entire program when only one source file changes.
- Use `.PHONY` for targets that do not represent files.
- Separate compilation from linking using object files (`.o`).

First understand the behavior of the existing Makefile, then make changes and verify their effects.

### Topics to discuss

- Multiple translation units.
- Function declarations and definitions.
- Header files and include guards.
- Compilation versus linking.
- Make variables and prerequisites.
- Incremental builds.
- Object files and dependency tracking.

---

## Exercise 3 — Source and Header Directories

Directory: [`03-with-directories/`](03-with-directories/)

This exercise introduces a project layout where source files and header files are stored in separate directories. Unlike the previous exercises, you will need to discover the appropriate GCC command yourself and then express the build in a Makefile.

The project has the following structure:

```text
03-with-directories/
├── include/
│   └── geometry.h
└── src/
    ├── Makefile
    ├── geometry.c
    └── main.c
```

### Step 1: Explore and compile manually with GCC

Navigate to the `03-with-directories/` directory and inspect the project structure and all the source and header files.

Before using Make, determine how to compile and link the program directly with GCC. You will need to work out:

- Which source files must be included in the build.
- How to make the compiler find the header in the separate `include/` directory.
- Which output filename to choose.
- Which warning flags to use.

Do not look at or use a ready-made GCC command. Construct it yourself, run it, inspect any errors or warnings, and revise it until the executable builds and runs correctly.

### Step 2: Write and test the Makefile

Once you have a working manual GCC command, open the `Makefile` in the `src/` directory.

Write a Makefile that performs the same build. Work out the targets, prerequisites, recipe, compiler options, and output filename yourself.

Your Makefile should include:

- A default target that builds the executable.
- Appropriate prerequisites.
- A recipe that builds the program.
- A `clean` target.
- `.PHONY` declarations where appropriate.

Run `make` and verify that the executable behaves as it did when you built it manually.

### Step 3: Test incremental builds

Run `make` again without changing any files. Observe whether the build command runs a second time.

Next, modify `geometry.c` and run `make` again. Repeat the experiment after modifying `geometry.h`.

Ask yourself:

- Which files should trigger a rebuild?
- Does your Makefile express all dependencies required for a correct build?
- Does Make know which source files depend on a particular header?
- How could you improve the Makefile so that changes trigger only the necessary recompilation?

Finally, use your `clean` target and verify that the generated executable and any other build artifacts are removed.

**Important:** paths in a Makefile are generally interpreted relative to the directory from which `make` is run, not automatically relative to the Makefile's own location. Choose a working directory and write your paths consistently.

### Topics to discuss

- Project directory structure.
- Header search paths.
- Relative paths in Makefiles.
- Targets, prerequisites, and recipes.
- Header dependencies.
- Incremental builds.
- Testing a Makefile from a clean state.

## Key Takeaway

GCC compiles and links programs. Make automates the build process by tracking targets, prerequisites, and the commands needed to produce them.

A good Makefile is not simply a shorter way to write a GCC command. It describes the dependencies of a project so that builds are repeatable, efficient, and correct.
