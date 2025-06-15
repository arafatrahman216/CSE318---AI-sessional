from Dim import dimension
import copy
from files import read_board, write_board
import random

# check_blast , apply_move, out_of_bounds , get_legal_moves, explode_cell


def gameover(board):
    winner = None
    # for row in board:
        # print(row)
    # print("game over check")
    # print(board)
    has_red = any(cell.endswith('R') for row in board for cell in row if cell != '0')
    has_blue = any(cell.endswith('B') for row in board for cell in row if cell != '0')
    over= not (has_red and has_blue)

    if over:
        red_count = sum(int(cell[0]) for row in board for cell in row if cell.endswith('R'))
        blue_count = sum(int(cell[0]) for row in board for cell in row if cell.endswith('B'))
        if red_count > blue_count and red_count > 1:
            # print("Red wins!")
            winner = "R"
            
        elif blue_count > red_count and blue_count > 1:
            # print("Blue wins!")
            winner = "B"
        elif red_count == blue_count and red_count > 1:
            print("It's a draw!")
            over = False
        else:
            over = False

    return over, winner


def check_blast( position,count) :
    critical_mass=4
    i,j= position
    if (i==dimension.ROWS-1 or i==0):
        critical_mass-=1
    if (j==dimension.COLS-1 or j==0):
        critical_mass-=1
    if count>= critical_mass :
        return True
    return False

def out_of_bounds(position):
    i,j = position
    if i < 0 or i >= dimension.ROWS or j < 0 or j >= dimension.COLS:
        return True
    return False


def explode_cell(board, position, player):
    i,j = position
    nb= copy.deepcopy(board)
    lmoves= [(0,-1),(0,1),(1,0),(-1,0)]
    for dx,dy in lmoves :
        m1= i+dx
        m2= j+dy
        # print(f"{m1} and {m2}")
        if out_of_bounds((m1,m2)):
            continue
        nb, b = apply_move(nb, (m1,m2), player)
        over , winner = gameover(nb)
        if over:
            # print("Game over detected in explode_cell")
            # write_board("Game Over", nb)
            return nb

        # if b :
        #     print("blast in explode")

    return nb 




def get_legal_moves(board, player):
    return [(i, j) for i in range(dimension.ROWS) for j in range(dimension.COLS)
            if board[i][j] == '0' or board[i][j].endswith(player)]

def apply_move(board, move, player):
    new_board = copy.deepcopy(board)
    i, j = move
    blast= False
 
    if new_board[i][j] == '0':
        new_board[i][j] = f"1{player}"
    else:
        count = int(new_board[i][j][0]) + 1
        new_board[i][j] = f"{count}{player}"
        blast=check_blast(move,count)
        if blast :
            new_board[i][j]= '0'
            new_board=explode_cell(new_board, move, player)
    return new_board, blast

def random_move(board, player):
    legal_moves = get_legal_moves(board, player)
    if not legal_moves:
        return None
    move = random.choice(legal_moves)
    board, _ = apply_move(board, move, player)
    return board, move

