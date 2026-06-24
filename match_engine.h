#pragma once
#include "teams.h"
#include <algorithm>
#include <map>
#include <iostream>
#include <iomanip>

// ─────────────────────────────────────────────
//  HEAD-TO-HEAD HISTORICAL BONUS TABLE
//  Key: "TeamA_vs_TeamB"  → bonus for TeamA attacking score
//  Positive = TeamA historically strong vs TeamB
// ─────────────────────────────────────────────

inline std::map<std::string, double> buildH2HTable() {
    std::map<std::string, double> h2h;

    // Argentina matchups
    h2h["Argentina_vs_Brazil"]      = -0.2;
    h2h["Argentina_vs_France"]      =  0.3;  // 2022 final
    h2h["Argentina_vs_Germany"]     =  0.2;  // 2014 final experience
    h2h["Argentina_vs_England"]     =  0.4;  // historical dominance
    h2h["Argentina_vs_Portugal"]    =  0.1;
    h2h["Argentina_vs_Spain"]       = -0.1;
    h2h["Argentina_vs_Netherlands"] =  0.2;

    // Brazil matchups
    h2h["Brazil_vs_Argentina"]      =  0.2;
    h2h["Brazil_vs_France"]         = -0.1;
    h2h["Brazil_vs_Germany"]        = -0.5;  // 7-1 scar
    h2h["Brazil_vs_England"]        =  0.3;
    h2h["Brazil_vs_Portugal"]       =  0.2;
    h2h["Brazil_vs_Spain"]          =  0.0;
    h2h["Brazil_vs_Netherlands"]    =  0.1;

    // France matchups
    h2h["France_vs_Argentina"]      = -0.3;
    h2h["France_vs_Brazil"]         =  0.1;
    h2h["France_vs_Germany"]        =  0.2;
    h2h["France_vs_England"]        =  0.2;
    h2h["France_vs_Portugal"]       =  0.1;
    h2h["France_vs_Spain"]          =  0.0;
    h2h["France_vs_Netherlands"]    =  0.3;

    // Germany matchups
    h2h["Germany_vs_Argentina"]     = -0.2;
    h2h["Germany_vs_Brazil"]        =  0.5;  // 7-1
    h2h["Germany_vs_France"]        = -0.2;
    h2h["Germany_vs_England"]       =  0.3;
    h2h["Germany_vs_Portugal"]      =  0.4;  // 4-0 in 2014
    h2h["Germany_vs_Spain"]         = -0.1;
    h2h["Germany_vs_Netherlands"]   =  0.2;

    // England matchups
    h2h["England_vs_Argentina"]     = -0.4;
    h2h["England_vs_Brazil"]        = -0.3;
    h2h["England_vs_France"]        = -0.2;
    h2h["England_vs_Germany"]       = -0.3;
    h2h["England_vs_Portugal"]      = -0.1;
    h2h["England_vs_Spain"]         = -0.2;
    h2h["England_vs_Netherlands"]   =  0.1;

    // Portugal matchups
    h2h["Portugal_vs_Argentina"]    = -0.1;
    h2h["Portugal_vs_Brazil"]       = -0.2;
    h2h["Portugal_vs_France"]       = -0.1;
    h2h["Portugal_vs_Germany"]      = -0.4;
    h2h["Portugal_vs_England"]      =  0.1;
    h2h["Portugal_vs_Spain"]        = -0.1;
    h2h["Portugal_vs_Netherlands"]  =  0.2;

    // Spain matchups
    h2h["Spain_vs_Argentina"]       =  0.1;
    h2h["Spain_vs_Brazil"]          =  0.0;
    h2h["Spain_vs_France"]          =  0.0;
    h2h["Spain_vs_Germany"]         =  0.1;
    h2h["Spain_vs_England"]         =  0.2;
    h2h["Spain_vs_Portugal"]        =  0.1;
    h2h["Spain_vs_Netherlands"]     =  0.2;

    // Netherlands matchups
    h2h["Netherlands_vs_Argentina"] = -0.2;
    h2h["Netherlands_vs_Brazil"]    = -0.1;
    h2h["Netherlands_vs_France"]    = -0.3;
    h2h["Netherlands_vs_Germany"]   = -0.2;
    h2h["Netherlands_vs_England"]   = -0.1;
    h2h["Netherlands_vs_Portugal"]  = -0.2;
    h2h["Netherlands_vs_Spain"]     = -0.2;

    return h2h;
}

// ─────────────────────────────────────────────
//  WIN PROBABILITY ENGINE
//  Incorporates attack/defense/midfield + H2H
// ─────────────────────────────────────────────

inline double winProbability(
    const Team& A,
    const Team& B,
    const std::map<std::string, double>& h2h)
{
    // Base offensive and defensive scores
    double scoreA = (A.attack * 0.5) + (A.midfield * 0.3) + (A.defense * 0.2);
    double scoreB = (B.attack * 0.5) + (B.midfield * 0.3) + (B.defense * 0.2);

    // Apply head-to-head historical bonus
    std::string keyA = A.name + "_vs_" + B.name;
    std::string keyB = B.name + "_vs_" + A.name;

    auto itA = h2h.find(keyA);
    if (itA != h2h.end()) scoreA += itA->second;

    auto itB = h2h.find(keyB);
    if (itB != h2h.end()) scoreB += itB->second;

    // Clamp scores to avoid negatives
    if (scoreA < 0.1) scoreA = 0.1;
    if (scoreB < 0.1) scoreB = 0.1;

    return scoreA / (scoreA + scoreB);
}

