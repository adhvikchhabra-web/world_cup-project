#include "players/SquadDB.h"
#include "simulation/TournamentSimulator.h"
#include "simulation/MonteCarlo.h"
#include "io/Display.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

// ── World Cup bracket (Round of 16) ──────────────────────────────────────────
// Simplified but realistic 8-team bracket using our DB
std::vector<std::pair<std::string,std::string>> buildBracket() {
    return {
        {"Portugal",  "Morocco"},
        {"France",    "England"},
        {"Brazil",    "Germany"},
        {"Argentina", "Spain"}
    };
}

// ── Input helpers ─────────────────────────────────────────────────────────────
std::string pickTeam(const std::unordered_map<std::string, Team>& teams) {
    std::cout << CYAN << "\n  Available teams:\n" << RESET;
    int i = 1;
    std::vector<std::string> names;
    for (auto& kv : teams) { names.push_back(kv.first); }
    std::sort(names.begin(), names.end());
    for (auto& n : names)
        std::cout << "  " << YELLOW << i++ << ". " << RESET << n << "\n";

    std::cout << "\n  Enter team name: ";
    std::string choice;
    std::getline(std::cin, choice);

    // Try number
    try {
        int idx = std::stoi(choice);
        if (idx >= 1 && idx <= (int)names.size())
            return names[idx-1];
    } catch (...) {}

    // Try name (case-insensitive prefix)
    for (auto& n : names) {
        std::string nl = n, cl = choice;
        std::transform(nl.begin(),nl.end(),nl.begin(),::tolower);
        std::transform(cl.begin(),cl.end(),cl.begin(),::tolower);
        if (nl.find(cl) == 0) return n;
    }
    return "";
}

// ── Menu ──────────────────────────────────────────────────────────────────────
void showMenu() {
    std::cout << BOLD << "\n  ══ MAIN MENU ══\n" << RESET;
    std::cout << "  1. Show all tournament paths for a team\n";
    std::cout << "  2. Show Best XI for a team vs an opponent\n";
    std::cout << "  3. Run Monte Carlo simulation (10,000 tournaments)\n";
    std::cout << "  4. Head-to-head match simulation\n";
    std::cout << "  5. Show full bracket probabilities\n";
    std::cout << "  6. Exit\n";
    std::cout << "\n  Choice: ";
}

// ── Feature 1: Tournament paths ───────────────────────────────────────────────
void featurePaths(std::unordered_map<std::string, Team>& teams) {
    std::cout << "\n  Select your team:\n";
    std::string team = pickTeam(teams);
    if (team.empty() || teams.find(team) == teams.end()) {
        std::cout << RED << "  Team not found.\n" << RESET; return;
    }

    auto bracket = buildBracket();
    TournamentSimulator sim(&teams, team);
    sim.setBracket(bracket);
    sim.simulate();

    Display::printPaths(sim.targetPaths, team);

    // Show best XI for each match in the best path
    if (!sim.targetPaths.empty()) {
        auto& best = sim.targetPaths.front();
        std::cout << BOLD << GREEN << "  ── Best XI for each match on the BEST PATH ──\n\n" << RESET;
        for (auto& opp : best.opponents)
            Display::printBestXI(teams[team], opp, teams);
    }
}

// ── Feature 2: Best XI ────────────────────────────────────────────────────────
void featureBestXI(std::unordered_map<std::string, Team>& teams) {
    std::cout << "\n  Select YOUR team:\n";
    std::string team = pickTeam(teams);
    if (team.empty()) { std::cout << RED << "  Not found.\n" << RESET; return; }

    std::cout << "\n  Select OPPONENT:\n";
    std::string opp = pickTeam(teams);
    if (opp.empty() || opp == team) { std::cout << RED << "  Invalid.\n" << RESET; return; }

    Display::printBestXI(teams[team], opp, teams);
}

// ── Feature 3: Monte Carlo ────────────────────────────────────────────────────
void featureMonteCarlo(std::unordered_map<std::string, Team>& teams) {
    std::cout << "\n  Select team to highlight (or press Enter for none): ";
    std::string highlight;
    std::getline(std::cin, highlight);

    std::cout << YELLOW << "\n  Running 10,000 simulations...\n" << RESET;

    auto bracket = buildBracket();
    MonteCarloSimulator mc(&teams, bracket, 42);
    mc.run(10000);
    Display::printMonteCarloTable(mc.stats, highlight);
}

// ── Feature 4: Head to head ───────────────────────────────────────────────────
void featureH2H(std::unordered_map<std::string, Team>& teams) {
    std::cout << "\n  Select Team A:\n";
    std::string a = pickTeam(teams);
    std::cout << "\n  Select Team B:\n";
    std::string b = pickTeam(teams);

    if (a.empty()||b.empty()||a==b) { std::cout<<RED<<"  Invalid.\n"<<RESET; return; }

    auto& A = teams[a];
    auto& B = teams[b];
    auto res = MatchEngine::simulate(A, B);

    Display::divider("Head-to-Head: " + a + " vs " + b);
    std::cout << "  " << BOLD << a << RESET << "  win probability: "
              << GREEN << BOLD << std::fixed << std::setprecision(1)
              << res.probA*100 << "%" << RESET << "\n";
    std::cout << "  " << BOLD << b << RESET << "  win probability: "
              << YELLOW << BOLD << std::fixed << std::setprecision(1)
              << res.probB*100 << "%" << RESET << "\n\n";

    Display::printBestXI(A, b, teams);
    Display::printBestXI(B, a, teams);
}

// ── Feature 5: Full bracket ───────────────────────────────────────────────────
void featureBracket(std::unordered_map<std::string, Team>& teams) {
    auto bracket = buildBracket();
    Display::divider("Round of 16 Bracket Probabilities");

    for (auto& [a, b] : bracket) {
        auto& A = teams[a]; auto& B = teams[b];
        auto  res = MatchEngine::simulate(A, B);
        std::cout << "  " << std::setw(12) << std::left << a
                  << BOLD << GREEN << std::setw(6) << std::right
                  << std::fixed << std::setprecision(1) << res.probA*100 << "%" << RESET
                  << "  vs  "
                  << std::setw(12) << std::left << b
                  << BOLD << YELLOW << std::setw(6) << std::right
                  << res.probB*100 << "%" << RESET << "\n";
    }
    std::cout << "\n";
}

// ── Main ──────────────────────────────────────────────────────────────────────
int main() {
    auto teams = buildAllTeams();
    Display::banner();

    std::cout << GREEN << "  Loaded " << teams.size()
              << " national teams with full squads.\n" << RESET;

    while (true) {
        showMenu();
        std::string input;
        std::getline(std::cin, input);

        int choice = 0;
        try { choice = std::stoi(input); } catch (...) {}

        switch (choice) {
            case 1: featurePaths(teams);     break;
            case 2: featureBestXI(teams);    break;
            case 3: featureMonteCarlo(teams);break;
            case 4: featureH2H(teams);       break;
            case 5: featureBracket(teams);   break;
            case 6:
                std::cout << CYAN << "\n  Full time. Goodbye! ⚽\n\n" << RESET;
                return 0;
            default:
                std::cout << RED << "  Invalid choice.\n" << RESET;
        }
    }
}
