import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import { Container, Typography } from '@mui/material';
import HomePage from './pages/HomePage';
import HumanVsHuman from './pages/HumanVsHuman';
import HumanVsAI from './pages/HumanVsAI';
import AIVsAI from './pages/AIVsAI';
import RandomVsAI from './pages/RandomVsAI';

export default function App() {
  return (
    <Router>
      <Container>
        <Typography variant="h4" style={{ textAlign: 'center', marginBottom: '10px' , width : '100vw'}}  color="primary"  component="h1"  gutterBottom>
          Chain Reaction Game</Typography>
        <Routes>
          <Route path="/" element={<HomePage />} />
          <Route path="/human-vs-human" element={<HumanVsHuman />} />
          <Route path="/human-vs-ai" element={<HumanVsAI />} />
          <Route path="/ai-vs-ai" element={<AIVsAI />} />
          <Route path="/random-vs-ai" element={<RandomVsAI />} />
        </Routes>
      </Container>
    </Router>
  );
}