// ─────────────────────────────────────────────
//  PLAYER SCORE CALCULATOR
//  Weights: 40% club, 30% international, 30% H2H
// ─────────────────────────────────────────────

inline double playerScore(
    const Player& p,
    const std::string& opponentName,
    const std::map<std::string, double>& h2h,
    const std::string& teamName)
{
    // H2H bonus is team-level; apply a fraction to individual players
    double h2hBonus = 0.0;
    std::string key = teamName + "_vs_" + opponentName;
    auto it = h2h.find(key);
    if (it != h2h.end()) h2hBonus = it->second;

    return 0.40 * p.clubForm
         + 0.30 * p.intlForm
         + 0.30 * (5.0 + h2hBonus);  // 5.0 is neutral baseline
}

// ─────────────────────────────────────────────
//  BEST XI SELECTOR
//  Formation: 4-3-3
//  Picks best player per slot from the squad
// ─────────────────────────────────────────────

struct BestXI {
    std::string gk;
    std::string rb, cb1, cb2, lb;
    std::string cm1, cm2, cm3;
    std::string rw, st, lw;
};

inline BestXI selectBestXI(
    const Team& team,
    const std::string& opponentName,
    const std::map<std::string, double>& h2h)
{
    // Gather best player per position
    auto best = [&](const std::string& pos) -> std::string {
        std::string bestName = "N/A";
        double bestScore = -1.0;
        for (const auto& p : team.squad) {
            if (p.position == pos) {
                double s = playerScore(p, opponentName, h2h, team.name);
                if (s > bestScore) {
                    bestScore = s;
                    bestName = p.name;
                }
            }
        }
        return bestName;
    };

    // For CB we need two; track top 2
    auto top2CB = [&]() -> std::pair<std::string, std::string> {
        std::vector<std::pair<double, std::string>> cbs;
        for (const auto& p : team.squad) {
            if (p.position == "CB") {
                cbs.push_back({playerScore(p, opponentName, h2h, team.name), p.name});
            }
        }
        std::sort(cbs.rbegin(), cbs.rend());
        std::string a = cbs.size() > 0 ? cbs[0].second : "N/A";
        std::string b = cbs.size() > 1 ? cbs[1].second : "N/A";
        return {a, b};
    };

    // For CM we need three
    auto top3CM = [&]() -> std::tuple<std::string, std::string, std::string> {
        std::vector<std::pair<double, std::string>> cms;
        for (const auto& p : team.squad) {
            if (p.position == "CM") {
                cms.push_back({playerScore(p, opponentName, h2h, team.name), p.name});
            }
        }
        std::sort(cms.rbegin(), cms.rend());
        std::string a = cms.size() > 0 ? cms[0].second : "N/A";
        std::string b = cms.size() > 1 ? cms[1].second : "N/A";
        std::string c = cms.size() > 2 ? cms[2].second : "N/A";
        return {a, b, c};
    };

    BestXI xi;
    xi.gk  = best("GK");
    xi.rb  = best("RB");
    auto [cb1, cb2] = top2CB();
    xi.cb1 = cb1; xi.cb2 = cb2;
    xi.lb  = best("LB");
    auto [cm1, cm2, cm3] = top3CM();
    xi.cm1 = cm1; xi.cm2 = cm2; xi.cm3 = cm3;
    xi.rw  = best("RW");
    xi.st  = best("ST");
    xi.lw  = best("LW");

    return xi;
}

// ─────────────────────────────────────────────
//  TACTICAL ADVICE ENGINE
// ─────────────────────────────────────────────

inline void printTacticalAdvice(const Team& us, const Team& opponent) {
    std::cout << "\n  Tactical Advice:\n";

    if (opponent.attack > 8.8) {
        std::cout << "  - Opponent has elite attack — hold a deep defensive line.\n";
        std::cout << "  - Prioritize midfield compactness and limit through-balls.\n";
    } else if (opponent.attack < 8.0) {
        std::cout << "  - Opponent attack is beatable — press high and force errors.\n";
    }

    if (opponent.midfield > 8.7) {
        std::cout << "  - Opponent dominates midfield — use two holding midfielders.\n";
        std::cout << "  - Win second balls and disrupt their build-up early.\n";
    }

    if (opponent.defense < 8.0) {
        std::cout << "  - Opponent defense is vulnerable — exploit wings and use pace.\n";
    } else if (opponent.defense > 8.4) {
        std::cout << "  - Opponent defense is solid — be patient, wait for set pieces.\n";
    }

    if (us.attack > opponent.defense) {
        std::cout << "  - You have the attacking edge — play aggressive, press forward.\n";
    } else {
        std::cout << "  - Defensive solidity is key — absorb pressure, counter-attack.\n";
    }
}

// ─────────────────────────────────────────────
//  PRINT BEST XI
// ─────────────────────────────────────────────

inline void printBestXI(const BestXI& xi) {
    std::cout << "\n  Best XI (4-3-3):\n";
    std::cout << "  GK : " << xi.gk  << "\n";
    std::cout << "  RB : " << xi.rb  << "\n";
    std::cout << "  CB : " << xi.cb1 << "\n";
    std::cout << "  CB : " << xi.cb2 << "\n";
    std::cout << "  LB : " << xi.lb  << "\n";
    std::cout << "  CM : " << xi.cm1 << "\n";
    std::cout << "  CM : " << xi.cm2 << "\n";
    std::cout << "  CM : " << xi.cm3 << "\n";
    std::cout << "  RW : " << xi.rw  << "\n";
    std::cout << "  ST : " << xi.st  << "\n";
    std::cout << "  LW : " << xi.lw  << "\n";
}
