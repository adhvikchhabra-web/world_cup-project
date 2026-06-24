#pragma once
#include "Team.h"
#include <unordered_map>
#include <string>

// ── helper macro ─────────────────────────────────────────────────────────────
#define PLAYER(name, pos, nat, club, cf, intf) \
    Player(name, Position::pos, nat, club, cf, intf)

// ── Build all squads ─────────────────────────────────────────────────────────
inline void buildSquad_Portugal(Team& t) {
    t.squad = {
        PLAYER("Diogo Costa",      GK,  "Portugal", "FC Porto",           8.0, 7.5),
        PLAYER("Joao Cancelo",     RB,  "Portugal", "Man City",           7.5, 8.0),
        PLAYER("Ruben Dias",       CB,  "Portugal", "Man City",           9.0, 8.5),
        PLAYER("Pepe",             CB,  "Portugal", "FC Porto",           7.0, 8.0),
        PLAYER("Nuno Mendes",      LB,  "Portugal", "PSG",                8.5, 7.5),
        PLAYER("Joao Palhinha",    CDM, "Portugal", "Fulham",             8.0, 7.5),
        PLAYER("Bruno Fernandes",  CM,  "Portugal", "Man United",         8.5, 8.5),
        PLAYER("Bernardo Silva",   CM,  "Portugal", "Man City",           9.0, 8.5),
        PLAYER("Rafael Leao",      LW,  "Portugal", "AC Milan",           8.5, 7.5),
        PLAYER("Cristiano Ronaldo",ST,  "Portugal", "Al-Nassr",           6.5, 9.5),
        PLAYER("Pedro Neto",       RW,  "Portugal", "Chelsea",            8.0, 7.5),
        PLAYER("Vitinha",          CAM, "Portugal", "PSG",                8.0, 7.5),
        PLAYER("Joao Felix",       CAM, "Portugal", "Chelsea",            7.5, 7.5),
    };
    // h2h boosts
    t.squad[9].vsOpponent["Germany"] = 8.0; // Ronaldo vs Germany record
    t.squad[9].vsOpponent["Spain"]   = 7.5;
    t.squad[7].vsOpponent["France"]  = 8.5; // Bernardo vs France
}

inline void buildSquad_France(Team& t) {
    t.squad = {
        PLAYER("Mike Maignan",     GK,  "France", "AC Milan",            8.5, 8.0),
        PLAYER("Jules Kounde",     RB,  "France", "Barcelona",           8.5, 8.0),
        PLAYER("Dayot Upamecano", CB,  "France", "Bayern Munich",       8.5, 8.0),
        PLAYER("William Saliba",   CB,  "France", "Arsenal",             9.0, 8.0),
        PLAYER("Theo Hernandez",   LB,  "France", "AC Milan",            8.5, 8.0),
        PLAYER("Aurelien Tchouameni", CDM,"France","Real Madrid",        8.5, 8.0),
        PLAYER("Adrien Rabiot",    CM,  "France", "Juventus",            7.5, 7.5),
        PLAYER("Antoine Griezmann",CAM, "France", "Atletico Madrid",     8.5, 9.0),
        PLAYER("Ousmane Dembele",  RW,  "France", "Barcelona",           8.5, 7.5),
        PLAYER("Kylian Mbappe",    ST,  "France", "Real Madrid",         9.5, 9.5),
        PLAYER("Marcus Thuram",    LW,  "France", "Inter Milan",         8.5, 8.0),
    };
    t.squad[9].vsOpponent["Argentina"] = 9.5; // Mbappe WC2022 final
    t.squad[8].vsOpponent["Portugal"]  = 7.0;
}

inline void buildSquad_Brazil(Team& t) {
    t.squad = {
        PLAYER("Alisson",          GK,  "Brazil", "Liverpool",           9.0, 9.0),
        PLAYER("Danilo",           RB,  "Brazil", "Juventus",            7.5, 7.5),
        PLAYER("Marquinhos",       CB,  "Brazil", "PSG",                 8.5, 8.5),
        PLAYER("Gabriel Magalhaes",CB,  "Brazil", "Arsenal",             8.5, 7.5),
        PLAYER("Guilherme Arana",  LB,  "Brazil", "Atletico Mineiro",    7.5, 7.0),
        PLAYER("Casemiro",         CDM, "Brazil", "Man United",          8.0, 8.5),
        PLAYER("Lucas Paqueta",    CM,  "Brazil", "West Ham",            8.0, 8.0),
        PLAYER("Rodrygo",          RW,  "Brazil", "Real Madrid",         8.5, 8.0),
        PLAYER("Vinicius Jr",      LW,  "Brazil", "Real Madrid",         9.5, 8.5),
        PLAYER("Endrick",          ST,  "Brazil", "Real Madrid",         8.5, 7.5),
        PLAYER("Raphinha",         CAM, "Brazil", "Barcelona",           8.5, 8.0),
    };
    t.squad[8].vsOpponent["Argentina"] = 9.0; // Vini vs Argentina
}

