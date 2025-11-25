#!/usr/bin/env bash
set -euo pipefail

echo "=== PART_A_START ==="
echo "--- FORK_IDENTITY_START ---"
./fork_identity
echo "--- FORK_IDENTITY_END ---"

echo "--- PROC_TREE_START ---"
./proc_tree
echo "--- PROC_TREE_END ---"

echo "--- FORK_EXEC_START ---"
./fork_exec /bin/echo "Lab1_Test_Message"
echo "--- FORK_EXEC_END ---"

echo "=== PART_A_END ==="

echo "=== PART_B_START ==="

echo "--- MEMORY_PERF_START ---"
./memory_perf
echo "--- MEMORY_PERF_END ---"

echo "=== PART_B_END ==="
