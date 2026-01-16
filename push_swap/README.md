*This project has been created as part of the 42 curriculum by lmanuel-.*

# push_swap

## Description

`push_swap` is a 42 project whose goal is to sort a list of integers using **two stacks** (`a` and `b`) and a very limited set of allowed operations:

- `sa`, `sb`, `ss`: swap the top elements
- `pa`, `pb`: push between stacks
- `ra`, `rb`, `rr`: rotate (top element goes to bottom)
- `rra`, `rrb`, `rrr`: reverse rotate (bottom element goes to top)

The challenges are:

- **Always produce a correct sorting sequence**
- **Minimize the number of operations**, especially on:
  - up to 3 numbers
  - up to 5 numbers
  - 100 numbers
  - 500 numbers

This implementation focuses on:

- A **parser** that accepts integers via multiple arguments and/or quoted strings.
- **Error checking** for invalid input, duplicates, and integer overflows.
- A **data structure design** using a doubly-linked list for stacks.
- A sorting strategy based on:
  - Optimized small-case sorts (`<= 3` and `<= 5` elements).
  - **Index compression** + **binary radix sort** on the compressed indices for larger inputs.

---

## Instructions

### Requirements

- Linux environment
- `make`
- `cc` (or another C compiler compatible with `cc`)
- The provided `libft` inside the `libft/` directory

### Compilation

From the project root (the directory containing the `Makefile`):

```bash
make
```

This will:

- Build `libft` as `libft/libft.a`
- Compile all `src/*.c`
- Produce the `push_swap` executable

Useful Makefile targets:

```bash
make        # build push_swap
make clean  # remove object files
make fclean # remove object files and push_swap binary (and clean libft)
make re     # fclean + full rebuild
```

> The Makefile is written so that repeated `make` calls do **not** relink if nothing has changed.

### Usage

Basic usage:

```bash
./push_swap <list of integers>
```

Examples:

- Separate arguments:
```bash
./push_swap 2 1 3
```

- Mixed with quotes
```bash
./push_swap "3 2 1"
```
```bash
./push_swap 4 "3 2" 1
```

The program outputs a sequence of operations to **stdout**, one per line, for example:

```text
sa
pb
ra
...
```

If the input is already sorted or contains less than two numbers, it prints **nothing**.

On **error**, the program prints:

```text
Error
```

to **stderr** and exits with a non-zero status.

#### Using `checker_linux` for tests

Assuming `checker_linux` is in the same directory as `push_swap`:

```bash
ARG="3 2 1" && ./push_swap $ARG | ./checker_linux $ARG
```

You should see:

```text
OK
```

Another example with random numbers:

```bash
ARG=$(seq 1 100 | sort -R | tr '\n' ' ') && ./push_swap $ARG | ./checker_linux $ARG
```

#### Counting operations

To see both the operations and how many there are:

```bash
ARG=$(seq 1 100 | sort -R | tr '\n' ' ') && ./push_swap $ARG | tee /dev/tty | wc -l
```

---

## Implementation Overview

### Data Structures

Stacks are stored as a **doubly-linked list** with a `t_stack` wrapper:

```c
typedef struct s_node
{
    int             value;
    int             index;
    struct s_node   *next;
    struct s_node   *prev;
}   t_node;

typedef struct s_stack
{
    int     size;
    t_node  *top;
    t_node  *bot;
}   t_stack;

typedef struct s_stacks
{
    t_stack a;
    t_stack b;
}   t_stacks;
```

- `value`: original integer value.
- `index`: compressed index (`0..n-1` after compression).
- `top` / `bot`: pointers to the top and bottom nodes.
- `size`: number of elements in the stack.

### Parsing & Validation

Input is parsed as follows:

- Each `argv[i]` is either a single number, or a **string containing multiple numbers separated by spaces** (handled via `ft_split` from `libft`).
- Each token is passed through a custom `ft_atoi_ps` which:
  - Skips leading whitespace.
  - Handles optional `+` / `-`.
  - Rejects invalid formats (`""`, `"+"`, `"-"`, `"1a"`, `"1  2x"`, etc.).
  - Detects **overflow** (values outside `INT_MIN..INT_MAX`).
- All valid values are pushed into stack `a` from bottom to top.
- After parsing, `check_dups` scans for duplicate values and triggers an error if any are found.

On **any** parsing or allocation error, `error_exit`:

- Frees both stacks.
- Prints `"Error"` to **stderr**.
- Exits with an appropriate error code.

### Sorting Strategy

1. **Early exit**  
   After parsing:
   - If `a.size == 0`: nothing to sort.
   - If `check_sort(&a)` is true: already sorted, no operations printed.

2. **Index compression**

   Before sorting, the code compresses values to indices:

   - Copy stack `a` values into an array.
   - Sort the array (using insertion sort).
   - For each node in stack `a`, set `node->index` to the position of its `value` in the sorted array.
   - Result: indices are in the range `[0, n - 1]`.

   This makes radix sort simpler and independent of the original integer range. Could also be used with other sorting algorithms.

