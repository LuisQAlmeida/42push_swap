#!/usr/bin/env bash

set -uo pipefail

ROOT_DIR="$(
    cd "$(dirname "${BASH_SOURCE[0]}")/.." >/dev/null 2>&1
    pwd
)"

PUSH_SWAP="$ROOT_DIR/push_swap"
CHECKER="$ROOT_DIR/tools/checker_linux"
CC_BIN="${CC:-cc}"

PASS_COUNT=0
FAIL_COUNT=0
SKIP_COUNT=0

TMP_DIR="$(mktemp -d)"

cleanup()
{
    rm -rf "$TMP_DIR"
    make -C "$ROOT_DIR" fclean >/dev/null 2>&1 || true
}

trap cleanup EXIT

pass()
{
    printf 'PASS: %s\n' "$1"
    PASS_COUNT=$((PASS_COUNT + 1))
}

fail()
{
    printf 'FAIL: %s\n' "$1"
    FAIL_COUNT=$((FAIL_COUNT + 1))
}

skip()
{
    printf 'SKIP: %s\n' "$1"
    SKIP_COUNT=$((SKIP_COUNT + 1))
}

build_project()
{
    printf '\n=== BUILD (%s) ===\n' "$CC_BIN"

    if ! make -C "$ROOT_DIR" fclean >/dev/null; then
        fail "make fclean"
        return 1
    fi

    if ! make -C "$ROOT_DIR" CC="$CC_BIN" >/dev/null; then
        fail "build with $CC_BIN"
        return 1
    fi

    if [ ! -x "$PUSH_SWAP" ]; then
        fail "push_swap executable exists"
        return 1
    fi

    pass "build with $CC_BIN"
    return 0
}

run_exact()
{
    local name="$1"
    local expected_status="$2"
    local expected_stdout="$3"
    local expected_stderr="$4"
    shift 4

    local stdout_file="$TMP_DIR/exact.stdout"
    local stderr_file="$TMP_DIR/exact.stderr"
    local expected_stdout_file="$TMP_DIR/expected.stdout"
    local expected_stderr_file="$TMP_DIR/expected.stderr"
    local actual_status

    : > "$stdout_file"
    : > "$stderr_file"

    printf '%s' "$expected_stdout" >"$expected_stdout_file"
    printf '%s' "$expected_stderr" >"$expected_stderr_file"

    "$PUSH_SWAP" "$@" >"$stdout_file" 2>"$stderr_file"
    actual_status=$?

    if [ "$actual_status" -ne "$expected_status" ]; then
        fail "$name: exit status $actual_status, expected $expected_status"
        return
    fi

    if ! cmp -s "$stdout_file" "$expected_stdout_file"; then
        fail "$name: unexpected stdout"
        printf '  expected bytes:\n'
        od -An -tx1 -c "$expected_stdout_file"
        printf '  actual bytes:\n'
        od -An -tx1 -c "$stdout_file"
        return
    fi

    if ! cmp -s "$stderr_file" "$expected_stderr_file"; then
        fail "$name: unexpected stderr"
        printf '  expected bytes:\n'
        od -An -tx1 -c "$expected_stderr_file"
        printf '  actual bytes:\n'
        od -An -tx1 -c "$stderr_file"
        return
    fi

    pass "$name"
}

validate_operations()
{
    local operations_file="$1"
    shift

    python3 - "$operations_file" "$@" <<'PY'
import sys

operations_path = sys.argv[1]
stack_a = [int(value) for value in sys.argv[2:]]
original = list(stack_a)
stack_b = []

valid_operations = {
    "sa", "sb", "ss",
    "pa", "pb",
    "ra", "rb", "rr",
    "rra", "rrb", "rrr",
}


def swap(stack):
    if len(stack) >= 2:
        stack[0], stack[1] = stack[1], stack[0]


def push(source, destination):
    if source:
        destination.insert(0, source.pop(0))


def rotate(stack):
    if len(stack) >= 2:
        stack.append(stack.pop(0))


def reverse_rotate(stack):
    if len(stack) >= 2:
        stack.insert(0, stack.pop())


with open(operations_path, encoding="utf-8") as stream:
    operations = stream.read().splitlines()

for operation in operations:
    if operation not in valid_operations:
        print(f"invalid operation: {operation!r}", file=sys.stderr)
        raise SystemExit(1)

    if operation == "sa":
        swap(stack_a)
    elif operation == "sb":
        swap(stack_b)
    elif operation == "ss":
        swap(stack_a)
        swap(stack_b)
    elif operation == "pa":
        push(stack_b, stack_a)
    elif operation == "pb":
        push(stack_a, stack_b)
    elif operation == "ra":
        rotate(stack_a)
    elif operation == "rb":
        rotate(stack_b)
    elif operation == "rr":
        rotate(stack_a)
        rotate(stack_b)
    elif operation == "rra":
        reverse_rotate(stack_a)
    elif operation == "rrb":
        reverse_rotate(stack_b)
    elif operation == "rrr":
        reverse_rotate(stack_a)
        reverse_rotate(stack_b)

if stack_b:
    print(f"stack B is not empty: {stack_b}", file=sys.stderr)
    raise SystemExit(1)

if stack_a != sorted(original):
    print(f"stack A is not sorted: {stack_a}", file=sys.stderr)
    raise SystemExit(1)

if len(stack_a) != len(original):
    print("element count changed", file=sys.stderr)
    raise SystemExit(1)

raise SystemExit(0)
PY
}

