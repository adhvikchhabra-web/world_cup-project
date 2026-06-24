#pragma once
#include "../simulation/TournamentSimulator.h"
#include "../simulation/MonteCarlo.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>

// ── Terminal colour helpers ───────────────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"

class Display {
public:
    static void banner() {
        std::cout << BOLD << CYAN;
        std::cout << R"(
╔══════════════════════════════════════════════════════════╗
║     ⚽  WORLD CUP PATH PREDICTOR & STRATEGY ENGINE  ⚽   ║
║                   Powered by C++ DSA                     ║
╚══════════════════════════════════════════════════════════╝
)" << RESET << "\n";
    }

    static void divider(const std::string& title = "") {
        if (title.empty()) {
            std::cout << CYAN << "──────────────────────────────────────────────────────────\n" << RESET;
        } else {
            int dashes = (int)(50 - title.size());
            if (dashes < 2) dashes = 2;
            std::string line(dashes, '-');
            std::cout << CYAN << "-- " << BOLD << title << RESET << CYAN
                      << " " << line << "\n" << RESET;
        }
    }

    // Print best XI for team vs opponent
    static void printBestXI(Team& team, const std::string& opponent,
                             std::unordered_map<std::string, Team>& teams) {
        if (teams.find(opponent) == teams.end()) {
            std::cout << RED << "  [Unknown opponent]\n" << RESET;
            return;
        }
        auto xi = MatchEngine::buildBestXI(team, opponent, teams[opponent]);

        divider("Best XI: " + team.name + " vs " + opponent);
        std::cout << YELLOW << BOLD << "  Formation: " << xi.formation << "\n" << RESET;
        std::cout << GREEN  << "  Tactical note: " << xi.tacticalNote << "\n\n" << RESET;

        // Group by position for display
        std::vector<std::string> posOrder = {"GK","RB","CB","LB","CDM","CM","CAM","RW","LW","ST"};
        std::unordered_map<std::string, std::vector<Player*>> grouped;
        for (auto* p : xi.lineup) grouped[posToStr(p->position)].push_back(p);

        for (auto& pos : posOrder) {
            if (grouped.find(pos) == grouped.end()) continue;
            for (auto* p : grouped[pos]) {
                double sc = p->scoreVs(opponent);
                std::cout << "  " << BOLD << std::setw(5) << std::left << pos << RESET
                          << "  " << std::setw(25) << std::left << p->name
                          << "  [" << p->club << "]"
                          << "  Score: " << YELLOW << std::fixed << std::setprecision(2)
                          << sc << RESET << "\n";
            }
        }
        std::cout << "\n";
    }

    // Print best and worst paths for target team
    static void printPaths(std::vector<TournamentPath>& paths,
                           const std::string& team) {
        if (paths.empty()) {
            std::cout << RED << "  No paths found for " << team << "\n" << RESET;
            return;
        }

        // Sort by probability descending
        std::sort(paths.begin(), paths.end(),
            [](const TournamentPath& a, const TournamentPath& b) {
                return a.totalProb > b.totalProb;
            });

        divider("Tournament Paths: " + team);
        std::cout << BOLD << "  Total possible paths found: " << paths.size() << "\n\n" << RESET;

        auto printPath = [&](const TournamentPath& p, const std::string& label, const std::string& color) {
            std::cout << color << BOLD << "  " << label << "\n" << RESET;
            std::cout << "  ";
            std::cout << BOLD << team << RESET;
            for (int i = 0; i < (int)p.opponents.size(); i++) {
                std::cout << " → " << GREEN << p.opponents[i] << RESET
                          << " (" << std::fixed << std::setprecision(1)
                          << p.winProbs[i]*100 << "%)";
            }
            std::cout << "\n  " << "Overall win probability: "
                      << YELLOW << BOLD
                      << std::fixed << std::setprecision(3)
                      << p.totalProb * 100 << "%" << RESET << "\n\n";
        };

        printPath(paths.front(), "★ BEST PATH (Easiest route):", GREEN);
        if (paths.size() > 1)
            printPath(paths.back(), "✗ WORST PATH (Hardest route):", RED);

        // Print top 3
        std::cout << BOLD << "  Top 3 most likely paths:\n" << RESET;
        int shown = std::min((int)paths.size(), 3);
        for (int i = 0; i < shown; i++) {
            std::cout << "  " << (i+1) << ". ";
            auto& p = paths[i];
            for (int j = 0; j < (int)p.opponents.size(); j++) {
                if (j) std::cout << " → ";
                std::cout << p.opponents[j]
                          << "(" << std::fixed << std::setprecision(1)
                          << p.winProbs[j]*100 << "%)";
            }
            std::cout << "  [" << YELLOW << std::fixed << std::setprecision(3)
                      << p.totalProb*100 << "%" << RESET << "]\n";
        }
        std::cout << "\n";
    }

    // Print Monte Carlo results table
    static void printMonteCarloTable(
            std::unordered_map<std::string, MonteCarloStats>& stats,
            const std::string& highlightTeam = "") {

        divider("Monte Carlo Results (10,000 simulations)");

        // Sort by win%
        std::vector<std::pair<std::string, MonteCarloStats*>> sorted;
        for (auto& kv : stats) sorted.push_back({kv.first, &kv.second});
        std::sort(sorted.begin(), sorted.end(),
            [](auto& a, auto& b) { return a.second->winPct() > b.second->winPct(); });

        std::cout << BOLD
                  << std::setw(15) << std::left  << "  Team"
                  << std::setw(10) << std::right << "Win%"
                  << std::setw(10) << std::right << "Final%"
                  << std::setw(10) << std::right << "Semi%"
                  << std::setw(10) << std::right << "QF%"
                  << "\n" << RESET;
        divider();

        for (auto& [name, s] : sorted) {
            bool hl = (name == highlightTeam);
            if (hl) std::cout << YELLOW << BOLD;
            std::cout << std::setw(15) << std::left  << ("  " + name)
                      << std::setw(10) << std::right << std::fixed << std::setprecision(1) << s->winPct()  << "%"
                      << std::setw(9)  << std::right << std::fixed << std::setprecision(1) << s->finPct()  << "%"
                      << std::setw(9)  << std::right << std::fixed << std::setprecision(1) << s->sfPct()   << "%"
                      << std::setw(9)  << std::right << std::fixed << std::setprecision(1) << s->qfPct()   << "%"
                      << "\n";
            if (hl) std::cout << RESET;
        }
        std::cout << "\n";
    }
};
