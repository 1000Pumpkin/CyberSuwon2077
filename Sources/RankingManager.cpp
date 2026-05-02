#include "pch.h"

void RankingManager::Save()
{
    SortRanking();

    ofstream os;
    os.open("rank.txt");

    os << ranking.size() << endl;
    for (auto iter : ranking)
    {
        os << iter.initial << endl;
        os << iter.score << endl;
    }

    os.close();
}

void RankingManager::Load()
{
    ifstream is;
    is.open("rank.txt");

    int rankingSize;
    is >> rankingSize;

    for (int i = 0; i < rankingSize; ++i)
    {
        RankData rd;
        is >> rd.initial;
        is >> rd.score;
        AddRanking(rd);
    }

    is.close();

    SortRanking();
}

void RankingManager::AddRanking(RankData rd)
{
    ranking.push_back(rd);
    SortRanking();
}

RankData RankingManager::GetRankData(int rank)
{
    RankData rd;
    if (rank >= ranking.size())
    {
        rd.initial = "###";
        rd.score = 0;
    }
    else
    {
        rd = ranking[rank];
    }
    return rd;
}

bool RankingManager::EditInitial()
{
    SortRanking();

    prevIndex = curIndex;

    for (; curIndex < ranking.size(); ++curIndex)
    {
        if (ranking[curIndex].initial.rfind("_") != string::npos)
            break;
    }

    if (curIndex > 3)
        return true;

    if (curIndex >= ranking.size())
        curIndex = prevIndex;

    if (letterCount < 3)
    {
        for (int i = 'A'; i < 'Z'; ++i)
        {
            if (KeyManager::GetInstance()->GetKeyState(i) == EKEY_PRESS)
            {
                string str;
                str += i;
                ranking[curIndex].initial.replace(letterCount, 1, str);
                letterCount++;
                break;
            }
        }
    }

    if (letterCount > 0)
    {
        if (KeyManager::GetInstance()->GetKeyState(VK_BACK) == EKEY_PRESS)
        {
            string str = "_";
            letterCount--;
            ranking[curIndex].initial.replace(letterCount, 1, str);
        }
    }

    if (ranking[curIndex].initial.rfind("_") == string::npos)
        return true;

    return false;
}

void RankingManager::SortRanking()
{
    sort(ranking.begin(), ranking.end(), [](RankData p1, RankData p2)
        {
            return p1.score > p2.score;
        });

    if (ranking.size() > 3)
        ranking.pop_back();
}

void RankingManager::Reset()
{
    letterCount = 0;
    curIndex = 0;
    prevIndex = 0;
}
