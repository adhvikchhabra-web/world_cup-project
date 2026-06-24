#pragma once
#include "match_engine.h"
#include <vector>
#include <string>
#include <limits>
#include <iostream>
#include <iomanip>
#include <sstream>

// ─────────────────────────────────────────────
//  PATH RESULT: stores one complete run to final
// ─────────────────────────────────────────────

struct PathResult {
    std::vector<std::string> opponents;   // teams faced in order
    double probability;                   // cumulative win probability
    bool reachedFinal;
    bool wonFinal;
};

// ─────────────────────────────────────────────
//  TOURNAMENT CLASS
// ─────────────────────────────────────────────

class Tournament {
public:
    // The 8-team bracket (Round of 8 → SF → Final)
    // Slots: [0..7] — bracket positions
    // QF matches: (0v1), (2v3), (4v5), (6v7)
    // SF matches: winners of (0v1) vs (2v3), winners of (4v5) vs (6v7)
    // Final:      both SF winners

    std::vector<std::string> bracket;        // 8 team names in bracket order
    std::unordered_map<std::string, Team>& teams;
    std::map<std::string, double>& h2h;

    std::vector<PathResult> allPaths;        // all paths where target team participates
    PathResult bestPath;
    PathResult worstPath;

    Tournament(
        std::vector<std::string> b,
        std::unordered_map<std::string, Team>& t,
        std::map<std::string, double>& h)
        : bracket(b), teams(t), h2h(h)
    {
        bestPath.probability  = -1.0;
        worstPath.probability =  2.0;
    }

    // ── MAIN ENTRY POINT ──────────────────────
    void simulate(const std::string& targetTeam) {
        allPaths.clear();
        bestPath.probability  = -1.0;
        worstPath.probability =  2.0;

        // Start DFS from the full bracket
        // We pass copies of the bracket slot arrays at each round
        std::vector<std::string> currentRound = bracket;
        std::vector<std::string> pathSoFar;
        double cumulativeProb = 1.0;

        dfs(currentRound, targetTeam, pathSoFar, cumulativeProb);
    }

private:
    // ── DFS OVER TOURNAMENT ROUNDS ────────────
    // currentRound: list of teams still alive (must be even-sized)
    // After each round, only winners advance to the next call
    void dfs(
        const std::vector<std::string>& currentRound,
        const std::string& targetTeam,
        std::vector<std::string> pathSoFar,
        double cumulativeProb)
    {
        // Base case: final has been played (only 1 team left)
        if (currentRound.size() == 1) {
            PathResult result;
            result.opponents   = pathSoFar;
            result.probability = cumulativeProb;
            result.reachedFinal = true;
            result.wonFinal     = (currentRound[0] == targetTeam);

            if (result.wonFinal) {
                allPaths.push_back(result);
                if (result.probability > bestPath.probability)  bestPath  = result;
                if (result.probability < worstPath.probability) worstPath = result;
            }
            return;
        }

        // Check if target team is still alive
        bool targetAlive = false;
        for (const auto& t : currentRound) {
            if (t == targetTeam) { targetAlive = true; break; }
        }
        if (!targetAlive) return;  // prune: target already eliminated

        // Play all matches in this round, explore both outcomes
        // We generate all combinations of round winners via recursion
        playRound(currentRound, 0, {}, targetTeam, pathSoFar, cumulativeProb);
    }

