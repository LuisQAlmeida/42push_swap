# push_swap

[![CI](https://github.com/LuisQAlmeida/42push_swap/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/LuisQAlmeida/42push_swap/actions/workflows/ci.yml)

> Part of my [42 Common Core portfolio](https://github.com/LuisQAlmeida/42Portfolio).

A C sorting project that generates a valid sequence of constrained stack
operations using small-input routines, index compression, and binary radix sort.

## Overview

`push_swap` sorts a sequence of unique signed integers using two stacks,
`a` and `b`, and a deliberately restricted operation set.

The program does not print the sorted values themselves. Instead, it writes the
sequence of stack operations required to transform the original input into a
state where stack `a` is sorted in ascending order and stack `b` is empty.

The maintained implementation focuses on:

- validated integer parsing across separate and quoted arguments;
- explicit error handling and allocation cleanup;
- doubly linked stack structures;
- dedicated sorting logic for inputs of up to five values;
- index compression for arbitrary signed integer values;
- binary radix sorting for larger inputs;
- deterministic regression testing and continuous integration.

The original completed-project state is preserved by the annotated
`portfolio-baseline-2026-09` tag.

## Allowed operations

| Operation | Behaviour |
| --- | --- |
| `sa` | Swap the first two elements of stack `a` |
| `sb` | Swap the first two elements of stack `b` |
| `ss` | Apply the swap operation to both stacks |
| `pa` | Push the top element of `b` onto `a` |
| `pb` | Push the top element of `a` onto `b` |
| `ra` | Rotate stack `a` upward |
| `rb` | Rotate stack `b` upward |
| `rr` | Apply the rotate operation to both stacks |
| `rra` | Reverse rotate stack `a` |
| `rrb` | Reverse rotate stack `b` |
| `rrr` | Apply the reverse-rotate operation to both stacks |

Operations that cannot act on the required stack state safely become no-ops or,
for the maintained combined helpers, delegate to the valid single-stack
operation.

## Repository structure

```text
42push_swap/
├── .github/
│   └── workflows/
│       └── ci.yml
├── external/
│   └── libft/              # pinned 42Libft Git submodule
├── include/
│   └── push_swap.h
├── src/
│   ├── error_utils.c
│   ├── index_utils.c
│   ├── node_utils.c
│   ├── parse_args.c
│   ├── parse_utils.c
│   ├── push_ops.c
│   ├── push_swap.c
│   ├── rot_ops.c
│   ├── rrot_ops.c
│   ├── sort_radix.c
│   ├── sort_small.c
│   ├── stack_utils.c
│   ├── swap_ops.c
│   └── validation_utils.c
├── tests/
│   └── regression.sh
├── tools/
│   └── checker_linux
├── Doxyfile
├── Makefile
├── LICENSE
└── README.md
```

## Dependency management

The maintained portfolio version uses the canonical
[`42Libft`](https://github.com/LuisQAlmeida/42Libft) repository as a Git
submodule under:

```text
external/libft
```

The dependency is pinned to the `42Libft v1.0.0` release commit:

```text
0227823923ca15b580a481c3fb929d7f1382f545
```

Clone the repository together with the pinned dependency:

```bash
git clone --recurse-submodules git@github.com:LuisQAlmeida/42push_swap.git
cd 42push_swap
```

If the repository has already been cloned without submodules:

```bash
git submodule update --init --recursive
```

The academic dependency layout that existed before portfolio modernization is
preserved by `portfolio-baseline-2026-09`.

## Build

### Requirements

- Linux or another compatible Unix-like environment;
- `make`;
- a C compiler compatible with the project flags;
- Git for cloning the Libft submodule.

Build with the default compiler:

```bash
make
```

The maintained Makefile:

- compiles the project with `-Wall -Wextra -Werror`;
- builds the pinned Libft dependency;
- tracks the project and Libft public headers as object prerequisites;
- avoids unnecessary relinking when the build inputs have not changed.

Useful targets:

```bash
make
make clean
make fclean
make re
```

A different compatible compiler can be selected through `CC`, for example:

```bash
make CC=clang
```

## Usage

Pass integers as separate arguments:

```bash
./push_swap 4 2 5 1 3
```

Quoted groups are also supported:

```bash
./push_swap "4 2 5" 1 3
```

For unsorted valid input, the program prints one operation per line:

```text
pb
ra
pa
```

Already sorted input and a call without arguments produce no output.

Invalid input writes:

```text
Error
```

to standard error and exits with a non-zero status.

Rejected input includes:

- non-numeric tokens;
- duplicate values;
- values outside the signed `int` range;
- sign-only tokens;
- empty arguments;
- whitespace-only arguments.

`INT_MIN` and `INT_MAX` are accepted.

## Implementation

### Data model

Each stack is represented by a doubly linked list.

A node stores:

- the original integer value;
- a compressed sorting index;
- links to the previous and next nodes.

`t_stack` stores the current size and direct pointers to both the top and bottom
nodes. `t_stacks` groups stacks `a` and `b`.

### Parsing and error handling

Each command-line argument is split on space characters through Libft's
`ft_split()`.

`ft_atoi_ps()` validates and converts each token without terminating the process
itself. It reports a `t_error` result to the caller, which keeps ownership of
temporary parser allocations and can release them before calling
`error_exit()`.

Numeric overflow is detected before the intermediate arithmetic can exceed the
supported integer range.

After all values are parsed, the stack is checked for duplicates.

Fatal errors clear both stack structures, write `Error\n` to standard error and
exit using the corresponding error code.

### Index compression

The sorting algorithms operate on compressed indices rather than directly on
the original integer values.

The implementation:

1. copies stack values into a temporary array;
2. sorts that array using insertion sort;
3. finds the sorted position of each stack value;
4. stores that position as the node's index.

For `n` values, insertion sorting is `O(n²)` in the worst case. Assigning
indices currently performs linear searches for each node and is also `O(n²)`.

The index-compression stage therefore has overall `O(n²)` time complexity.

### Small inputs

Inputs of up to three values are handled by `sort_three()` using direct
comparisons of compressed indices.

For four or five values, `sort_five()` repeatedly moves the smallest index to
stack `b`, sorts the remaining values with `sort_three()`, then restores the
removed values with `pa`.

### Binary radix phase

Inputs larger than five values use a least-significant-bit-first binary radix
sort over the compressed indices.

For each required bit position:

1. inspect the index at the top of stack `a`;
2. push values whose current bit is `0` to stack `b`;
3. rotate values whose current bit is `1` within stack `a`;
4. restore all values from `b` to `a`;
5. continue with the next bit.

With indices in the range `0..n-1`, the radix phase processes `O(log n)` bit
positions and `O(n)` elements per position, giving that phase `O(n log n)`
time complexity.

Because the current index-compression stage is `O(n²)`, the complete maintained
implementation is overall `O(n²)`.

## v1.0.0 algorithm scope

The first maintained portfolio release intentionally preserves the binary radix
strategy selected for the original project.

During the academic project, this algorithm was chosen as a pragmatic trade-off
between correctness, implementation complexity, available development time, and
the need to continue progressing through the remaining 42 Common Core projects.

The objective was therefore to deliver a reliable and understandable sorting
implementation within the available project time rather than to maximize the
operation-count score.

This `v1.0.0` release preserves that engineering decision instead of
retroactively replacing the original algorithm during portfolio maintenance.

Future project evolution is intended to explore:

- alternative `push_swap` sorting strategies;
- operation-count optimization;
- comparative benchmarks on representative input sizes;
- algorithmic and implementation trade-offs;
- before-and-after comparison against the `v1.0.0` radix baseline.

## Testing

The repository includes a deterministic regression suite:

```bash
./tests/regression.sh
```

The current suite contains **30 checks** covering:

- accepted argument forms;
- integer boundaries;
- invalid syntax;
- empty and whitespace-only arguments;
- duplicate detection;
- integer overflow, including very large numeric input;
- exact stdout, stderr, and exit-status behaviour;
- emitted operation syntax;
- deterministic small-input sorting;
- the radix path;
- mixed signed values;
- independent replay of generated operations;
- verification that stack `a` becomes sorted and stack `b` becomes empty;
- supplementary validation through `tools/checker_linux` when available.

Run the same suite with Clang:

```bash
CC=clang ./tests/regression.sh
```

The repository-owned operation replay is the primary sorting oracle. The
provided Linux checker is supplementary validation.

GitHub Actions runs the suite with both `cc` and `clang`, initializes the Libft
submodule recursively, verifies the exact Libft revision, and checks that
validation leaves no generated build artefacts or repository modifications.

## Manual checker usage

After building the project:

```bash
ARG="3 2 1"
./push_swap $ARG | ./tools/checker_linux $ARG
```

A correct sequence produces:

```text
OK
```

To inspect the emitted operation count:

```bash
ARG=$(seq 1 100 | sort -R | tr '\n' ' ')
./push_swap $ARG | wc -l
```

## Doxygen reference

The maintained public and cross-module interface is documented in
`include/push_swap.h`.

Generate the HTML reference with:

```bash
doxygen Doxyfile
```

The generated documentation is written under:

```text
docs/html/
```

Generated Doxygen output is intentionally not versioned.

## Historical preservation

The annotated tag:

```text
portfolio-baseline-2026-09
```

preserves the repository state before professional portfolio modernization.

The maintained `main` branch adds testing, CI, dependency management,
documentation, build-system maintenance, and parser robustness while retaining
the project's original sorting strategy for the first portfolio release.

## Resources

Useful references for the concepts exercised by the project include:

- *The C Programming Language*, Kernighan and Ritchie;
- [42 Norminette](https://github.com/42School/norminette);
- [Insertion Sort](https://www.geeksforgeeks.org/insertion-sort/);
- [Radix Sort](https://www.geeksforgeeks.org/radix-sort/);
- [Data Structures](https://www.geeksforgeeks.org/data-structures/).

These resources were used for language, data-structure, algorithm, and
evaluation concepts rather than as sources of project implementations.

## AI usage

The original academic implementation and its core project decisions were
developed by me as part of the 42 Common Core.

AI-assisted tools were used as supporting tools during learning and later
portfolio maintenance, rather than as a substitute for understanding or project
ownership.

During the project and subsequent review, AI support was used for areas such as:

- explaining C, data-structure, and algorithm concepts;
- discussing alternative sorting approaches and complexity;
- reviewing error handling, memory ownership, and edge cases;
- reasoning about build and dependency behaviour;
- designing regression and validation scenarios;
- reviewing technical documentation.

During portfolio modernization, AI was also used to help structure a more
professional and repeatable repository workflow, including:

- repository audits;
- issue and pull-request scoping;
- regression and CI planning;
- documentation organization;
- Doxygen structure;
- dependency and build-system review;
- consistency across the wider 42 portfolio.

Changes introduced during portfolio maintenance were reviewed against the
actual implementation and validated through local builds, regression testing,
continuous integration, targeted memory checks, and repository-state audits.

This organization is intentionally being developed as a reusable foundation for
future software projects, while keeping the underlying implementation,
engineering decisions, and project understanding under my ownership.

The `portfolio-baseline-2026-09` tag preserves the completed-project state from
before this later portfolio-maintenance work.

## License

See [LICENSE](LICENSE).
