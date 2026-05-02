#include "pch.h"

void GameScene::Init()
{
    PlayerManager::GetInstance()->titleBgm->Stop();
    PlayerManager::GetInstance()->gameBgm->Play(true);
    if (stage == 0)
        PlayerManager::GetInstance()->Reset();
    PlayerManager::GetInstance()->g_pPlayer = new Player;
    AddObject(PlayerManager::GetInstance()->g_pPlayer);

    clearSprite = Sprite("Assets/ui/clear/" + to_string(stage) + ".png");
    pauseSprite = Sprite("Assets/ui/pause/*.png");
    pauseSprite.pos = Direct3D::GetInstance()->camPos;
    pauseSprite.a = 0.5f;

    MapLayerInfo mInfo;
    mInfo.gameTime = &gameTime;
    mInfo.layer = -3;
    char buffer[255];
    sprintf(buffer, "Assets/map/sky/%d/*.png", stage);
    mInfo.mapPath = buffer;
    mInfo.offset = D3DXVECTOR2(0, -100);
    mInfo.scrollSpeed = 0.05f;
    AddObject(new MapLayer(mInfo));
    mInfo.layer = -2;
    sprintf(buffer, "Assets/map/far/%d/*.png", stage);
    mInfo.mapPath = buffer;
    mInfo.scrollSpeed = 0.2f;
    AddObject(new MapLayer(mInfo));
    mInfo.layer = -1;
    sprintf(buffer, "Assets/map/near/%d/*.png", stage);
    mInfo.mapPath = buffer;
    mInfo.offset = D3DXVECTOR2(0, -500);
    mInfo.scrollSpeed = 0.4f;
    AddObject(new MapLayer(mInfo));
    g_pUI = new UI;
    AddObject(g_pUI);

    sprintf(buffer, "stage%d.txt", stage);
    ifstream is;
    is.open(buffer);

    float x;
    is >> x;
    int spawnCount;
    is >> spawnCount;
    for (int i = 0; i < spawnCount; ++i)
    {
        spawnPlan.push_back(new EnemySpawnObject(is));
    }

    is.close();

    g_pUI->AppearMessage(UI::MessageType((int)UI::MessageType::stg1_start + stage));
}

void GameScene::Term()
{
    gameTime = 0.f;
    stageClear = false;
    PlayerManager::GetInstance()->playerStatus = PlayerManager::GetInstance()->g_pPlayer->status;
    clearTime = 0.f;
    spawnPlan.clear();
    bossEnemies.clear();
    timer = 0.f;
}

