# World Cup Path Predictor & Strategy Engine (C++)

## Features
1. **Tournament Path Simulator** — DFS over all possible bracket outcomes
2. **Best XI Generator** — picks optimal 11 players vs each specific opponent using club form, international form, and head-to-head history
3. **Monte Carlo Engine** — runs 10,000 tournaments for accurate probability estimates
4. **Head-to-Head Simulator** — any two teams, with tactical breakdown
5. **Bracket Viewer** — round of 16 win probabilities

## Teams Included
Portugal, France, Brazil, Argentina, Germany, Spain, England, Morocco

## Build & Run
```bash
make
./worldcup
```

## DSA Concepts Used
- DFS (tournament tree traversal)
- Probability propagation through decision tree
- Greedy selection (best XI via scoring function)
- Monte Carlo simulation
- Unordered_map for O(1) team/player lookup
- Sorting for path ranking

## File Structure
```
worldcup/
├── main.cpp                      ← CLI & menu
├── players/
│   ├── Player.h                  ← Player struct with scoring
│   ├── Team.h                    ← Team struct
│   └── SquadDB.h                 ← All 8 squads hardcoded
├── simulation/
│   ├── MatchEngine.h             ← Win probability + Best XI builder
│   ├── TournamentSimulator.h     ← DFS over full bracket
│   └── MonteCarlo.h              ← Stochastic 10k-run engine
└── io/
    └── Display.h                 ← Coloured terminal output
```
