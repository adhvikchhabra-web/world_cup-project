#pragma once
#include <string>
#include <unordered_map>

enum class Position { GK, RB, CB, LB, CDM, CM, CAM, LW, RW, ST };

std::string posToStr(Position p) {
    switch(p) {
        case Position::GK:  return "GK";
        case Position::RB:  return "RB";
        case Position::CB:  return "CB";
        case Position::LB:  return "LB";
        case Position::CDM: return "CDM";
        case Position::CM:  return "CM";
        case Position::CAM: return "CAM";
        case Position::LW:  return "LW";
        case Position::RW:  return "RW";
        case Position::ST:  return "ST";
        default:            return "??";
    }
}

struct Player {
    std::string name;
    Position    position;
    std::string nationality;        // which national team
    std::string club;

    double clubForm;                // 0-10: current club form/rating
    double internationalForm;       // 0-10: international record rating

    // per-opponent history: opponent country → score boost (0-10)
    std::unordered_map<std::string, double> vsOpponent;

    // overall score vs a specific opponent
    double scoreVs(const std::string& opponent) const {
        double h2h = 5.0; // neutral default
        auto it = vsOpponent.find(opponent);
        if (it != vsOpponent.end()) h2h = it->second;

        return 0.40 * clubForm
             + 0.30 * internationalForm
             + 0.30 * h2h;
    }

    Player() : clubForm(5.0), internationalForm(5.0) {}
    Player(const std::string& n, Position pos, const std::string& nat,
           const std::string& club, double cf, double intf)
        : name(n), position(pos), nationality(nat), club(club),
          clubForm(cf), internationalForm(intf) {}
};