3. **Small cases**

   - If `a.size <= 3` - `sort_three(&stacks)`:
     - Uses hard-coded logic on the 3 indices to sort in minimal moves.
   - If `a.size <= 5` - `sort_five(&stacks)`:
     - Repeatedly:
       - Move the smallest element to the top of `a` (with minimal `ra` / `rra`),
       - `pb` to stack `b`.
     - Sort the remaining 3 elements with `sort_three`.
     - `pa` everything back from `b` to `a`.

4. **Larger cases: Binary radix sort on indices**

   For inputs larger than 5, a **binary radix sort** is applied to the `index` field:

   - Compute the number of bits needed:
     ```c
     max_idx = a.size - 1;
     bit_count = 0;
     while ((max_idx >> bit_count) != 0)
         bit_count++;
     ```
   - For each bit position `cur_bit` from `0` to `bit_count - 1`:
     - For each element in `a` (fixed size per pass):
       - Look at `idx = a.top->index;`
       - If the `cur_bit`-th bit of `idx` is `0` then `pb` (push to `b`).
       - Else `ra` (rotate in `a`).
     - After scanning all elements, push everything back from `b` to `a` with `pa` in a loop.
   - After all bits are processed, stack `a` is sorted in ascending order of `index` (and thus of `value`).

This approach gives:

- Deterministic behavior,
- Good time complexity around **O(n log n)**,
- Reasonable operation counts for 100 and 500 elements for evaluation.

---

## Project Structure

At the root of the project:

- `Makefile`: builds the project and `libft`.
- `push_swap`: final executable (after `make`).
- `include/push_swap.h`: main header file.
- `libft/`: custom library used by the project.
- `src/`: source files, grouped roughly as:

- **Core:**
  - `push_swap.c`: `main` with sorting selection.
  - `stack_utils.c`: `stack_init`, `stack_clear`, etc.
  - `node_utils.c`: node creation and push/pop helpers.
  - `error_utils.c`: `error_exit`.

- **Parsing & validation:**
  - `parse_utils.c`: `ft_atoi_ps`, `ft_split_free`, internal helpers (`ft_isspace`, `spaces_sign`).
  - `parse_args.c`: argument parsing and filling stack `a`.
  - `validation_utils.c`: `check_dups`, `check_sort`.

- **Indexing & sorting:**
  - `index_utils.c`: index compression (stack to array, sort, assign indices).
  - `sort_small.c`: `sort_three`, `sort_five`.
  - `sort_radix.c`: `sort_radix` and bit-count helper.

- **Operations:**
  - `swap_ops.c`: `sa`, `sb`, `ss`.
  - `push_ops.c`: `pa`, `pb`.
  - `rot_ops.c`: `ra`, `rb`, `rr`.
  - `rrot_ops.c`: `rra`, `rrb`, `rrr`.

---

## Resources

### C Language & General Concepts

- **The C Programming Language – Kernighan & Ritchie (K&R)**  
  Classic reference book for C; useful for understanding pointers, memory behavior, and idiomatic C.

- **42 Norm / Norminette (official repo)**  
  <https://github.com/42School/norminette>  
  Official repository for the norminette tool and Norm PDF.

### Data Structures & Algorithms

- **GeeksforGeeks – Data Structures (Linked Lists, Stacks, Queues)**  
  <https://www.geeksforgeeks.org/data-structures/>  
  Conceptual explanations and examples of basic data structures similar to those used in this project.

- **Insertion Sort**  
  <https://www.geeksforgeeks.org/insertion-sort/>  
  Used as a simple, clear algorithm for sorting the temporary array in index compression.

- **Radix Sort (idea and variants)**  
  <https://www.geeksforgeeks.org/radix-sort/>  
  Describes radix sorting by digits/bits; this project uses a binary, index-based variant.

- **Big-O Notation & Complexity**  
  - Big-O Cheat Sheet: <https://www.bigocheatsheet.com/>  
  - Asymptotic Analysis overview: <https://www.geeksforgeeks.org/analysis-of-algorithms-set-4-analysis-of-loops/>  
  These were used to understand and reason about complexity (time and space) of the different sorting approaches.

### push_swap-Specific (Conceptual Help)

*(Used for understanding typical strategies and pitfalls, not for copying code.)*

- **push_swap strategy explanations and common patterns**  
  - “push_swap tutorial” (Ayogun):  
    <https://medium.com/@ayogun/push-swap-tutorial-fa746e6aba1e>

These materials helped to:

- Understand the idea of index compression,
- Compare different approaches (small sorts, radix, chunk strategies),
- Clarify expectations for move counts on 100 and 500 elements.

### Additional Tools (For Testing)
- **Random number generator**
  - Calculator.net: <https://www.calculator.net/random-number-generator.html>
  Website with tool to generate random numbers.

### AI Usage

AI (ChatGPT - GPT-5.1) was used as a **learning and design assistant** during the development of this project.  
In particular, AI was used for:

- Clarifying theoretical concepts:
  - Time & space complexity (Big O / Ω / Θ).
  - Differences between insertion sort, bubble sort, and radix sort.
  - How binary representation relates to radix sorting by bits.
- Helping design and structure the project.
- Reviewing and refining.

All final code was:

- Written and adapted by me,
- Reviewed and tested with `checker_linux` and random input,
- Verified for Norm compliance and project rules.

AI was used as a tool for explanations, brainstorming, and feedback — not as a source to copy full implementations from.
