#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// ─────────────────────────────────────────────
//  DATA STRUCTURES
// ─────────────────────────────────────────────

struct Player {
    std::string name;
    std::string position;   // GK, RB, CB, LB, CM, RW, LW, ST
    double clubForm;        // 0-10  (recent club season rating)
    double intlForm;        // 0-10  (international tournament rating)
    // per-opponent bonuses are applied dynamically in match_engine
};

struct Team {
    std::string name;
    double attack;
    double defense;
    double midfield;
    std::vector<Player> squad;  // full squad (15+ players)
};

// ─────────────────────────────────────────────
//  SQUAD DEFINITIONS  (8 major nations)
// ─────────────────────────────────────────────

inline std::unordered_map<std::string, Team> buildTeams() {

    std::unordered_map<std::string, Team> teams;

    // ── ARGENTINA ──────────────────────────────
    teams["Argentina"] = {
        "Argentina", 9.1, 7.8, 8.5,
        {
            {"Emiliano Martinez", "GK",  8.5, 9.0},
            {"Nahuel Molina",     "RB",  7.8, 7.5},
            {"Cristian Romero",   "CB",  8.2, 8.0},
            {"Nicolas Otamendi",  "CB",  7.9, 8.5},
            {"Nicolas Tagliafico","LB",  7.6, 7.8},
            {"Rodrigo De Paul",   "CM",  8.3, 8.8},
            {"Enzo Fernandez",    "CM",  8.7, 8.5},
            {"Alexis Mac Allister","CM", 8.6, 8.4},
            {"Angel Di Maria",    "RW",  8.0, 8.9},
            {"Lautaro Martinez",  "ST",  8.8, 8.6},
            {"Lionel Messi",      "LW",  9.8, 9.9},
            {"Geronimo Rulli",    "GK",  7.2, 7.0},
            {"Lisandro Martinez", "CB",  8.4, 8.0},
            {"Leandro Paredes",   "CM",  7.8, 7.9},
            {"Julian Alvarez",    "ST",  8.5, 8.7}
        }
    };

    // ── BRAZIL ─────────────────────────────────
    teams["Brazil"] = {
        "Brazil", 8.9, 7.9, 8.6,
        {
            {"Alisson",       "GK",  9.0, 8.8},
            {"Danilo",        "RB",  8.0, 8.0},
            {"Marquinhos",    "CB",  8.5, 8.7},
            {"Eder Militao",  "CB",  8.3, 8.2},
            {"Alex Sandro",   "LB",  7.5, 7.6},
            {"Casemiro",      "CM",  8.6, 8.9},
            {"Lucas Paqueta", "CM",  8.4, 8.5},
            {"Bruno Guimaraes","CM", 8.5, 8.3},
            {"Raphinha",      "RW",  8.2, 8.0},
            {"Richarlison",   "ST",  8.3, 8.6},
            {"Vinicius Jr",   "LW",  9.2, 8.8},
            {"Ederson",       "GK",  8.8, 8.5},
            {"Thiago Silva",  "CB",  8.0, 8.8},
            {"Rodrygo",       "RW",  8.3, 8.2},
            {"Gabriel Jesus", "ST",  7.9, 8.1}
        }
    };

    // ── FRANCE ─────────────────────────────────
    teams["France"] = {
        "France", 9.0, 8.2, 8.7,
        {
            {"Hugo Lloris",      "GK",  8.6, 9.0},
            {"Benjamin Pavard",  "RB",  8.0, 8.2},
            {"Raphael Varane",   "CB",  8.3, 8.8},
            {"Dayot Upamecano", "CB",   8.5, 8.3},
            {"Theo Hernandez",   "LB",  8.4, 8.0},
            {"N'Golo Kante",     "CM",  8.8, 9.0},
            {"Aurelien Tchouameni","CM",8.6, 8.4},
            {"Antoine Griezmann","CM",  8.7, 9.1},
            {"Ousmane Dembele",  "RW",  8.5, 8.2},
            {"Olivier Giroud",   "ST",  8.1, 8.6},
            {"Kylian Mbappe",    "LW",  9.7, 9.5},
            {"Mike Maignan",     "GK",  8.7, 8.0},
            {"Jules Kounde",     "CB",  8.4, 8.1},
            {"Marcus Thuram",    "ST",  8.2, 8.0},
            {"Kingsley Coman",   "RW",  8.0, 8.3}
        }
    };

    // ── GERMANY ────────────────────────────────
    teams["Germany"] = {
        "Germany", 8.6, 8.3, 8.8,
        {
            {"Manuel Neuer",     "GK",  9.0, 9.2},
            {"Joshua Kimmich",   "RB",  9.0, 9.0},
            {"Antonio Rudiger",  "CB",  8.5, 8.6},
            {"Nico Schlotterbeck","CB", 8.2, 8.0},
            {"David Raum",       "LB",  8.0, 7.9},
            {"Leon Goretzka",    "CM",  8.3, 8.5},
            {"Ilkay Gundogan",   "CM",  8.7, 8.8},
            {"Jamal Musiala",    "CM",  9.0, 8.7},
            {"Serge Gnabry",     "RW",  8.4, 8.5},
            {"Thomas Muller",    "ST",  8.2, 9.0},
            {"Kai Havertz",      "LW",  8.3, 8.4},
            {"Marc-Andre ter Stegen","GK",8.8,8.6},
            {"Matthias Ginter",  "CB",  7.8, 8.0},
            {"Leroy Sane",       "RW",  8.5, 8.3},
            {"Niclas Fullkrug",  "ST",  8.0, 8.2}
        }
    };

    // ── ENGLAND ────────────────────────────────
    teams["England"] = {
        "England", 8.5, 8.0, 8.4,
        {
            {"Jordan Pickford",  "GK",  8.2, 8.5},
            {"Trent Alexander-Arnold","RB",8.8,8.6},
            {"Harry Maguire",    "CB",  7.8, 8.0},
            {"John Stones",      "CB",  8.3, 8.5},
            {"Luke Shaw",        "LB",  8.0, 8.2},
            {"Declan Rice",      "CM",  8.7, 8.6},
            {"Jude Bellingham",  "CM",  9.2, 9.0},
            {"Mason Mount",      "CM",  8.2, 8.0},
            {"Bukayo Saka",      "RW",  8.8, 8.5},
            {"Harry Kane",       "ST",  9.0, 9.1},
            {"Marcus Rashford",  "LW",  8.3, 8.2},
            {"Nick Pope",        "GK",  8.0, 7.8},
            {"Eric Dier",        "CB",  7.6, 7.8},
            {"Phil Foden",       "CM",  8.9, 8.4},
            {"Raheem Sterling",  "RW",  8.0, 8.3}
        }
    };

    // ── PORTUGAL ───────────────────────────────
    teams["Portugal"] = {
        "Portugal", 8.8, 7.7, 8.3,
        {
            {"Diogo Costa",      "GK",  8.4, 8.2},
            {"Joao Cancelo",     "RB",  8.7, 8.5},
            {"Ruben Dias",       "CB",  9.0, 8.8},
            {"Pepe",             "CB",  7.5, 8.4},
            {"Nuno Mendes",      "LB",  8.3, 8.0},
            {"Bernardo Silva",   "CM",  9.0, 8.9},
            {"Vitinha",          "CM",  8.5, 8.2},
            {"Bruno Fernandes",  "CM",  8.8, 8.7},
            {"Rafael Leao",      "RW",  8.9, 8.3},
            {"Cristiano Ronaldo","ST",  8.5, 9.5},
            {"Joao Felix",       "LW",  8.6, 8.4},
            {"Rui Patricio",     "GK",  8.0, 8.5},
            {"Antonio Silva",    "CB",  8.3, 7.9},
            {"Matheus Nunes",    "CM",  8.2, 7.8},
            {"Goncalo Ramos",    "ST",  8.4, 8.0}
        }
    };

    // ── SPAIN ──────────────────────────────────
    teams["Spain"] = {
        "Spain", 8.7, 8.1, 9.0,
        {
            {"Unai Simon",       "GK",  8.3, 8.4},
            {"Dani Carvajal",    "RB",  8.5, 8.7},
            {"Aymeric Laporte",  "CB",  8.4, 8.5},
            {"Pau Torres",       "CB",  8.2, 8.3},
            {"Jose Gaya",        "LB",  8.0, 8.1},
            {"Sergio Busquets",  "CM",  8.3, 9.0},
            {"Pedri",            "CM",  9.0, 8.8},
            {"Gavi",             "CM",  8.9, 8.7},
            {"Dani Olmo",        "RW",  8.4, 8.3},
            {"Alvaro Morata",    "ST",  8.0, 8.2},
            {"Ferran Torres",    "LW",  8.2, 8.0},
            {"David Raya",       "GK",  8.2, 7.9},
            {"Eric Garcia",      "CB",  7.9, 8.0},
            {"Marcos Llorente",  "CM",  8.1, 8.2},
            {"Ansu Fati",        "LW",  8.3, 8.0}
        }
    };

    // ── NETHERLANDS ────────────────────────────
    teams["Netherlands"] = {
        "Netherlands", 8.4, 7.9, 8.2,
        {
            {"Andries Noppert",  "GK",  7.8, 7.5},
            {"Denzel Dumfries",  "RB",  8.4, 8.2},
            {"Virgil van Dijk",  "CB",  9.0, 8.8},
            {"Matthijs de Ligt", "CB",  8.6, 8.4},
            {"Daley Blind",      "LB",  7.8, 8.0},
            {"Frenkie de Jong",  "CM",  8.8, 8.6},
            {"Marten de Roon",   "CM",  8.0, 8.1},
            {"Steven Bergwijn",  "CM",  8.1, 7.9},
            {"Cody Gakpo",       "RW",  8.7, 8.5},
            {"Memphis Depay",    "ST",  8.3, 8.6},
            {"Davy Klaassen",    "LW",  7.8, 7.9},
            {"Jasper Cillessen", "GK",  7.9, 8.0},
            {"Nathan Ake",       "CB",  8.2, 8.0},
            {"Teun Koopmeiners", "CM",  8.4, 8.0},
            {"Wout Weghorst",    "ST",  8.0, 8.3}
        }
    };

    return teams;
}
