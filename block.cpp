#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Helper function to perform DFS and find connected blocks
void dfs(int i, int j, int block_num, vector<vector<int>>& grid, unordered_set<pair<int, int>, hash<pair<int, int>>>& visited, unordered_map<int, unordered_set<pair<int, int>>>& blocks) {
    if (visited.count({i, j}) || i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size() || grid[i][j] != block_num)
        return;

    visited.insert({i, j});
    blocks[block_num].insert({i, j});  // Add the current block to its corresponding set

    // Visit all 4 neighbors (up, down, left, right)
    dfs(i + 1, j, block_num, grid, visited, blocks);
    dfs(i - 1, j, block_num, grid, visited, blocks);
    dfs(i, j + 1, block_num, grid, visited, blocks);
    dfs(i, j - 1, block_num, grid, visited, blocks);
}

// Function to find all connected blocks
unordered_map<int, unordered_set<pair<int, int>, hash<pair<int, int>>>> find_connected_blocks(vector<vector<int>>& grid) {
    unordered_map<int, unordered_set<pair<int, int>, hash<pair<int, int>>>> blocks;
    unordered_set<pair<int, int>, hash<pair<int, int>>> visited;

    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            if (visited.count({i, j}) == 0) {
                int block_num = grid[i][j];
                dfs(i, j, block_num, grid, visited, blocks);  // Run DFS for this block
            }
        }
    }

    return blocks;
}

// Function to find dependencies (blocks to remove to clear target)
int find_dependencies(unordered_map<int, unordered_set<pair<int, int>, hash<pair<int, int>>>>& blocks, vector<vector<int>>& grid, int target) {
    unordered_map<int, int> block_tops;
    // Find the top-most row for each block
    for (const auto& block : blocks) {
        int block_num = block.first;
        int min_row = grid.size();
        for (const auto& coord : block.second) {
            min_row = min(min_row, coord.first);
        }
        block_tops[block_num] = min_row;
    }

    int target_top = block_tops[target];
    unordered_set<int> blocks_to_remove;

    for (const auto& block : blocks) {
        int block_num = block.first;
        if (block_num == target) continue;  // Skip the target block itself

        int block_top = block_tops[block_num];

        // Blocks above or at the same level as the target may block it
        if (block_top <= target_top) {
            unordered_set<int> target_columns;
            unordered_set<int> block_columns;

            // Collect columns for target block
            for (const auto& coord : blocks[target]) {
                target_columns.insert(coord.second);
            }

            // Collect columns for current block
            for (const auto& coord : block.second) {
                block_columns.insert(coord.second);
            }

            // Check if there's any overlap in columns
            if (!target_columns.empty() && !block_columns.empty()) {
                for (const auto& col : target_columns) {
                    if (block_columns.count(col)) {
                        blocks_to_remove.insert(block_num);
                        break;
                    }
                }
            }
        }
    }

    return blocks_to_remove.size();  // Return number of blocks to be removed
}

void solve_block_extraction() {
    int N, M;
    cin >> N >> M;  // Read dimensions of the grid

    vector<vector<int>> grid(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> grid[i][j];  // Read the grid
        }
    }

    int K;
    cin >> K;  // Read the target block number

    auto blocks = find_connected_blocks(grid);  // Get the connected blocks
    int result = find_dependencies(blocks, grid, K);  // Find blocks to remove to access target block
    cout << result << endl;  // Output the result
}

int main() {
    solve_block_extraction();  // Call the solve function when the script is run
    return 0;
}