run_sort_case()
{
    local name="$1"
    shift

    local stdout_file="$TMP_DIR/sort.stdout"
    local stderr_file="$TMP_DIR/sort.stderr"
    local checker_stdout="$TMP_DIR/checker.stdout"
    local checker_stderr="$TMP_DIR/checker.stderr"
    local program_status
    local checker_status

    : > "$stdout_file"
    : > "$stderr_file"

    "$PUSH_SWAP" "$@" >"$stdout_file" 2>"$stderr_file"
    program_status=$?

    if [ "$program_status" -ne 0 ]; then
        fail "$name: push_swap exited with $program_status"
        return
    fi

    if [ -s "$stderr_file" ]; then
        fail "$name: unexpected stderr"
        sed 's/^/  | /' "$stderr_file"
        return
    fi

    if ! validate_operations "$stdout_file" "$@"; then
        fail "$name: internal operation replay"
        return
    fi

    if [ -x "$CHECKER" ]; then
        : > "$checker_stdout"
        : > "$checker_stderr"

        "$CHECKER" "$@" \
            <"$stdout_file" \
            >"$checker_stdout" \
            2>"$checker_stderr"

        checker_status=$?

        if [ "$checker_status" -ne 0 ]; then
            fail "$name: supplied checker exited with $checker_status"
            return
        fi

        if [ "$(cat "$checker_stdout")" != "OK" ]; then
            fail "$name: supplied checker did not report OK"
            printf '  checker stdout: %q\n' "$(cat "$checker_stdout")"
            return
        fi

        if [ -s "$checker_stderr" ]; then
            fail "$name: supplied checker produced stderr"
            return
        fi
    else
        skip "$name: supplied checker unavailable"
    fi

    pass "$name"
}

printf '============================================================\n'
printf 'push_swap regression suite\n'
printf '============================================================\n'

printf '\nCompiler: %s\n' "$CC_BIN"

if ! build_project; then
    printf '\nBuild failed; regression tests cannot continue.\n'
    exit 1
fi

printf '\n=== BASIC / ACCEPTED INPUT ===\n'

run_exact \
    "no arguments" \
    0 \
    "" \
    ""

run_exact \
    "one value" \
    0 \
    "" \
    "" \
    42

run_exact \
    "already sorted" \
    0 \
    "" \
    "" \
    1 2 3

run_exact \
    "empty argument" \
    0 \
    "" \
    "" \
    ""

run_exact \
    "whitespace-only argument" \
    0 \
    "" \
    "" \
    "   "

run_exact \
    "INT_MIN" \
    0 \
    "" \
    "" \
    -2147483648

run_exact \
    "INT_MAX" \
    0 \
    "" \
    "" \
    2147483647

printf '\n=== DETERMINISTIC SMALL CASES ===\n'

run_exact \
    "two values reversed" \
    0 \
    $'sa\n' \
    "" \
    2 1

run_exact \
    "three values reversed" \
    0 \
    $'sa\nrra\n' \
    "" \
    3 2 1

run_exact \
    "single quoted argument" \
    0 \
    $'sa\nrra\n' \
    "" \
    "3 2 1"

run_exact \
    "leading plus sign" \
    0 \
    $'sa\n' \
    "" \
    +1 0

printf '\n=== INVALID INPUT ===\n'

run_exact \
    "duplicate values" \
    3 \
    "" \
    $'Error\n' \
    1 2 1

run_exact \
    "alphabetic token" \
    1 \
    "" \
    $'Error\n' \
    1 abc 2

run_exact \
    "mixed numeric token" \
    1 \
    "" \
    $'Error\n' \
    1 2a 3

run_exact \
    "double plus sign" \
    1 \
    "" \
    $'Error\n' \
    ++1

run_exact \
    "sign without digits" \
    1 \
    "" \
    $'Error\n' \
    -

run_exact \
    "below INT_MIN" \
    2 \
    "" \
    $'Error\n' \
    -2147483649

run_exact \
    "above INT_MAX" \
    2 \
    "" \
    $'Error\n' \
    2147483648

printf '\n=== OPERATION REPLAY / SORTING ===\n'

run_sort_case \
    "two-element sort" \
    2 1

run_sort_case \
    "three-element sort" \
    3 2 1

run_sort_case \
    "five-element sort" \
    5 1 4 2 3

run_sort_case \
    "radix path: six elements" \
    6 1 5 2 4 3

run_sort_case \
    "mixed signed values" \
    42 -7 0 13 -5 100 2 1

run_sort_case \
    "integer boundaries in one stack" \
    2147483647 -2147483648 0 1 -1

mapfile -t descending_100 < <(seq 100 -1 1)

run_sort_case \
    "100-element deterministic radix case" \
    "${descending_100[@]}"

printf '\n============================================================\n'
printf 'SUMMARY\n'
printf '============================================================\n'
printf 'Passed:  %d\n' "$PASS_COUNT"
printf 'Failed:  %d\n' "$FAIL_COUNT"
printf 'Skipped: %d\n' "$SKIP_COUNT"

if [ "$FAIL_COUNT" -ne 0 ]; then
    printf '\nFAIL: regression suite detected %d failure(s)\n' "$FAIL_COUNT"
    exit 1
fi

printf '\nPASS: all regression checks succeeded\n'
exit 0