inline void buildSquad_Argentina(Team& t) {
    t.squad = {
        PLAYER("Emiliano Martinez", GK, "Argentina","Aston Villa",       9.0, 9.5),
        PLAYER("Nahuel Molina",    RB,  "Argentina","Atletico Madrid",   8.0, 8.0),
        PLAYER("Cristian Romero",  CB,  "Argentina","Tottenham",         8.5, 8.5),
        PLAYER("Lisandro Martinez",CB,  "Argentina","Man United",        8.5, 8.0),
        PLAYER("Nicolas Tagliafico",LB, "Argentina","Lyon",              7.5, 8.0),
        PLAYER("Rodrigo De Paul",  CDM, "Argentina","Atletico Madrid",   8.5, 9.0),
        PLAYER("Enzo Fernandez",   CM,  "Argentina","Chelsea",           8.5, 8.5),
        PLAYER("Alexis Mac Allister",CM,"Argentina","Liverpool",         8.5, 8.0),
        PLAYER("Lionel Messi",     CAM, "Argentina","Inter Miami",       8.5, 10.0),
        PLAYER("Lautaro Martinez", ST,  "Argentina","Inter Milan",       9.0, 8.5),
        PLAYER("Julian Alvarez",   LW,  "Argentina","Atletico Madrid",   8.5, 8.5),
    };
    t.squad[8].vsOpponent["France"]  = 10.0; // Messi WC2022 final
    t.squad[8].vsOpponent["Brazil"]  = 9.0;
    t.squad[8].vsOpponent["Germany"] = 9.5; // WC2014 final
}

inline void buildSquad_Germany(Team& t) {
    t.squad = {
        PLAYER("Manuel Neuer",     GK,  "Germany","Bayern Munich",       8.5, 9.0),
        PLAYER("Joshua Kimmich",   RB,  "Germany","Bayern Munich",       9.0, 9.0),
        PLAYER("Antonio Rudiger",  CB,  "Germany","Real Madrid",         8.5, 8.5),
        PLAYER("Jonathan Tah",     CB,  "Germany","Bayer Leverkusen",    8.5, 7.5),
        PLAYER("David Raum",       LB,  "Germany","RB Leipzig",          8.0, 7.5),
        PLAYER("Toni Kroos",       CDM, "Germany","Real Madrid",         8.5, 9.0),
        PLAYER("Florian Wirtz",    CAM, "Germany","Bayer Leverkusen",    9.0, 8.5),
        PLAYER("Jamal Musiala",    CM,  "Germany","Bayern Munich",       9.0, 8.5),
        PLAYER("Leroy Sane",       RW,  "Germany","Bayern Munich",       8.5, 8.0),
        PLAYER("Kai Havertz",      ST,  "Germany","Arsenal",             8.5, 8.0),
        PLAYER("Serge Gnabry",     LW,  "Germany","Bayern Munich",       8.5, 8.5),
    };
}

inline void buildSquad_Spain(Team& t) {
    t.squad = {
        PLAYER("David Raya",       GK,  "Spain","Arsenal",              8.5, 7.5),
        PLAYER("Dani Carvajal",    RB,  "Spain","Real Madrid",          8.5, 8.5),
        PLAYER("Aymeric Laporte",  CB,  "Spain","Al-Nassr",             8.0, 8.5),
        PLAYER("Robin Le Normand", CB,  "Spain","Atletico Madrid",      8.5, 7.5),
        PLAYER("Marc Cucurella",   LB,  "Spain","Chelsea",              8.0, 8.0),
        PLAYER("Rodri",            CDM, "Spain","Man City",             9.5, 9.0),
        PLAYER("Pedri",            CM,  "Spain","Barcelona",            9.0, 8.5),
        PLAYER("Fabian Ruiz",      CM,  "Spain","PSG",                  8.5, 8.5),
        PLAYER("Lamine Yamal",     RW,  "Spain","Barcelona",            9.0, 8.5),
        PLAYER("Alvaro Morata",    ST,  "Spain","AC Milan",             8.0, 8.5),
        PLAYER("Nico Williams",    LW,  "Spain","Athletic Bilbao",      8.5, 8.0),
    };
    t.squad[9].vsOpponent["Germany"] = 9.0; // Spain beat Germany Euro 2024
}

