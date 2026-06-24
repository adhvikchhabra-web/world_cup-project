#pragma once
#include "Player.h"
#include <vector>
#include <string>
#include <algorithm>

struct Team {
    std::string name;
    std::string group;       // "A" – "H"

    // Base stats (0-10 scale, derived from FIFA rankings + historical data)
    double attack;
    double defense;
    double midfield;
    double overallRating;   // weighted composite

    std::vector<Player> squad;

    Team() : attack(5), defense(5), midfield(5), overallRating(5) {}
    Team(const std::string& n, double atk, double def, double mid)
        : name(n), attack(atk), defense(def), midfield(mid) {
        overallRating = 0.35*atk + 0.35*def + 0.30*mid;
    }

    // Get players for a specific position
    std::vector<Player*> byPosition(Position pos) {
        std::vector<Player*> result;
        for (auto& p : squad)
            if (p.position == pos) result.push_back(&p);
        return result;
    }

    // Effective attack rating after XI selection vs opponent
    double effectiveAttack(const std::string& opponent) const;
    double effectiveDefense(const std::string& opponent) const;
};
