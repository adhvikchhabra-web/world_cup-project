# World Cup Path Predictor & Strategy Engine (C++)

A C++ based football tournament simulation and analytics engine that predicts tournament outcomes, evaluates team strengths, and generates strategic insights using Monte Carlo simulations, object-oriented programming, and data structures.

## Features

- Simulate complete World Cup tournaments
- Generate all possible tournament paths for a team
- Predict tournament outcomes using Monte Carlo Simulation (10,000+ runs)
- Head-to-head match simulation
- Team selection and Best XI generation
- Bracket probability analysis
- Team and squad database management

## Tech Stack

- C++
- Object-Oriented Programming (OOP)
- Data Structures & Algorithms
- Monte Carlo Simulation
- STL (Vectors, Maps, Algorithms)
- Makefile

## Project Structure

```text
players/
├── SquadDB.h
├── Player.h

simulation/
├── TournamentSimulator.h
├── MonteCarlo.h
├── MatchEngine.h

io/
├── Display.h

main.cpp
Makefile
```

## How to Build and Run

### Clone Repository

```bash
git clone https://github.com/adhvikchhabra-web/world_cup-project.git
cd world_cup-project
```

### Build

```bash
make
```

### Run

```bash
./worldcup
```

## Menu Options

1. Show all tournament paths for a team
2. Show Best XI for a team vs an opponent
3. Run Monte Carlo Simulation (10,000 tournaments)
4. Head-to-head match simulation
5. Show full bracket probabilities
6. Exit

## Skills Demonstrated

- Object-Oriented Design
- Simulation Modeling
- Probability Analysis
- Algorithm Design
- Data Structures
- C++ Development
- Software Architecture

## Author

Adhvik Chhabra
