import os
import sys

sys.setrecursionlimit(3000)

import copy
import random
from Dim import dimension
from moves import get_legal_moves, apply_move,out_of_bounds, gameover


# gameover, minimax

# --- Helper Functions ---


def evaluate(board, name = "orb_diff", blue= True):
    
    # print(name)
    if name == "orb_diff":
        return orb_diff(board,blue)
    if name == "cell_diff":
        return cell_diff(board, blue)
    if name == "critical_cells":
        redN = 0
        blueN = 0
        for i in range(dimension.ROWS):
            for j in range(dimension.COLS):
                if board[i][j].endswith('R'):
                    redN += criticalCells(board, (i, j))
                elif board[i][j].endswith('B'):
                    blueN += criticalCells(board, (i, j))
        if blue:
            return redN - blueN
        return blueN - redN
    if name == "adjacent_cells":
        redN = 0
        blueN = 0
        for i in range(dimension.ROWS):
            for j in range(dimension.COLS):
                if board[i][j].endswith('R'):
                    redN += adjCells(board, (i, j))
                elif board[i][j].endswith('B'):
                    blueN += adjCells(board, (i, j))
        if blue:
            return redN - blueN
        return blueN - redN
    if name == "own_support": 
        redN = 0
        blueN = 0
        for i in range(dimension.ROWS):
            for j in range(dimension.COLS):
                if board[i][j].endswith('R'):
                    redN += ownSupport(board, (i, j))
                elif board[i][j].endswith('B'):
                    blueN += ownSupport(board, (i, j))
        if blue:
            return redN - blueN
        return blueN - redN
    if name == "opponent_support":
        redN = 0
        blueN = 0
        for i in range(dimension.ROWS):
            for j in range(dimension.COLS):
                if board[i][j].endswith('R'):
                    redN += opponentSupport(board, (i, j))
                elif board[i][j].endswith('B'):
                    blueN += opponentSupport(board, (i, j))
        if blue:
            return redN - blueN  # should not choose where opponent is maximum
        return blueN - redN
    return 0


def orb_diff(board, blue= True):
    red_orbs = sum(int(cell[0]) for row in board for cell in row if cell.endswith('R'))
    blue_orbs = sum(int(cell[0]) for row in board for cell in row if cell.endswith('B'))
    # print(f"Red orbs: {red_orbs}, Blue orbs: {blue_orbs}")
    if blue:
        return blue_orbs - red_orbs
    return red_orbs - blue_orbs

def cell_diff(board , blue= True):
    red_cells = sum(1 for row in board for cell in row if cell.endswith('R'))
    blue_cells = sum(1 for row in board for cell in row if cell.endswith('B'))
    if blue :
        return blue_cells- red_cells
    return red_cells - blue_cells 


def critical_mass(board, position):
    i, j = position
    critical_mass = 4
    if i == dimension.ROWS - 1 or i == 0:
        critical_mass -= 1
    if j == dimension.COLS - 1 or j == 0:
        critical_mass -= 1
    return critical_mass

def adjCells(board, position):
    i, j = position
    count = 0
    for dx, dy in [(0, -1), (0, 1), (1, 0), (-1, 0)]:
        ni, nj = i + dx, j + dy
        if not out_of_bounds((ni, nj)):
            count += 1
    return count

def ownSupport ( board, position):
    i, j = position
    player = board[i][j][-1]  # Get the player from the cell
    if player not in ['R', 'B']:
        return 0  
    count = 0
    for dx, dy in [(0, -1), (0, 1), (1, 0), (-1, 0)]:
        ni, nj = i + dx, j + dy
        if not out_of_bounds((ni, nj)) and board[ni][nj].endswith(player):
            count += int(board[ni][nj][0])  
    return count

def opponentSupport(board, position):
    i, j = position
    player = board[i][j][-1]  # Get the player from the cell
    if player not in ['R', 'B']:
        return 0  
    count = 0
    for dx, dy in [(0, -1), (0, 1), (1, 0), (-1, 0)]:
        ni, nj = i + dx, j + dy
        if not out_of_bounds((ni, nj)) and board[ni][nj].endswith('R' if player == 'B' else 'B'):
            count += int(board[ni][nj][0])  
    return count

def criticalCells (board, position):
    i, j = position
    player = board[i][j][-1]  # Get the player from the cell
    if player not in ['R', 'B']:
        return 0  
    count = 0
    for dx, dy in [(0, -1), (0, 1), (1, 0), (-1, 0)]:
        ni, nj = i + dx, j + dy
        if not out_of_bounds((ni, nj)) and board[ni][nj].endswith(player) and (critical_mass(board, (ni, nj))-1) == int(board[ni][nj][0]):
            count += 1
    return count
