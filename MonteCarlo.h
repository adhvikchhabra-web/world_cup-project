#pragma once
#include "MatchEngine.h"
#include <random>
#include <unordered_map>
#include <string>
#include <vector>

struct MonteCarloStats {
    int   wins       = 0;
    int   finals     = 0;
    int   semifinals = 0;
    int   quarters   = 0;
    int   r16        = 0;
    int   totalRuns  = 0;

    double winPct()  const { return totalRuns ? 100.0*wins/totalRuns       : 0; }
    double finPct()  const { return totalRuns ? 100.0*finals/totalRuns     : 0; }
    double sfPct()   const { return totalRuns ? 100.0*semifinals/totalRuns : 0; }
    double qfPct()   const { return totalRuns ? 100.0*quarters/totalRuns   : 0; }
};

class MonteCarloSimulator {
public:
    std::unordered_map<std::string, Team>*   teams;
    std::vector<std::pair<std::string,std::string>> bracket;
    std::mt19937 rng;

    std::unordered_map<std::string, MonteCarloStats> stats;

    MonteCarloSimulator(std::unordered_map<std::string, Team>* t,
                        const std::vector<std::pair<std::string,std::string>>& b,
                        unsigned int seed = 42)
        : teams(t), bracket(b), rng(seed) {
        for (auto& kv : *t) stats[kv.first] = MonteCarloStats{};
    }

    // Run N simulations
    void run(int N = 10000) {
        for (int i = 0; i < N; i++) runOnce(N);
    }

private:
    // Simulate one match outcome stochastically
    std::string playMatch(const std::string& a, const std::string& b) {
        auto& A = (*teams)[a];
        auto& B = (*teams)[b];
        double pA = MatchEngine::winProb(A, B);
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng) < pA ? a : b;
    }

    void runOnce(int totalRuns) {
        auto currentBracket = bracket;
        int round = 0; // 0=R16, 1=SF, 2=Final

        while (currentBracket.size() >= 1) {
            std::vector<std::pair<std::string,std::string>> next;
            std::vector<std::string> roundWinners;

            for (auto& [a, b] : currentBracket) {
                std::string winner = playMatch(a, b);
                std::string loser  = (winner == a) ? b : a;

                // Both teams reached this round
                if (round == 0) {
                    stats[a].r16++; stats[b].r16++;
                } else if (round == 1) {
                    stats[a].quarters++; stats[b].quarters++;
                } else if (round == 2) {
                    stats[a].semifinals++; stats[b].semifinals++;
                } else {
                    stats[a].finals++; stats[b].finals++;
                }

                (void)loser;
                roundWinners.push_back(winner);
            }

            if (roundWinners.size() == 1) {
                // Champion
                stats[roundWinners[0]].wins++;
                break;
            }

            // Pair up winners for next round
            for (int i = 0; i+1 < (int)roundWinners.size(); i += 2)
                next.push_back({roundWinners[i], roundWinners[i+1]});

            currentBracket = next;
            round++;
        }

        for (auto& kv : *teams) stats[kv.first].totalRuns++;
    }
};
