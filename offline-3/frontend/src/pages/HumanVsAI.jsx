import React, { useState, useEffect } from 'react';
import GameBoard from '../components/GameBoard';
import axios from 'axios';
import { serverLink } from '../config';

export default function HumanVsAI() {
  const [board, setBoard] = useState([]);
  const [turn, setTurn] = useState('R');
  const [bestMove, setBestMove] = useState(null);

  useEffect(() => {
    const fetchBoard = async () => {
      try {
        const res = await axios.get(`${serverLink}/board`);
        setBoard(res.data.board);
        if (res.data.header === 'Human Move:') {
          setTurn('B');
        } else {
          setTurn('R');
        }
      } catch (err) {
        console.error('Error fetching board:', err);
      }
    };
    fetchBoard();
  }, []);

  const handleMove = async (row, col, setWinner) => {
    try {
      if ((turn === 'R' && board[row][col].endsWith('B')) || (turn === 'B' && board[row][col].endsWith('R'))) {
        alert('Invalid move');
        return {
          valid: false,
          t: turn
        };
      }

      if (turn === 'R') {
        await axios.post(`${serverLink}/human`, { row, col, player: 'R' });
        const res = await axios.get(`${serverLink}/board`);
        setBoard(res.data.board);
        setTurn('B');

        const check = await axios.get(`${serverLink}/board`);
        if (check.data.game_over) {
          setWinner(check.data.winner);
          await axios.get(`${serverLink}/reset`);
          return {
            valid: false,
            t: 'R'
          };
        }
        if (check.data.header === 'Human Move:') {
          const aiResponse = await axios.get(`${serverLink}/ai`);
          const newBoard = await axios.get(`${serverLink}/board`);
          setBoard(newBoard.data.board);
          setTurn('R');
          setBestMove(aiResponse.data.best_move);
          console.log('AI Move:', aiResponse.data.best_move[0] + 1, aiResponse.data.best_move[1] + 1);

          if (aiResponse.data.game_over) {
            setWinner(aiResponse.data.winner);
            await axios.get(`${serverLink}/reset`);
            return {
              valid: false,
              t: 'R'
            };
          }

        }
      }
      return {
        valid: true,
        t: turn
      };
    } catch (err) {
      setTurn('R');
      console.error(check.data.header);
      console.log('Error during move:', err);
      
      alert('Error during move.');
    }
  };

  return <GameBoard board={board} handleMove={handleMove} bestMove={bestMove} />;
}
