#pragma once
#include "../players/Team.h"
#include <cmath>
#include <string>

// ── Best XI result ────────────────────────────────────────────────────────────
struct BestXI {
    std::vector<Player*> lineup;    // exactly 11 players
    std::string formation;
    std::string tacticalNote;
    double effectiveAttack;
    double effectiveDefense;
};

// ── Match result ──────────────────────────────────────────────────────────────
struct MatchResult {
    double probA;   // probability team A wins
    double probB;
    BestXI xiA;
    BestXI xiB;
};

class MatchEngine {
public:
    // Core win probability: based on attack vs defence + midfield control
    static double winProb(const Team& A, const Team& B,
                          double aAtkBoost = 0.0, double aDefBoost = 0.0) {
        double atkA = A.attack    + aAtkBoost;
        double atkB = B.attack;
        double defA = A.defense   + aDefBoost;
        double defB = B.defense;
        double midA = A.midfield;
        double midB = B.midfield;

        // Composite score
        double scoreA = 0.40 * atkA + 0.35 * defA + 0.25 * midA;
        double scoreB = 0.40 * atkB + 0.35 * defB + 0.25 * midB;

        // Logistic transform → probability
        double diff = scoreA - scoreB;
        return 1.0 / (1.0 + std::exp(-0.8 * diff));
    }

    // Build best XI for teamA against teamB
    static BestXI buildBestXI(Team& A, const std::string& opponentName,
                               const Team& B) {
        BestXI xi;
        xi.effectiveAttack  = 0;
        xi.effectiveDefense = 0;

        // Positions needed for 4-3-3 (default)
        // If opponent defense > 8.5 → use 4-2-3-1 (more defensive)
        // If opponent defense < 7.5 → use 3-5-2 (ultra attack)
        bool strongOpp = (B.defense > 8.3);
        bool weakOpp   = (B.defense < 7.5);

        std::vector<std::pair<Position, int>> needs;
        if (strongOpp) {
            xi.formation = "4-2-3-1";
            needs = {{Position::GK,1},{Position::RB,1},{Position::CB,2},
                     {Position::LB,1},{Position::CDM,2},{Position::CAM,1},
                     {Position::RW,1},{Position::LW,1},{Position::ST,1}};
            xi.tacticalNote = "Compact shape, double pivot, counter on wings";
        } else if (weakOpp) {
            xi.formation = "3-5-2";
            needs = {{Position::GK,1},{Position::CB,3},{Position::CDM,1},
                     {Position::CM,2},{Position::RW,1},{Position::LW,1},
                     {Position::ST,2}};
            xi.tacticalNote = "Dominant possession, overload midfield, two strikers";
        } else {
            xi.formation = "4-3-3";
            needs = {{Position::GK,1},{Position::RB,1},{Position::CB,2},
                     {Position::LB,1},{Position::CM,2},{Position::CDM,1},
                     {Position::RW,1},{Position::LW,1},{Position::ST,1}};
            xi.tacticalNote = "Balanced press, wing play, high line";
        }

        // For each position slot, pick best scorer vs opponent
        for (auto& [pos, count] : needs) {
            std::vector<Player*> candidates;
            for (auto& p : A.squad)
                if (p.position == pos) candidates.push_back(&p);

            // Sort by scoreVs opponent descending
            std::sort(candidates.begin(), candidates.end(),
                [&](Player* a, Player* b) {
                    return a->scoreVs(opponentName) > b->scoreVs(opponentName);
                });

            int picked = 0;
            for (auto* p : candidates) {
                if (picked >= count) break;
                // Avoid picking same player twice
                bool alreadyIn = false;
                for (auto* sel : xi.lineup)
                    if (sel->name == p->name) { alreadyIn = true; break; }
                if (!alreadyIn) {
                    xi.lineup.push_back(p);
                    xi.effectiveAttack  += p->scoreVs(opponentName);
                    xi.effectiveDefense += p->scoreVs(opponentName);
                    picked++;
                }
            }
        }

        xi.effectiveAttack  /= 11.0;
        xi.effectiveDefense /= 11.0;
        return xi;
    }

    // Full match simulation with XI selection
    static MatchResult simulate(Team& A, Team& B) {
        MatchResult r;
        r.xiA = buildBestXI(A, B.name, B);
        r.xiB = buildBestXI(B, A.name, A);

        double atkBoostA = (r.xiA.effectiveAttack  - 5.0) * 0.1;
        double defBoostA = (r.xiA.effectiveDefense - 5.0) * 0.1;

        r.probA = winProb(A, B, atkBoostA, defBoostA);
        r.probB = 1.0 - r.probA;
        return r;
    }
};
