from collections import deque

# Function to count infected neighbors
def count_infected_neighbors(board, x, y):
    size = len(board)
    infected_count = 0
    # Check 8 neighbors
    for dx in [-1, 0, 1]:
        for dy in [-1, 0, 1]:
            if dx == 0 and dy == 0:  # Skip the current city
                continue
            nx, ny = x + dx, y + dy
            if 0 <= nx < size and 0 <= ny < size and board[nx][ny] == 1:
                infected_count += 1
    return infected_count

# Function to simulate the plague spread for one day
def plague_simulation(board):
    size = len(board)
    new_board = [row[:] for row in board]  # Copy of the board

    for x in range(size):
        for y in range(size):
            infected_count = count_infected_neighbors(board, x, y)
            # If the city is uninfected and has exactly 3 infected neighbors, it becomes infected
            if board[x][y] == 0 and infected_count == 3:
                new_board[x][y] = 1
            # If the city is infected and has less than 2 or more than 3 infected neighbors, it becomes uninfected
            elif board[x][y] == 1 and (infected_count < 2 or infected_count > 3):
                new_board[x][y] = 0
    return new_board

# BFS to find the minimum days required to reach the herb
def find_path(grid_size, initial_board):
    board = [[1 if cell == '1' else 0 for cell in row] for row in initial_board]
    start_x, start_y, end_x, end_y = -1, -1, -1, -1

    # Find the start ('s') and destination ('d') positions
    for x in range(grid_size):
        for y in range(grid_size):
            if initial_board[x][y] == 's':
                start_x, start_y = x, y
                board[x][y] = 0  # Mark start as uninfected
            elif initial_board[x][y] == 'd':
                end_x, end_y = x, y
                board[x][y] = 0  # Mark destination as uninfected

    # BFS initialization
    queue = deque([(start_x, start_y, board, 0)])  # (x, y, current_board, days)
    visited_states = set()

    while queue:
        x, y, current_board, days = queue.popleft()

        # Check if we've reached the destination
        if x == end_x and y == end_y:
            return days

        state = (x, y, tuple(map(tuple, current_board)))  # Represent current state as a tuple for immutability
        if state in visited_states:
            continue

        visited_states.add(state)

        # Simulate the next day's plague spread
        next_board = plague_simulation(current_board)

        # Check neighboring cells for possible moves (up, down, left, right)
        for dx, dy in [(0, 0), (-1, 0), (1, 0), (0, -1), (0, 1)]:
            nx, ny = x + dx, y + dy
            if 0 <= nx < grid_size and 0 <= ny < grid_size and next_board[nx][ny] == 0:
                queue.append((nx, ny, next_board, days + 1))

    return -1  # If destination is not reachable (which is guaranteed to be reachable)

# Main function to solve the problem
if __name__ == "__main__":
    n = int(input())  # Grid size
    grid = [input().strip() for _ in range(n)]  # Read grid
    print(find_path(n, grid) + 1)  # Output the minimum days required
