from helper import gameover
from Dim import dimension
from helper import evaluate
from moves import get_legal_moves, apply_move 

def minimax(board, depth, maximizing, alpha, beta, player):
    over,w = gameover(board)
    if depth == 0 or over==True:
        # red player -> 1st player -> player Not None (called from /ai2)
        name = dimension.heuristicRed


        if player=='B' : # none means blue player -> (called from /ai)
            name = dimension.heuristicBlue
            # print("hahaha")
        # else :
        #     print("jajaja")
        blue = player=='B'
        # if not blue :
        # print(blue)
        return evaluate(board,name, blue), None
    # print(f"Minimax called: Depth: {depth}, Player: {player}")
    legal_moves = get_legal_moves(board, player)

    if maximizing:
        max_eval = float('-inf')
        best_move = None
        vals = []
        for move in legal_moves:
            # print(f"Current Move: {move}, Depth: {depth}")
            new_board,_ = apply_move(board, move, player)
            next_player = 'R' if player == 'B' else 'B'
            eval_score, _ = minimax(new_board, depth - 1, False, alpha, beta, next_player)
            vals.append(eval_score)
            # print(f"Evaluating move: {move}, Eval Score: {eval_score}, Depth: {depth}")
            if eval_score > max_eval:
                max_eval = eval_score
                best_move = move
            alpha = max(alpha, eval_score)
            if beta <= alpha:
                # print("Alpha-Beta Pruning")
                # print(f"Alpha: {alpha}, Beta: {beta}")
                # print(f"Max Eval: {max_eval}, Best Move: {best_move}")
                # print(f"Current Move: {move}, Depth: {depth}")
                break
        # print(vals)
        if depth ==3 :
            print(max_eval)
        return max_eval, best_move

    else:
        min_eval = float('inf')
        best_move = None
        for move in legal_moves:
            new_board ,_ = apply_move(board, move, player)
            next_player = 'R' if player == 'B' else 'B'
            eval_score, _ = minimax(new_board, depth - 1, True, alpha, beta, next_player)
            if eval_score < min_eval:
                min_eval = eval_score
                best_move = move
            beta = min(beta, eval_score)
            if beta <= alpha:
                break
        return min_eval, best_move
