import React, { useState } from 'react';
import { useEffect } from 'react';
import GameBoard from '../components/GameBoard';
import axios from 'axios';
import { serverLink } from '../config';

var ROWS = 9;
var COLS = 6;
var initialBoard = Array.from({ length: ROWS }, () => Array(COLS).fill('0'));

export default function HumanVsHuman() {
  const [board, setBoard] = useState([]);
  const [turn, setTurn] = useState('R');

  useEffect(() => {
    const fetchBoard = async () => {
      try {
        const res = await axios.get(`${serverLink}/board`);
        setBoard(res.data.board);
        initialBoard= res.data.board;
        ROWS = res.data.row;
        COLS = res.data.col;
        if (res.data.header=="Human Move:") {
          setTurn('B');
        }
        else {
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
              t : turn
            };
        }

      const response= await axios.post(`${serverLink}/human`, { row, col, player: turn });
      const res = await axios.get(`${serverLink}/board`);
      setBoard(res.data.board);
      setTurn(turn === 'R' ? 'B' : 'R');
      if (response.data.game_over) {
        setWinner(response.data.winner);
        // alert(`${turn} wins! 🎉`);
        await axios.get(`${serverLink}/reset`);
      }

      return {
        valid: true,
        t: turn === 'R' ? 'B' : 'R'
      }
    } catch (err) {
      alert('Invalid move or server error');
    }
  };

  return <GameBoard board={board} handleMove={handleMove} />;
}
