from collections import defaultdict, set

def find_connected_blocks(grid, N, M):
    blocks = defaultdict(set)
    visited = set()
    
    def dfs(i, j, block_num):
        # DFS to visit all blocks connected to (i, j)
        if (i, j) in visited or i < 0 or i >= N or j < 0 or j >= M or grid[i][j] != block_num:
            return
        visited.add((i, j))
        blocks[block_num].add((i, j))  # Add the current block to its corresponding set
        # Visit all 4 neighbors (up, down, left, right)
        for ni, nj in [(i+1, j), (i-1, j), (i, j+1), (i, j-1)]:
            dfs(ni, nj, block_num)
    
    # Traverse the grid and start DFS when an unvisited block is found
    for i in range(N):
        for j in range(M):
            if (i, j) not in visited:
                block_num = grid[i][j]  # Current block number
                dfs(i, j, block_num)  # Run DFS for this block
    
    return blocks

def find_dependencies(blocks, N, M, target):
    """
    Find all blocks that need to be removed to reach target block.
    Returns the count of blocks to remove.
    """
    block_tops = {}
    for block_num, coords in blocks.items():
        min_row = min(i for i, j in coords)  # Find the minimum row (top-most row) for each block
        block_tops[block_num] = min_row
    
    target_coords = blocks[target]
    target_top = block_tops[target]  # Get the top-most row of the target block
    blocks_to_remove = set()  # Set to store blocks that need removal
    
    # Check other blocks to see if they need removal
    for block_num, coords in blocks.items():
        if block_num == target:  # Skip the target block itself
            continue
        
        block_top = block_tops[block_num]  # Get the top-most row of this block
        
        # Blocks that are above or at the same level as the target block may block the target
        if block_top <= target_top:
            target_columns = set(j for i, j in target_coords)  # Set of columns in target block
            block_columns = set(j for i, j in coords)  # Set of columns in current block
            
            if target_columns & block_columns:  # If there is any column overlap
                blocks_to_remove.add(block_num)  # Add the block to removal set
    
    return len(blocks_to_remove)  # Return how many blocks need removal

def solve_block_extraction():
    N, M = map(int, input().split())  # Read dimensions of the grid
    
    grid = []
    for _ in range(N):
        row = list(map(int, input().split()))  # Read each row of the grid
        grid.append(row)
    
    K = int(input())  # Read the target block number
    
    blocks = find_connected_blocks(grid, N, M)  # Get the connected blocks
    
    result = find_dependencies(blocks, N, M, K)  # Find blocks to remove to access target block
    print(result)  # Output the result

# Corrected main check
if __name__ == "__main__":  # Corrected check for the main module
    solve_block_extraction()  # Call the solve function when the script is run
