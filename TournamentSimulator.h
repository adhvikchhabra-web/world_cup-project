#pragma once
#include "MatchEngine.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

// One possible path through the tournament
struct TournamentPath {
    std::vector<std::string> opponents;   // in order: R16, QF, SF, F
    std::vector<double>      winProbs;    // probability of winning each match
    double                   totalProb;  // product of all match probs
    std::string              teamName;

    TournamentPath() : totalProb(0.0) {}
};

// Stats across all simulations
struct TeamStats {
    double probR16    = 0.0;
    double probQF     = 0.0;
    double probSF     = 0.0;
    double probFinal  = 0.0;
    double probWinner = 0.0;
};

class TournamentSimulator {
public:
    // All teams in the tournament, indexed by name
    std::unordered_map<std::string, Team>* teams;

    // Accumulated win probabilities per team
    std::unordered_map<std::string, TeamStats> stats;

    // All possible paths for the target team
    std::vector<TournamentPath> targetPaths;
    std::string targetTeam;

    // World Cup bracket: 8 groups of 2 top teams each
    // We model a simplified 16-team knockout bracket
    // bracket[i] = pair of teams in that slot
    std::vector<std::pair<std::string,std::string>> r16Bracket;

    TournamentSimulator(std::unordered_map<std::string, Team>* t,
                        const std::string& target)
        : teams(t), targetTeam(target) {
        // Initialize stats for all teams
        for (auto& kv : *teams)
            stats[kv.first] = TeamStats{};
    }

    // Set up the Round of 16 bracket
    void setBracket(const std::vector<std::pair<std::string,std::string>>& bracket) {
        r16Bracket = bracket;
    }

    // ── Main entry point ──────────────────────────────────────────────────────
    void simulate() {
        // Build 4 bracket halves (upper and lower)
        // We do full DFS over all possible outcomes
        simulateBracket(r16Bracket, 1.0, {});
    }

private:
    // Recursive DFS over the bracket
    // bracket: current round's matchups
    // prob:    accumulated probability of reaching this state
    // path:    current team's path (only populated for targetTeam)
    void simulateBracket(
            const std::vector<std::pair<std::string,std::string>>& bracket,
            double prob,
            TournamentPath currentPath) {

        if (bracket.size() == 1) {
            // This is the final
            auto& [teamA, teamB] = bracket[0];
            auto& A = (*teams)[teamA];
            auto& B = (*teams)[teamB];
            auto  res = MatchEngine::simulate(A, B);

            // Branch: A wins
            {
                double newProb = prob * res.probA;
                stats[teamA].probFinal  += prob;
                stats[teamB].probFinal  += prob;
                stats[teamA].probWinner += newProb;

                if (teamA == targetTeam || currentPath.teamName == teamA) {
                    TournamentPath p = currentPath;
                    p.teamName = teamA;
                    p.opponents.push_back(teamB);
                    p.winProbs.push_back(res.probA);
                    p.totalProb = newProb;
                    targetPaths.push_back(p);
                }
            }
            // Branch: B wins
            {
                double newProb = prob * res.probB;
                stats[teamB].probWinner += newProb;

                if (teamB == targetTeam || currentPath.teamName == teamB) {
                    TournamentPath p = currentPath;
                    p.teamName = teamB;
                    p.opponents.push_back(teamA);
                    p.winProbs.push_back(res.probB);
                    p.totalProb = newProb;
                    targetPaths.push_back(p);
                }
            }
            return;
        }

        // Simulate all matches in this round, get winners, recurse
        simulateRound(bracket, 0, prob, {}, currentPath,
                      (int)std::round(std::log2(bracket.size())));
    }

    // Iterate over all matchups in a round via recursion
    void simulateRound(
            const std::vector<std::pair<std::string,std::string>>& bracket,
            int matchIdx,
            double prob,
            std::vector<std::string> winners,
            TournamentPath currentPath,
            int roundNum) {

        if (matchIdx == (int)bracket.size()) {
            // All matches decided — build next round
            std::vector<std::pair<std::string,std::string>> nextBracket;
            for (int i = 0; i + 1 < (int)winners.size(); i += 2)
                nextBracket.push_back({winners[i], winners[i+1]});
            simulateBracket(nextBracket, prob, currentPath);
            return;
        }

        auto& [teamA, teamB] = bracket[matchIdx];
        if (teams->find(teamA) == teams->end() ||
            teams->find(teamB) == teams->end()) return;

        auto& A   = (*teams)[teamA];
        auto& B   = (*teams)[teamB];
        auto  res = MatchEngine::simulate(A, B);

        // Update round stats
        if (roundNum == 3) { // R16
            stats[teamA].probR16 += prob;
            stats[teamB].probR16 += prob;
        } else if (roundNum == 2) { // QF
            stats[teamA].probQF += prob;
            stats[teamB].probQF += prob;
        } else if (roundNum == 1) { // SF
            stats[teamA].probSF += prob;
            stats[teamB].probSF += prob;
        }

        // Branch A wins
        {
            TournamentPath pathA = currentPath;
            if (teamA == targetTeam || currentPath.teamName == teamA) {
                pathA.teamName = teamA;
                pathA.opponents.push_back(teamB);
                pathA.winProbs.push_back(res.probA);
            }
            auto w = winners; w.push_back(teamA);
            simulateRound(bracket, matchIdx+1, prob * res.probA,
                          w, pathA, roundNum);
        }
        // Branch B wins
        {
            TournamentPath pathB = currentPath;
            if (teamB == targetTeam || currentPath.teamName == teamB) {
                pathB.teamName = teamB;
                pathB.opponents.push_back(teamA);
                pathB.winProbs.push_back(res.probB);
            }
            auto w = winners; w.push_back(teamB);
            simulateRound(bracket, matchIdx+1, prob * res.probB,
                          w, pathB, roundNum);
        }
    }
};