void GameScene::Update(float deltaTime)
{
    timer += deltaTime;
    if (stageClear)
    {
        auto& player = PlayerManager::GetInstance()->g_pPlayer;
        PlayerManager::GetInstance()->remainingHpPercent = (float)player->status.hp / (float)player->status.GetMaxHp(player->status.level);
        clearTime += deltaTime;
        if (clearTime >= 3.f)
        {
            PlayerManager::GetInstance()->score += hpScore;
            if (stage == 0)
            {
                player->status.hp = player->status.GetMaxHp(player->status.level);
                SceneManager::GetInstance()->ChangeScene("Stage1");
            }
            else
            {
                PlayerManager::GetInstance()->isClear = true;
                SceneManager::GetInstance()->ChangeScene("Result");
            }
        }
    }

    for (auto iter = bossEnemies.begin(); iter != bossEnemies.end();)
    {
        if ((*iter)->isDead)
        {
            if (Enemy::enemyTypeIsStageClear[(*iter)->info.type])
            {
                stageClear = true;
            }
            iter = bossEnemies.erase(iter);
        }
        else
            ++iter;
    }

    if (bossEnemies.size() > 0)
    {
        gameTimeStop -= deltaTime;
        if (gameTimeStop <= 0.f)
            gameTimeStop = 0.f;
    }
    else
    {
        gameTimeStop += deltaTime;
        if (gameTimeStop >= 1.f)
            gameTimeStop = 1.f;
    }

    gameTime += deltaTime * gameTimeStop;

    for (auto iter = spawnPlan.begin(); iter != spawnPlan.end();)
    {
        if ((*iter)->info.spawnTime <= gameTime)
        {
            EnemyInfo eInfo;
            eInfo.pathes = (*iter)->info.pathes;
            eInfo.rotate = (*iter)->info.rotate;
            eInfo.type = (*iter)->info.type;
            Enemy* enemy = new Enemy(eInfo);
            if (Enemy::enemyTypeIsBoss[enemy->info.type])
                bossEnemies.push_back(enemy);

            AddObject(enemy);
            delete (*iter);
            iter = spawnPlan.erase(iter);
        }
        else
            ++iter;
    }

    if (KeyManager::GetInstance()->GetKeyState(VK_F3) == EKEY_PRESS)
    {
        ItemInfo iInfo;
        if (PlayerManager::GetInstance()->g_pPlayer->status.level < Status::maxLevel)
            iInfo.type = (ItemType)Random::GetInstance()->rand(0, 2);
        else
            iInfo.type = (ItemType)Random::GetInstance()->rand(0, 1);
        iInfo.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        AddObject(new Item(iInfo));
    }

    if (KeyManager::GetInstance()->GetKeyState(VK_F7) == EKEY_PRESS)
    {
        isF7 = !isF7;
    }

    if (KeyManager::GetInstance()->GetKeyState(VK_F8) == EKEY_PRESS)
    {
        PlayerManager::GetInstance()->isPause = !PlayerManager::GetInstance()->isPause;
    }
    if (KeyManager::GetInstance()->GetKeyState(VK_F9) == EKEY_PRESS)
    {
        if (isF9)
        {
            isF9 = false;
            PlayerManager::GetInstance()->gameBgm->Play(true);
        }
        else
        {
            isF9 = true;
            PlayerManager::GetInstance()->gameBgm->Stop();
        }
    }
    if (PlayerManager::GetInstance()->isPause)
        Time::GetInstance()->timeScale = 0;
    else
        Time::GetInstance()->timeScale = 1;
    Scene::Update(deltaTime);
}

void GameScene::Render()
{
    Scene::Render();
    if (stageClear)
    {
        hpScore = 100000 * PlayerManager::GetInstance()->remainingHpPercent;
        clearSprite.pos = Direct3D::GetInstance()->camPos;
        clearSprite.Render();
        char buffer[255];
        sprintf(buffer, "%08d + %d", PlayerManager::GetInstance()->score, hpScore);
        RECT rc = { 0, 360, 1280, 500 };
        Direct3D::GetInstance()->pFont->DrawTextA(NULL, buffer, -1, &rc, DT_CENTER | DT_VCENTER, 0xffffffff);
    }

    if (isF7)
    {
        auto player = PlayerManager::GetInstance()->g_pPlayer;
        int totalExp = player->status.exp;
        for (int i = 1; i < player->status.level; ++i)
        {
            totalExp += player->status.GetMaxExp(i);
        }
        char buffer[255];
        sprintf(buffer, "Level : %d\nTotalExp : %d\nLv1 -> Lv2 NeedExp : %d\nLv2 -> Lv3 NeedExp : %d\nLv3 -> Lv4 NeedExp : %d\nLv4 -> Lv5 NeedExp : %d",
            player->status.level,
            totalExp,
            player->status.GetMaxExp(1),
            player->status.GetMaxExp(2),
            player->status.GetMaxExp(3),
            player->status.GetMaxExp(4));
        RECT rc = { 0, 0, 1280, 720 };
        Direct3D::GetInstance()->pFont->DrawTextA(NULL, buffer, -1, &rc, DT_CENTER | DT_VCENTER, 0xffffffff);
    }
    /*char buffer[255];
    sprintf(buffer, "%d", (int)timer);
    RECT rc = { 1000, 0, 1200, 100 };
    Direct3D::GetInstance()->pFont->DrawTextA(NULL, buffer, -1, &rc, DT_RIGHT | DT_BOTTOM, 0xffffffff);*/
    if (PlayerManager::GetInstance()->isPause)
    {
        pauseSprite.Render();
    }
}
