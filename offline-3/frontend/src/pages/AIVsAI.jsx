import React, { useState, useEffect } from 'react';
import GameBoard from '../components/GameBoard';
import axios from 'axios';
import { Button, Box } from '@mui/material';
import { serverLink } from '../config';

export default function AIvsAI() {
  const [board, setBoard] = useState([]);
  const [bestMove, setBestMove] = useState(null);
  const [winner, setWinner] = useState(null);
  const [aiRunning, setAiRunning] = useState(false);

  useEffect(() => {
    const fetchInitialBoard = async () => {
      try {
        const res = await axios.get(`${serverLink}/board`);
        setBoard(res.data.board);
      } catch (err) {
        console.error('Error fetching board:', err);
      }
    };

    fetchInitialBoard();
  }, []);

  useEffect(() => {
    if (!aiRunning || winner) return;

    const sleep = (ms) => new Promise((res) => setTimeout(res, ms));

    const runAIvsAI = async () => {
      let currentTurn = 'R';
      let currentLink = `${serverLink}/ai2`;

      while (true) {
        try {
          console.log(`${currentLink} making move for ${currentTurn}`);

          const res = await axios.get(currentLink);
          const updatedBoard = await axios.get(`${serverLink}/board`);
          console.log(res.data);
          
          setBoard(updatedBoard.data.board);
          setBestMove(res.data.best_move);

          if (res.data.game_over) {
            setWinner(res.data.winner);
            console.log(`${currentTurn} wins! 🎉`);
            
            break;
          }

          // Alternate turn locally
          currentTurn = currentTurn === 'R' ? 'B' : 'R';
          currentLink = currentTurn === 'R'
            ? `${serverLink}/ai2`
            : `${serverLink}/ai`;

          await sleep(1000);
        } catch (err) {
          console.error('AI vs AI error:', err);
          break;
        }
      }
    };

    runAIvsAI();
  }, [aiRunning, winner]);

  return (
    <Box display="flex" flexDirection="column" alignItems="center" width="100vw" justifyContent="center" minHeight="100vh">
      <Button
        variant="contained"
        color="primary"
        onClick={() => setAiRunning(true)}
        disabled={aiRunning}
        sx={{ mb: 3 }}
      >
        Start AI vs AI
      </Button>

      <GameBoard board={board} handleMove={()=>{

      } } bestMove={bestMove} />
    </Box>
  );
}