    // ── PLAY ALL MATCHES IN A ROUND ───────────
    // matchIdx: index of the current pair being resolved (0, 2, 4, ...)
    void playRound(
        const std::vector<std::string>& roundTeams,
        int matchIdx,
        std::vector<std::string> winnersAccum,
        const std::string& targetTeam,
        std::vector<std::string> pathSoFar,
        double cumulativeProb)
    {
        int n = static_cast<int>(roundTeams.size());

        // All matches in this round resolved → recurse to next round
        if (matchIdx >= n) {
            dfs(winnersAccum, targetTeam, pathSoFar, cumulativeProb);
            return;
        }

        const std::string& teamA = roundTeams[matchIdx];
        const std::string& teamB = roundTeams[matchIdx + 1];

        const Team& A = teams.at(teamA);
        const Team& B = teams.at(teamB);

        double probAWins = winProbability(A, B, h2h);
        double probBWins = 1.0 - probAWins;

        // ── Branch 1: A wins ──────────────────
        {
            std::vector<std::string> newPath  = pathSoFar;
            std::vector<std::string> newWinners = winnersAccum;
            newWinners.push_back(teamA);

            double newProb = cumulativeProb;

            // Only track path/prob for the target team's matches
            if (teamA == targetTeam || teamB == targetTeam) {
                if (teamA == targetTeam) {
                    newPath.push_back(teamB);   // target faced teamB and won
                    newProb *= probAWins;
                } else {
                    // target (teamB) lost → prune this branch
                    // (don't recurse since target is eliminated)
                    goto branchB;
                }
            }

            playRound(roundTeams, matchIdx + 2, newWinners, targetTeam, newPath, newProb);
        }

        branchB:
        // ── Branch 2: B wins ──────────────────
        {
            std::vector<std::string> newPath  = pathSoFar;
            std::vector<std::string> newWinners = winnersAccum;
            newWinners.push_back(teamB);

            double newProb = cumulativeProb;

            if (teamA == targetTeam || teamB == targetTeam) {
                if (teamB == targetTeam) {
                    newPath.push_back(teamA);   // target faced teamA and won
                    newProb *= probBWins;
                } else {
                    // target (teamA) lost → prune
                    return;
                }
            }

            playRound(roundTeams, matchIdx + 2, newWinners, targetTeam, newPath, newProb);
        }
    }
};

// ─────────────────────────────────────────────
//  PRINT HELPERS
// ─────────────────────────────────────────────

inline void printPath(
    const std::string& label,
    const std::string& teamName,
    const PathResult& path)
{
    std::cout << "\n  " << label << ":\n";
    std::cout << "  " << teamName;
    for (const auto& opp : path.opponents) {
        std::cout << " → " << opp;
    }
    std::cout << "\n";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  Win Probability: " << (path.probability * 100.0) << "%\n";
}

inline void printMatchupDetail(
    const std::string& teamName,
    const std::vector<PathResult>& paths,
    const std::unordered_map<std::string, Team>& teams,
    const std::map<std::string, double>& h2h)
{
    if (paths.empty()) {
        std::cout << "  No winning paths found for " << teamName << ".\n";
        return;
    }

    // Show best path matchup details
    const PathResult& best = *std::max_element(
        paths.begin(), paths.end(),
        [](const PathResult& a, const PathResult& b) {
            return a.probability < b.probability;
        });

    std::cout << "\n──────────────────────────────────────────\n";
    std::cout << "  MATCH-BY-MATCH BREAKDOWN (Best Path)\n";
    std::cout << "──────────────────────────────────────────\n";

    const Team& us = teams.at(teamName);
    double runningProb = 1.0;
    std::string round;

    int n = static_cast<int>(best.opponents.size());
    for (int i = 0; i < n; ++i) {
        if (n == 3)      round = (i == 0) ? "Quarter-Final" : (i == 1) ? "Semi-Final" : "Final";
        else if (n == 2) round = (i == 0) ? "Semi-Final" : "Final";
        else             round = "Final";

        const std::string& oppName = best.opponents[i];
        const Team& opp = teams.at(oppName);
        double prob = winProbability(us, opp, h2h);
        runningProb *= prob;

        std::cout << "\n  [" << round << "] " << teamName << " vs " << oppName << "\n";
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "  Match win probability: " << (prob * 100.0) << "%\n";

        BestXI xi = selectBestXI(us, oppName, h2h);
        printBestXI(xi);
        printTacticalAdvice(us, opp);
    }
}
