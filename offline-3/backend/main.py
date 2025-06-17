from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from Dim import dimension

from helper import criticalCells, adjCells, ownSupport, opponentSupport,critical_mass, orb_diff, cell_diff 
from moves import get_legal_moves, apply_move, out_of_bounds, explode_cell, gameover, random_move
from files import read_board, write_board, initialize_board
from minimax import minimax

serverLink = "http://localhost:8000"



access= True

app = FastAPI()

origins = [
    "http://localhost:8000",  # React dev server
    "http://127.0.0.1:8000"
]



app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # ⚠️ temporary, for testing only
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


class Move(BaseModel):
    row: int
    col: int


class UserInput(BaseModel):
    row : int
    col : int
    player : str | None = None



# --- Routes ---
@app.get("/board")
def get_board():
    try:
        header, board = read_board()
        over, winner = gameover(board)
        return {"header": header, "board": board, "game_over": over, "winner": winner}
    except Exception as e:
        print(e)
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/human")
def human_move(userinput : UserInput ):
    header,board = read_board()
    # print("jojojo")    
    # if board[move.row][move.col] == '0' or board[move.row][move.col].endswith('R'):
    board, blast=apply_move(board,(userinput.row, userinput.col), userinput.player)
    move = "Human Move:"
    if userinput.player == 'B':
        move = "Ai Move:"
    write_board(move, board)
    over, winner = gameover(board)
    print(orb_diff(board, userinput.player == 'B'))
    print(winner)
    return {"status": "Human move recorded", "game_over": over, "winner": winner}
    # else:
    #     raise HTTPException(status_code=400, detail="Invalid move")

## AI move-> 2nd move (Blue player)
@app.get("/ai") 
def ai_move():
    _, board = read_board()
    #  here player is None , so heuristic1 is used  -> Blue player
    __, best_move = minimax(board, depth=3, maximizing=True, alpha=float('-inf'), beta=float('inf'), player='B')
    print("best move is ", best_move)
    if best_move:
        board,_ = apply_move(board, best_move, 'B')
    else:
        raise HTTPException(status_code=400, detail="No valid moves available for AI")
    print(__)
    write_board("AI Move:", board)
    print("AI Move:")
    print(orb_diff(board, True))
    # print(board)
    over, winner = gameover(board)
    print("winner:", winner)
    return {"status": "AI move made", "best_move": best_move, "game_over": over, "winner": winner}


#first move (Red AI player)
@app.get("/ai2")
def ai_move2():
    _, board = read_board()
    # here Player is not None, so heuristic 2 is used -> Red player
    __, best_move = minimax(board, depth=3, maximizing=True, alpha=float('-inf'), beta=float('inf'), player='R')
    
    if best_move:
        board,_ = apply_move(board, best_move, 'R')
    else:
        raise HTTPException(status_code=400, detail="No valid moves available for AI")
    print(__)
    write_board("Human Move:", board)
    print("human Move:")
    # print(board)
    over, winner = gameover(board)
    print("winner:", winner)
    return {"status": "AI2 move made", "best_move": best_move, "game_over": over, "winner": winner}


@app.post("/setHeuristic")
def setHeuristic(userinput : UserInput):
    numb = userinput.row    # heuristic number
    name = userinput.player # name of heuristic
    if (numb==1) :
        dimension.heuristicBlue = name
    elif (numb==2) :
        dimension.heuristicRed = name
    print(dimension.heuristicBlue)
    print(dimension.heuristicRed)
    return { "status" : "success"}


@app.get("/")
def test_endpoint():
    print(dimension.heuristicRed)
    print(dimension.heuristicBlue)
    return {"status": "Test endpoint is working"}

@app.post("/setDim")
def demo_move(userinput : UserInput):
    dimension.ROWS= userinput.row
    dimension.COLS = userinput.col
    board = initialize_board()
    write_board("Start", board)
    print(dimension.ROWS)
    print(dimension.COLS)
    return {"status": "Dimensions set", "rows": dimension.ROWS, "cols": dimension.COLS}

@app.get("/getDim")
def get_dimensions():
    return {"rows": dimension.ROWS, "cols": dimension.COLS}


@app.get("/reset") 
def resetBoard():
    board = initialize_board()
    write_board("Start",board)
    return board

# random move agent(Red AI) -> 1st move
@app.get("/randomMove") 
def randomMoveAgent():
    _,board = read_board()
    new_board, move = random_move(board,'R')
    if move:
        write_board("Human Move:", new_board)
    else:
        raise HTTPException(status_code=400, detail="No valid moves available for Random Agent")
    print("Random Move:")
    over, winner = gameover(new_board)
    print("winner:", winner)
    return {"status": "Random move made", "move": move, "game_over": over, "winner": winner}