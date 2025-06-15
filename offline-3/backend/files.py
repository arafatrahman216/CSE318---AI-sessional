import os
from Dim import dimension
import copy

GAMESTATE_FILE = "gamestate.txt"


# initialize_board, gameover, minimax

# --- Helper Functions ---
def initialize_board():
    return [['0' for _ in range(dimension.COLS)] for _ in range(dimension.ROWS)]



def read_board():
    if not os.path.exists(GAMESTATE_FILE):
        return "Game not started", initialize_board()
    
    with open(GAMESTATE_FILE, 'r') as f:
        lines = f.read().strip().split('\n')
    
    header = lines[0].strip()

    board = []
    for line in lines[1:]:
        row = line.strip().split()
        # Ensure all items are strings and the row has expected number of columns
        if len(row) == dimension.COLS:
            board.append([str(cell) for cell in row])
        else:
            print(f"row {row} rowsss {dimension.ROWS}")
            raise ValueError("Invalid board format: incorrect number of columns.")
    
    if len(board) != dimension.ROWS:
        print(f"row {row} rowsss {dimension.ROWS}")

        raise ValueError("Invalid board format: incorrect number of dimension.ROWS.")
    
    return header, board


def write_board(header: str, board):
    with open(GAMESTATE_FILE, 'w') as f:
        f.write(header + '\n')
        for row in board:
            f.write(' '.join(str(cell) for cell in row) + '\n')

def reset_game():
    board = initialize_board()
    write_board("Start",board)
    return board
