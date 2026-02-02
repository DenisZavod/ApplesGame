#include "Record.h"

namespace ApplesGame
{
    std::vector<Record> leaderboard =
    {
        {"Alice",100},
        {"Bob",85},
        {"Carol",60},
        {"Dave",40},
        {"Player",0}
    };
    void SortLeaderboard(std::vector<Record>& records)
    {
        for (size_t i = 0; i < records.size(); ++i)
        {
            for (size_t j = i + 1; j < records.size(); ++j)
            {
                if (records[i].score < records[j].score)
                {
                    std::swap(records[i], records[j]);
                }
            }
        }
    }
}