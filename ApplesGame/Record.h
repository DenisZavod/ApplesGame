#pragma once
#include <string>
#include <vector>

namespace ApplesGame
{
    struct Record
    {
        std::string name;
        int score;
    };

    // Только объявление глобального списка
    extern std::vector<Record> leaderboard;
    void SortLeaderboard(std::vector<Record>& records);
}

