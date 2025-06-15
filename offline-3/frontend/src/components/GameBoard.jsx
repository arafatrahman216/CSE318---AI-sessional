// components/GameBoard.jsx
import React, { useEffect, useState } from 'react';
import { Grid, Box, Typography, Button, Fade, Chip } from '@mui/material';

const getCellColor = (value) => {
  if (value.endsWith('R')) return '#e53935';
  if (value.endsWith('B')) return '#1e88e5';
  return '#f5f5f5';
};

const getOrbCount = (value) => (value === '0' ? 0 : parseInt(value[0]));

export default function GameBoard({ board, handleMove ,bestMove}) {
  const [turn, setTurn] = useState('R');
  const [winner, setWinner] = useState(null);

  const ROWS = board.length;
  const COLS = board[0]?.length || 6;

  useEffect(() => {
    const checkWinner = async () => {
      try {
        const response = await fetch('http://localhost:8000/board');
        const data = await response.json();
        if (data.game_over) {
          setWinner(data.winner);
          console.log(`${data.winner} wins! 🎉`)
          await fetch('http://localhost:8000/reset', { method: 'GET' });
        }
      } catch (error) {
        console.error('Error checking winner:', error);
      }
    };
    checkWinner();
  }, [board]);

  const handleCellClick = async (i, j) => {
    try {
      if (!winner) {
        const result = await handleMove(i, j, setWinner);
        if (result?.valid) {
          setTurn(result.t);
        }
      }
    } catch (error) {
      console.error('Error during cell click:', error);
    }
  };

  const handleReturnHome = () => {
    window.location.href = '/';
  };

  const renderOrbs = (count, color) => {
    const radius = 18;
    return (
     
      <Box position="relative" width="100vw" height="100%">
        {[...Array(count)].map((_, idx) => (
          <Box
            key={idx}
            sx={{
              position: 'absolute',
              width: 20,
              height: 20,
              borderRadius: '50%',
              backgroundColor: color,
              top: `${35 + radius * Math.sin((2 * Math.PI * idx) / count)}%`,
              left: `${35 + radius * Math.cos((2 * Math.PI * idx) / count)}%`,
              transform: 'translate(-50%, -50%)',
              boxShadow: `0 0 2ypx ${color}, 0 0 2px ${color}`,
              transition: 'all 0.2s ease-in-out',
            }}
          />
        ))}
      </Box>
    );
  };

  return (
    <div
      style={{
        backgroundColor: '#f0f4f8',
        minHeight: '100vh',
        width: '100vw',
        display: 'block',
        alignItems: 'center',
        justifyContent: 'center',
      }}
    >
      <Box
        display="flex"
        flexDirection="column"
        alignItems="center"
        justifyContent="center"
        minHeight="80vh"
        sx={{
          background: 'linear-gradient(135deg,rgb(227, 233, 242) 0%,rgb(217, 221, 229) 100%)',
          py: 6,
        }}
      >
        <Box display="flex" gap={2} mb={3}>
          <Button
            variant="contained"
            color="primary"
            size="large"
            sx={{
              borderRadius: 3,
              boxShadow: 2,
              textTransform: 'none',
              fontWeight: 600,
              letterSpacing: 1,
              px: 4,
              background: 'linear-gradient(90deg, #1e88e5 0%, #43a047 100%)',
            }}
            onClick={async () => {
              await fetch('http://localhost:8000/reset', { method: 'GET' });
              window.location.reload();
            }}
          >
            Reset Game
          </Button>
          <Button
            variant="outlined"
            color="secondary"
            size="large"
            sx={{
              borderRadius: 3,
              fontWeight: 600,
              letterSpacing: 1,
              px: 4,
              borderColor: '#1e88e5',
              color: '#1e88e5',
              '&:hover': {
                borderColor: '#43a047',
                color: '#43a047',
              },
            }}
            onClick={handleReturnHome}
          >
            Return to Home
          </Button>
        </Box>

        {bestMove && (
          <Box mt={1} mb={2}>
            <Chip
              label={`AI Best Move: Row ${bestMove[0] + 1}, Col ${bestMove[1] + 1}`}
              color="info"
              sx={{
                fontSize: '1.05rem',
                px: 2,
                py: 1,
                borderRadius: 2,
                background: 'linear-gradient(90deg, #29b6f6 0%, #81d4fa 100%)',
                color: '#01579b',
                fontWeight: 500,
                boxShadow: 1,
              }}
            />
          </Box>
        )}

        <Fade in>
          <Box mt={1} mb={2}>
            <Chip
              label={winner ? `${winner} wins! 🎉` : `Current Turn: ${turn === 'R' ? 'Red' : 'Blue'}`}
              color={winner ? 'success' : turn === 'R' ? 'error' : 'primary'}
              sx={{
                fontSize: '1.2rem',
                px: 3,
                py: 1.5,
                borderRadius: 2,
                fontWeight: 700,
                letterSpacing: 1,
                boxShadow: 2,
                background: winner
                  ? 'linear-gradient(90deg, #43a047 0%, #a5d6a7 100%)'
                  : turn === 'R'
                  ? 'linear-gradient(90deg, #e53935 0%, #ff8a65 100%)'
                  : 'linear-gradient(90deg, #1e88e5 0%, #90caf9 100%)',
                color: '#fff',
              }}
            />
          </Box>
        </Fade>

        <Box
          mt={2}
          display="flex"
          justifyContent="center"
          alignItems="center"
          minHeight="70vh"
          flexDirection="column"
          bgcolor="rgba(183, 185, 182, 0.81)"
          p={3}
          borderRadius="20px"
          boxShadow="0 8px 32px rgba(30,136,229,0.10)"
          sx={{
            backdropFilter: 'blur(6px)',
            border: '1.5px solid #e3eafc',
          }}
        >
          <Box
            display="grid"
            gridTemplateColumns={`repeat(${COLS}, 70px)`}
            gap={2}
          >
            {board.map((row, i) =>
              row.map((cell, j) => (
                <Box
                  key={`${i}-${j}`}
                  onClick={() => handleCellClick(i, j)}
                  sx={{
                    width: 70,
                    height: 70,
                    background: 'transparent',
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'center',
                    borderRadius: '18px',
                    boxShadow: '0 4px 16px rgba(30,136,229,0.08)',
                    cursor: winner ? 'not-allowed' : 'pointer',
                    transition: 'transform 0.18s cubic-bezier(.4,2,.6,1), box-shadow 0.18s',
                    '&:hover': {
                      transform: winner ? 'none' : 'scale(1.08)',
                      boxShadow: winner
                        ? '0 4px 16px rgba(30,136,229,0.08)'
                        : '0 8px 24px rgba(30,136,229,0.18)',
                    },
                    border: cell !== '0' ? '2.5px solid #fff' : '2.5px solid #e3eafc',
                    position: 'relative',
                  }}
                >
                  {renderOrbs(getOrbCount(cell), cell.endsWith('R') ? '#e53935' : cell.endsWith('B') ? '#1e88e5' : '#ccc')}
                </Box>
              ))
            )}
          </Box>
        </Box>
      </Box>
    </div>
  );
}
