import React, { useEffect, useState } from 'react';
import { Box, Typography, Button, Stack, TextField, Fade, Paper, FormControl, InputLabel, Select, MenuItem } from '@mui/material';
import { useNavigate } from 'react-router-dom';
import { motion } from 'framer-motion';
import axios from 'axios';
import { serverLink } from '../config';

export default function HomePage() {
  const navigate = useNavigate();
  const [rows, setRows] = useState(3);
  const [cols, setCols] = useState(3);
  const [heuristic1, setHeuristic1] = useState('orb_diff');
  const [heuristic2, setHeuristic2] = useState('cell_diff');

  useEffect(() => {
    const fetchDimensions = async () => {
      try {
        const response = await axios.get(`${serverLink}/getDim`);
        setRows(response.data.rows);
        setCols(response.data.cols);
      } catch (error) {
        console.error('Error fetching dimensions:', error);
      }
    };
    fetchDimensions();
  }, []);

  const handleStart = async (path) => {
    await axios.post(`${serverLink}/setDim`, {
      row: rows,
      col: cols,
      heuristic1,
      heuristic2,
      player: null
    });
    navigate(path);
  };

  const handleSetHeuristic = async (number, name) => {
  try {
    await axios.post(`${serverLink}/setHeuristic`, {
      row: number, // 1-> blue, 2-> red
      col : 1,
      player: name
    });
    console.log(`Heuristic for AI ${number} set to ${name}`);
  } catch (error) {
    console.error(`Failed to set heuristic for AI ${number}:`, error);
  }
};

  

  return (
    <Fade in timeout={800}>
      <Box
        component={motion.div}
        initial={{ opacity: 0, y: 50 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: 0.8 }}
        display="flex"
        flexDirection="column"
        alignItems="center"
        justifyContent="center"
        minHeight="90vh"
        width="100vw"
        bgcolor="linear-gradient(to right, #e0f7fa, #ffe0f0)"
      >
        <Paper elevation={6} sx={{ p: 5, borderRadius: '20px', background: 'linear-gradient(135deg, #ffffff 0%, #f3e5f5 100%)', width: '90%', maxWidth: 600 }}>
          <Typography variant="h3" gutterBottom align="center" sx={{ fontWeight: 'bold', color: '#6a1b9a' }}>
            Welcome to Chain Reaction
          </Typography>

          <Stack direction="row" spacing={2} justifyContent="center" mb={3}>
            <TextField
              label="Rows"
              type="number"
              value={rows}
              onChange={(e) => setRows(Number(e.target.value))}
              inputProps={{ min: 3, max: 15 }}
              fullWidth
            />
            <TextField
              label="Columns"
              type="number"
              value={cols}
              onChange={(e) => setCols(Number(e.target.value))}
              inputProps={{ min: 3, max: 15 }}
              fullWidth
            />
          </Stack>

          <Stack direction="column" spacing={2} mb={3}>
            <FormControl fullWidth>
              <InputLabel>Primary AI 1 Heuristic(human vs AI or Blue player)</InputLabel>
              <Select
                value={heuristic1}
                label="AI 1 Heuristic"
                onChange={(e) => setHeuristic1(e.target.value)}
              >
                <MenuItem value={'orb_diff'}>Orb Difference</MenuItem>
                <MenuItem value={'cell_diff'}>Cell Difference</MenuItem>
                <MenuItem value={'critical_cells'}>Critical Cells</MenuItem>
                <MenuItem value={'adjacent_cells'}>Adjacent Cells</MenuItem>
                <MenuItem value={'own_support'}>Own Support</MenuItem>
                <MenuItem value={'opponent_support'}>Opponent Support</MenuItem>
                <MenuItem value={'test'}>Zero</MenuItem>
                
                
              </Select>
            </FormControl>
            <FormControl fullWidth>
              <InputLabel>Secondary AI 2 Heuristic( Red player)</InputLabel>
              <Select
                value={heuristic2}
                label="AI 2 Heuristic"
                onChange={(e) => setHeuristic2(e.target.value)}
              >
                <MenuItem value={'orb_diff'}>Orb Difference</MenuItem>
                <MenuItem value={'cell_diff'}>Cell Difference</MenuItem>
                <MenuItem value={'critical_cells'}>Critical Cells</MenuItem>
                <MenuItem value={'adjacent_cells'}>Adjacent Cells</MenuItem>
                <MenuItem value={'own_support'}>Own Support</MenuItem>
                <MenuItem value={'opponent_support'}>Opponent Support</MenuItem>
                <MenuItem value={'test'}>Zero</MenuItem>
              </Select>
            </FormControl>
            <Button
              variant="outlined"
              color="primary"
              onClick={() => {
                handleSetHeuristic(1, heuristic1);
                handleSetHeuristic(2, heuristic2);
              }}
            >
              Set Heuristics
            </Button>

          </Stack>

          <Stack spacing={2} direction="column" alignItems="center">
            <Button variant="contained" size="large" fullWidth onClick={() => handleStart('/human-vs-human')}>
              Human vs Human
            </Button>
            <Button variant="contained" size="large" fullWidth color="secondary" onClick={() => handleStart('/human-vs-ai')}>
              Human vs AI
            </Button>
            <Button variant="contained" size="large" fullWidth color="error" onClick={() => handleStart('/ai-vs-ai')}>
              AI vs AI
            </Button>
            <Button variant="contained" size="large" fullWidth color="warning" onClick={() => handleStart('/random-vs-ai')}>
              Random vs AI
            </Button>
          </Stack>
        </Paper>
      </Box>
    </Fade>
  );
}
