#pragma once

class RankData
{
public:
    string initial;
    int score;
};

class RankingManager : public Singleton<RankingManager>
{
    vector<RankData> ranking;
    int letterCount = 0;
    int curIndex = 0;
    int prevIndex = 0;
public:
    void Save();
    void Load();
    void AddRanking(RankData rd);
    RankData GetRankData(int rank);
    bool EditInitial();
    void SortRanking();
    void Reset();
};