inline void buildSquad_England(Team& t) {
    t.squad = {
        PLAYER("Jordan Pickford",  GK,  "England","Everton",            8.0, 8.5),
        PLAYER("Trent Alexander-Arnold",RB,"England","Real Madrid",     8.5, 8.0),
        PLAYER("John Stones",      CB,  "England","Man City",           8.5, 8.0),
        PLAYER("Marc Guehi",       CB,  "England","Crystal Palace",     8.5, 7.5),
        PLAYER("Luke Shaw",        LB,  "England","Man United",         7.5, 8.0),
        PLAYER("Declan Rice",      CDM, "England","Arsenal",            9.0, 8.5),
        PLAYER("Jude Bellingham",  CM,  "England","Real Madrid",        9.5, 9.0),
        PLAYER("Phil Foden",       CAM, "England","Man City",           9.0, 8.0),
        PLAYER("Bukayo Saka",      RW,  "England","Arsenal",            9.0, 8.5),
        PLAYER("Harry Kane",       ST,  "England","Bayern Munich",      9.0, 9.0),
        PLAYER("Marcus Rashford",  LW,  "England","Man United",         8.0, 7.5),
    };
}

inline void buildSquad_Morocco(Team& t) {
    t.squad = {
        PLAYER("Yassine Bounou",   GK,  "Morocco","Al-Hilal",           9.0, 9.0),
        PLAYER("Achraf Hakimi",    RB,  "Morocco","PSG",                9.0, 9.0),
        PLAYER("Nayef Aguerd",     CB,  "Morocco","West Ham",           8.0, 8.5),
        PLAYER("Romain Saiss",     CB,  "Morocco","Besiktas",           7.5, 8.5),
        PLAYER("Noussair Mazraoui",LB,  "Morocco","Man United",         8.0, 8.5),
        PLAYER("Sofyan Amrabat",   CDM, "Morocco","Fiorentina",         8.5, 9.0),
        PLAYER("Azzedine Ounahi",  CM,  "Morocco","Marseille",          8.0, 8.0),
        PLAYER("Selim Amallah",    CM,  "Morocco","Standard Liege",     7.5, 7.5),
        PLAYER("Hakim Ziyech",     RW,  "Morocco","Galatasaray",        8.0, 8.5),
        PLAYER("Youssef En-Nesyri",ST,  "Morocco","Fenerbahce",         8.5, 9.0),
        PLAYER("Soufiane Boufal",  LW,  "Morocco","Southampton",        7.5, 8.0),
    };
}

// ── Master database factory ──────────────────────────────────────────────────
inline std::unordered_map<std::string, Team> buildAllTeams() {
    std::unordered_map<std::string, Team> db;

    // (name, attack, defense, midfield) — all 0-10
    db["Portugal"]  = Team("Portugal",  8.2, 7.8, 8.5); db["Portugal"].group  = "A";
    db["France"]    = Team("France",    9.0, 8.5, 8.8); db["France"].group    = "D";
    db["Brazil"]    = Team("Brazil",    8.8, 8.2, 8.5); db["Brazil"].group    = "G";
    db["Argentina"] = Team("Argentina", 9.0, 8.3, 8.8); db["Argentina"].group = "C";
    db["Germany"]   = Team("Germany",   8.5, 8.2, 9.0); db["Germany"].group   = "A";
    db["Spain"]     = Team("Spain",     8.5, 8.5, 9.2); db["Spain"].group     = "B";
    db["England"]   = Team("England",   8.5, 8.3, 8.8); db["England"].group   = "C";
    db["Morocco"]   = Team("Morocco",   7.5, 9.0, 8.0); db["Morocco"].group   = "F";

    buildSquad_Portugal (db["Portugal"]);
    buildSquad_France   (db["France"]);
    buildSquad_Brazil   (db["Brazil"]);
    buildSquad_Argentina(db["Argentina"]);
    buildSquad_Germany  (db["Germany"]);
    buildSquad_Spain    (db["Spain"]);
    buildSquad_England  (db["England"]);
    buildSquad_Morocco  (db["Morocco"]);

    return db;
}
