#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <tuple>
using namespace std;

// Directions for moving in the grid (up, down, left, right)
vector<pair<int, int>> moves = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Function to count the number of infected neighbors
int countInfectedNeighbors(const vector<vector<int>>& grid, int x, int y) {
    int n = grid.size();
    int infected = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && grid[nx][ny] == 1) {
                infected++;
            }
        }
    }
    return infected;
}

// Function to simulate the spread of the plague (1 step)
vector<vector<int>> simulatePlague(const vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> nextGrid = grid;

    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            int infected = countInfectedNeighbors(grid, x, y);

            if (grid[x][y] == 0 && infected == 3) {
                nextGrid[x][y] = 1;  // Cell becomes infected
            } else if (grid[x][y] == 1) {
                if (infected < 2 || infected > 3) {
                    nextGrid[x][y] = 0;  // Cell becomes uninfected
                }
            }
        }
    }
    return nextGrid;
}

// BFS to find the shortest time to destination
int solve(int n, vector<vector<char>>& initialGrid) {
    vector<vector<int>> grid(n, vector<int>(n));
    int startx = -1, starty = -1, destx = -1, desty = -1;

    // Convert input grid and locate 's' (start) and 'd' (destination)
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            grid[x][y] = (initialGrid[x][y] == '1') ? 1 : 0;
            if (initialGrid[x][y] == 's') {
                startx = x;
                starty = y;
                grid[x][y] = 0;  // Start cell is empty
            }
            if (initialGrid[x][y] == 'd') {
                destx = x;
                desty = y;
                grid[x][y] = 0;  // Destination cell is empty
            }
        }
    }

    queue<tuple<int, int, vector<vector<int>>, int>> q;
    set<tuple<int, int, vector<vector<int>>>> visited;

    q.push({startx, starty, grid, 0});

    while (!q.empty()) {
        auto [x, y, current_grid, days] = q.front();
        q.pop();

        // If we've reached the destination, return the number of days
        if (x == destx && y == desty) {
            return days;
        }

        auto state_key = make_tuple(x, y, current_grid);
        if (visited.count(state_key)) continue;
        visited.insert(state_key);

        // Simulate the next plague spread
        vector<vector<int>> next_grid = simulatePlague(current_grid);

        // Explore all possible moves (up, down, left, right)
        for (auto [dx, dy] : moves) {
            int nx = x + dx, ny = y + dy;

            // If the move is within bounds and the cell is not infected
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && next_grid[nx][ny] == 0) {
                q.push({nx, ny, next_grid, days + 1});
            }
        }
    }

    return -1;  // Return -1 if the destination is unreachable
}

int main() {
    int n;
    cin >> n;

    vector<vector<char>> grid(n, vector<char>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }

    cout << solve(n, grid) + 1;  // Add 1 to account for the initial day
    return 0;
}
