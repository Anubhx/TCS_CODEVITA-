from collections import defaultdict, deque

def find_connected_blocks(grid, N, M):
    """Find connected blocks in the matrix."""
    blocks = defaultdict(set)
    visited = set()

    def dfs(i, j, block_num):
        """Perform DFS to find all cells belonging to the same block."""
        if (
            (i, j) in visited or i < 0 or i >= N or j < 0 or j >= M or
            grid[i][j] != block_num
        ):
            return
        visited.add((i, j))
        blocks[block_num].add((i, j))

        for ni, nj in [(i + 1, j), (i - 1, j), (i, j + 1), (i, j - 1)]:
            dfs(ni, nj, block_num)

    for i in range(N):
        for j in range(M):
            if (i, j) not in visited:
                dfs(i, j, grid[i][j])
    return blocks

def apply_gravity(blocks, N, M):
    """Adjust blocks to account for gravity."""
    column_blocks = defaultdict(list)

    # Group cells by column
    for block_num, coords in blocks.items():
        for i, j in coords:
            column_blocks[j].append((i, block_num))

    # Sort cells in each column by row descending
    for j in column_blocks:
        column_blocks[j].sort(reverse=True)

    # Recalculate blocks after gravity
    new_blocks = defaultdict(set)
    for j in range(M):
        rows_in_column = []
        for i, block_num in column_blocks[j]:
            rows_in_column.append((i, block_num))
        rows_in_column.sort(reverse=True)

        # Place blocks at the bottom of the column
        new_row_index = N - 1
        for _, block_num in rows_in_column:
            new_blocks[block_num].add((new_row_index, j))
            new_row_index -= 1

    return new_blocks

def find_dependencies(blocks, N, M, target):
    """Find all blocks that need to be removed to reach the target block."""
    target_coords = blocks[target]
    target_columns = set(j for i, j in target_coords)

    # Find all blocks that overlap the target's columns
    blocks_to_remove = set()
    for block_num, coords in blocks.items():
        if block_num == target:
            continue

        block_columns = set(j for i, j in coords)
        if target_columns & block_columns:  # Check if columns overlap
            blocks_to_remove.add(block_num)

    return len(blocks_to_remove)

def solve_block_extraction():
    """Main function to solve the problem."""
    N, M = map(int, input().split())

    grid = []
    for _ in range(N):
        row = list(map(int, input().split()))
        grid.append(row)

    K = int(input())

    # Step 1: Find connected blocks
    blocks = find_connected_blocks(grid, N, M)

    # Step 2: Apply gravity to adjust blocks
    blocks = apply_gravity(blocks, N, M)

    # Step 3: Calculate dependencies to access the target block
    result = find_dependencies(blocks, N, M, K)
    print(result)

if __name__ == "__main__":
    solve_block_extraction()
