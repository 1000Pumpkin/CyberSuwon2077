#include "pch.h"

EnemySpawnObject::EnemySpawnObject(EnemySpawnInfo info)
{
    this->info = info;
}

EnemySpawnObject::EnemySpawnObject(ifstream& is)
{
    is >> info.spawnTime;
    is >> (int&)info.type;
    is >> info.rotate;
    int pathCount;
    is >> pathCount;
    for (int i = 0; i < pathCount; ++i)
    {
        EnemyMovePath ep;
        is >> ep.isAttack;
        is >> ep.pos.x;
        is >> ep.pos.y;
        info.pathes.push_back(ep);
    }
}

void EnemySpawnObject::Write(ofstream& os)
{
    os << info.spawnTime << endl;
    os << info.type << endl;
    os << info.rotate << endl;
    os << info.pathes.size() << endl;
    for (auto iter : info.pathes)
    {
        os << iter.isAttack << endl;
        os << iter.pos.x << endl;
        os << iter.pos.y << endl;
    }
}

void EnemySpawnObject::Init()
{
    tag = "SpawnObject";
    EnemyInfo eInfo;
    eInfo.pathes = info.pathes;
    eInfo.rotate = info.rotate;
    eInfo.type = info.type;
    enemy = new Enemy(eInfo);
    SceneManager::GetInstance()->AddObject(enemy);
}

void EnemySpawnObject::Term()
{
    enemy->isRemove = true;
}

void EnemySpawnObject::Update(float deltaTime)
{
}

void EnemySpawnObject::OnCollision(Object* another, float deltaTime)
{
}

void EnemySpawnObject::LateUpdate(float deltaTime)
{
}

void EnemySpawnObject::Render()
{
    auto line = Direct3D::GetInstance()->pLine;
    vector<D3DXVECTOR3> lines;
    for (auto iter : info.pathes)
    {
        lines.push_back(D3DXVECTOR3(iter.pos.x, iter.pos.y, 0));
    }
    D3DXMATRIX lineMat = Direct3D::GetInstance()->viewMat * Direct3D::GetInstance()->projMat;
    line->Begin();
    line->DrawTransform(&lines[0], lines.size(), &lineMat, 0xffff0000);
    line->End();
}
