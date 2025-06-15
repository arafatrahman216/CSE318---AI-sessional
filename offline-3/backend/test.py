from helper import GAMESTATE_FILE, ROWS, COLS, read_board, write_board, get_legal_moves, apply_move, evaluate, initialize_board
import os
import copy


board= initialize_board()
board,b=apply_move(board, (1, 1), 'R')
print(b)
write_board("Human Move",board)

board,b=apply_move(board, (1, 1), 'R')
print(b)
write_board("Human Move",board)

board,b=apply_move(board, (1, 1), 'R')
print(b)
write_board("Human Move",board)

board,b=apply_move(board, (1, 1), 'R')
print(b)
write_board("Human Move",board)

board,b=apply_move(board, (0, 0), 'B')
print(b)
write_board("Human Move",board)
board,b=apply_move(board, (0, 1), 'R')
print(b)
write_board("AI Move",board)

board,b=apply_move(board, (0, 1), 'R')
print(b)
write_board("AI Move",board)

board,b=apply_move(board, (1, 0), 'R')
print(b)
write_board("AI Move",board)

board,b=apply_move(board, (0, 0), 'R')
print(b)
write_board("AI Move",board)


board,b=apply_move(board, (1, 1), 'R')
print(b)
write_board("AI Move",board)

board,b=apply_move(board, (1, 0), 'R')
print(b)
write_board("AI Move",board)


board,b=apply_move(board, (1, 0), 'R')
print(b)
write_board("AI Move",board)

board,b=apply_move(board, (1, 0), 'R')
print(b)
write_board("AI Move",board)

board,b=apply_move(board, (2, 1), 'R')
print(b)
write_board("AI Move",board)
board,b=apply_move(board, (1, 1), 'R')
print(b)
write_board("AI Move",board)

board,b=apply_move(board, (0, 2), 'R')
print(b)
write_board("AI Move",board)
