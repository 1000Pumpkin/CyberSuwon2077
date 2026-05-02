#include "pch.h"

const string Enemy::enemyTypeStrs[Count] = {
    "type1", "type2", "mid1", "boss1", 
    "type3", "type4", "mid2", "boss2", 
    "buoy"
};

const bool Enemy::enemyTypeIsBoss[Count] = {
    false, false, true, true,
    false, false, true, true,
    false
};

const bool Enemy::enemyTypeIsStageClear[Count] = {
    false, false, false, true,
    false, false, false, true,
    false
};

const bool Enemy::enemyTypeIsMoveEffect[Count] = {
    true, true, false, false,
    true, true, false, false,
    false
};

Enemy::Enemy(EnemyInfo info)
{
    this->info = info;
}

void Enemy::Init()
{
    sprite = Sprite("Assets/enemy/" + enemyTypeStrs[info.type] + "/*.png");
    collisionLayer = 2;
    sprite.rotate = info.rotate;
    layer = 5;
    if (info.pathes.size() > 0)
        realPosition = info.pathes[0].pos;

    switch (info.type)
    {
    case buoy:
    {
        isImmotal = true;
        CLine line;
        line.vP1 = D3DXVECTOR2(ANGLETOX(info.rotate), ANGLETOY(info.rotate)) * sprite.GetTextureWidth() / 2;
        line.vP2 = D3DXVECTOR2(ANGLETOX(info.rotate), ANGLETOY(info.rotate)) * -sprite.GetTextureWidth() / 2;
        SetLine(line);
    }break;
    case s1_mid:
    {
        PlayerManager::GetInstance()->isBossAppear = true;
        FRECT rc = { -125, 15, -20, -80 };
        SetMask(rc);
        rc = { -62, 80, 125, -40 };
        SetMask(rc);
    }break;
    case s1_boss:
    {
        PlayerManager::GetInstance()->isBossAppear = true;
        FRECT rc = { -297, 37, 297, -44 };
        SetMask(rc);
        rc = { -127, 80, 297, 37 };
        SetMask(rc);
        rc = { 23, 103, 297, 80 };
        SetMask(rc);
        rc = { 161, 0, 297, -103 };
        SetMask(rc);
    }break;
    case s2_mid:
    {
        PlayerManager::GetInstance()->isBossAppear = true;
        FRECT rc = { -218, 33, -58, -88 };
        SetMask(rc);
        rc = { -58, 63, 218, -88 };
        SetMask(rc);
    }break;
    case s2_boss:
    {
        PlayerManager::GetInstance()->isBossAppear = true;
        FRECT rc = { -306, 46, 144, -83 };
        SetMask(rc);
        rc = { 54, 83, 306, -13 };
        SetMask(rc);
    }break;
    default:
    {
        SetMask(sprite);
    }break;
    }

    Character::Init();

    statusInfo.exp = 100;
    statusInfo.attackSpeed = 300;
    statusInfo.moveSpeed = 300.f;

    switch (info.type)
    {
    case s1_t1:
    {
        statusInfo.hp = 50;
        statusInfo.attack = 10;
    }break;
    case s1_t2:
    {
        statusInfo.hp = 30;
        statusInfo.attack = 10;
        statusInfo.moveSpeed = 400.f;
    }break;
    case s1_mid:
    {
        statusInfo.hp = 3500;
        statusInfo.attack = 20;
    }break;
    case s1_boss:
    {
        statusInfo.hp = 6500;
        statusInfo.attack = 30;
    }break;
    case s2_t1:
    {
        statusInfo.hp = 100;
        statusInfo.attack = 20;
    }break;
    case s2_t2:
    {
        statusInfo.hp = 60;
        statusInfo.attack = 20;
        statusInfo.moveSpeed = 400.f;
    }break;
    case s2_mid:
    {
        statusInfo.hp = 6000;
        statusInfo.attack = 40;
    }break;
    case s2_boss:
    {
        statusInfo.hp = 10000;
        statusInfo.attack = 50;
    }break;
    case buoy:
    {
        statusInfo.hp = 50;
        statusInfo.attack = 50;
    }break;
    }
    status = Status(statusInfo, 1);
}

void Enemy::Term()
{
}

void Enemy::Update(float deltaTime)
{
    if (isDead)
    {
        if (enemyTypeIsBoss[info.type])
            PlayerManager::GetInstance()->isBossAppear = false;
        deadDeltaTime += deltaTime;
        moveDir.y -= 10.f * deltaTime;
        D3DXVec2Normalize(&moveDir, &moveDir);

        if (deadDeltaTime >= 2.f)
        {
            for (int i = 0; i < 3; ++i)
            {
                EffectInfo effInfo;
                effInfo.aniTime = 0.01f;
                effInfo.duration = 0.f;
                effInfo.name = "explode3";
                effInfo.pos = position + D3DXVECTOR2(
                    Random::GetInstance()->rand(-sprite.GetTextureWidth() / 2, sprite.GetTextureWidth() / 2), 
                    Random::GetInstance()->rand(-sprite.GetTextureHeight() / 2, sprite.GetTextureHeight() / 2));
                effInfo.rotate = Random::GetInstance()->rand(0, 360);

                SceneManager::GetInstance()->AddObject(new Effect(effInfo));
            }

            if (Random::GetInstance()->rand(0, 15) == 0)
            {
                ItemInfo iInfo;
                if (PlayerManager::GetInstance()->g_pPlayer->status.level < Status::maxLevel)
                    iInfo.type = (ItemType)Random::GetInstance()->rand(0, 2);
                else
                    iInfo.type = (ItemType)Random::GetInstance()->rand(0, 1);
                iInfo.pos = position;
                SceneManager::GetInstance()->AddObject(new Item(iInfo));
            }
            isRemove = true;
        }
        else
        {
            static float effIntervalTime = 0.1f;

            deadEffDeltaTime += deltaTime;
            if (deadEffDeltaTime >= effIntervalTime)
            {
                EffectInfo effInfo;
                effInfo.aniTime = 0.f;
                effInfo.duration = 0.f;
                effInfo.name = "explode1";
                effInfo.pos = position + D3DXVECTOR2(Random::GetInstance()->rand(-10, 10), Random::GetInstance()->rand(-10, 10));
                effInfo.rotate = Random::GetInstance()->rand(0, 360);

                SceneManager::GetInstance()->AddObject(new Effect(effInfo));

                deadEffDeltaTime = 0.f;
            }
            r = g = b = 0.3f;
            sprite.rotate = 45.f;
            position += moveDir * 100.f * deltaTime;
        }
        return;
    }

    if (enemyTypeIsBoss[info.type])
    {
        PlayerManager::GetInstance()->bossHpRate = (float)status.hp / (float)status.GetMaxHp(status.level);
    }

    moveEffTime -= deltaTime;
    if (moveEffTime <= 0.f && enemyTypeIsMoveEffect[info.type])
    {
        EffectInfo effInfo;
        effInfo.aniTime = 0.f;
        effInfo.duration = 1.f;
        effInfo.name = "enemymove";
        effInfo.pos = position + D3DXVECTOR2(30, 0);
        effInfo.rotate = 0.f;
        moveEffTime = 0.3f;
    }

    if (phase == 0)
    {
        if (info.pathes.size() < 2)
        {
            moveDir = D3DXVECTOR2(-1, 0);
        }
        else
        {
            auto& curPath = info.pathes[pathIndex];
            auto& prevPath = info.pathes[pathIndex - 1];
            auto diffPos = curPath.pos - position;
            if (D3DXVec2Length(&diffPos) < 20.f)
            {
                if (++pathIndex >= info.pathes.size())
                {
                    pathIndex = info.pathes.size() - 1;
                    if (enemyTypeIsBoss[info.type])
                    {
                        phase = 1;
                    }
                }
                else
                {
                    if (curPath.isAttack)
                    {
                        switch (info.type)
                        {
                        case s1_t1: case s2_t1:
                        {
                            BulletInfo bInfo;
                            bInfo.boomEffName = "hit";
                            bInfo.caster = this;
                            bInfo.collisionLayer = 2;
                            bInfo.damage = status.attack;
                            bInfo.moveDir = D3DXVECTOR2(-1, 0);
                            bInfo.moveSpeed = 500.f;
                            bInfo.name = "enemy2";
                            bInfo.pos = position + bInfo.moveDir * 50.f;
                            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
                        }break;
                        case s1_t2: case s2_t2: 
                        {
                            BulletInfo bInfo;
                            bInfo.boomEffName = "hit";
                            bInfo.caster = this;
                            bInfo.collisionLayer = 2;
                            bInfo.damage = status.attack;
                            bInfo.pos = position + bInfo.moveDir * 50.f;
                            D3DXVECTOR2 dir = PlayerManager::GetInstance()->g_pPlayer->position - bInfo.pos;
                            D3DXVec2Normalize(&dir, &dir);
                            bInfo.moveDir = dir;
                            bInfo.moveSpeed = 500.f;
                            bInfo.name = "enemy2";
                            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
                        }break;
                        }
                    }
                }
            }
            auto dir = curPath.pos - prevPath.pos;
            D3DXVec2Normalize(&dir, &dir);
            moveDir = dir;
        }
    }
    else
    {
        moveDir = D3DXVECTOR2(0, 0);
        switch (info.type)
        {
        case s1_mid:
        {
            if (phase == 1)
            {
                bossLevitation += deltaTime;
            }
            attackDeltaTime -= deltaTime;
            if (attackDeltaTime <= 0.f)
            {
                if (phase == 1)
                {
                    attackIntervalTime = 0.f;
                    phase = 2;
                    attackType = Random::GetInstance()->rand(0, 1);
                    attackDeltaTime = 1.f;
                }
                else if (phase == 2)
                {
                    if (attackType == 0)
                    {
                        attackIntervalTime -= deltaTime;
                        if (attackIntervalTime <= 0.f)
                        {
                            BulletInfo bInfo;
                            bInfo.boomEffName = "hit";
                            bInfo.caster = this;
                            bInfo.collisionLayer = 2;
                            bInfo.damage = status.attack;
                            D3DXVECTOR2 dir = (PlayerManager::GetInstance()->g_pPlayer->position +
                                D3DXVECTOR2(0, Random::GetInstance()->rand(-50, 50))) - position;
                            D3DXVec2Normalize(&dir, &dir);
                            bInfo.moveDir = dir;
                            bInfo.moveSpeed = 500.f;
                            bInfo.name = "enemy2";
                            bInfo.pos = position + bInfo.moveDir * 70.f;

                            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));

                            bulletCount++;
                            attackIntervalTime = 0.1f;
                        }
                        if (bulletCount > 8)
                        {
                            phase = 1;
                            attackDeltaTime = 1.f;
                            bulletCount = 0;
                        }
                    }
                    else
                    {
                        attackIntervalTime -= deltaTime;
                        if (attackIntervalTime <= 0.f)
                        {
                            for (int i = 0; i < 5 + (bulletCount % 2); ++i)
                            {
                                float rotate = 0.f;
                                if (bulletCount % 2 == 0)
                                    rotate = -160 - 10 * i;
                                else
                                    rotate = -155 - 10 * i;
                                BulletInfo bInfo;
                                bInfo.boomEffName = "hit";
                                bInfo.caster = this;
                                bInfo.collisionLayer = 2;
                                bInfo.damage = status.attack;
                                D3DXVECTOR2 dir = D3DXVECTOR2(ANGLETOX(rotate), ANGLETOY(rotate));
                                D3DXVec2Normalize(&dir, &dir);
                                bInfo.moveDir = dir;
                                bInfo.moveSpeed = 500.f;
                                bInfo.name = "enemy2";
                                bInfo.pos = position + bInfo.moveDir * 70.f;
                                SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
                            }
                            bulletCount++;
                            attackIntervalTime = 0.5f;
                        }
                        if (bulletCount > 2)
                        {
                            bulletCount = 0;
                            phase = 1;
                            attackDeltaTime = 1.f;
                        }
                    }
                }
            }
        }break;
        case s1_boss:
        {
            if (phase == 1)
            {
                bossLevitation += deltaTime;
            }
            attackDeltaTime -= deltaTime;
            if (attackDeltaTime <= 0.f)
            {
                if (phase == 1)
                {
                    attackIntervalTime = 0.f;
                    phase = 2;
                    attackType = Random::GetInstance()->rand(0, 1);
                    attackDeltaTime = 1.f;
                }
                else if (phase == 2)
                {
                    if (attackType == 0)
                    {
                        attackIntervalTime -= deltaTime;
                        bossLevitation += deltaTime;
                        if (attackIntervalTime <= 0.f)
                        {
                            BulletInfo bInfo;
                            bInfo.boomEffName = "hit";
                            bInfo.caster = this;
                            bInfo.collisionLayer = 2;
                            bInfo.damage = status.attack;
                            bInfo.pos = position + D3DXVECTOR2(0, 100);
                            D3DXVECTOR2 dir = (PlayerManager::GetInstance()->g_pPlayer->position +
                                D3DXVECTOR2(0, Random::GetInstance()->rand(-50, 50))) - bInfo.pos;
                            D3DXVec2Normalize(&dir, &dir);
                            bInfo.moveDir = dir;
                            bInfo.moveSpeed = 500.f;
                            bInfo.name = "enemy1";

                            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));

                            bulletCount++;
                            attackIntervalTime = 0.1f;
                        }
                        if (bulletCount > 20)
                        {
                            phase = 1;
                            attackDeltaTime = 1.f;
                            bulletCount = 0;
                        }
                    }
                    else
                    {
                        attackIntervalTime -= deltaTime;
                        if (attackIntervalTime <= 0.f)
                        {
                            for (int i = 0; i < 8; ++i)
                            {
                                int rotate = -160 - Random::GetInstance()->rand(0, 40);
                                BulletInfo bInfo;
                                bInfo.boomEffName = "hit";
                                bInfo.caster = this;
                                bInfo.collisionLayer = 2;
                                bInfo.damage = status.attack;
                                D3DXVECTOR2 dir = D3DXVECTOR2(ANGLETOX(rotate), ANGLETOY(rotate));
                                D3DXVec2Normalize(&dir, &dir);
                                bInfo.moveDir = dir;
                                bInfo.moveSpeed = Random::GetInstance()->rand(300, 600);
                                bInfo.name = "enemy2";
                                bInfo.pos = position + D3DXVECTOR2(0, 100);
                                SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
                            }
                            bulletCount++;
                            attackIntervalTime = 0.3f;
                        }
                        if (bulletCount > 6)
                        {
                            bulletCount = 0;
                            phase = 1;
                            attackDeltaTime = 1.f;
                        }
                    }
                }
            }
        }break;
        case s2_mid:
        {
            if (phase == 1)
            {
                bossLevitation += deltaTime;
            }
            attackDeltaTime -= deltaTime;
            if (attackDeltaTime <= 0.f)
            {
                if (phase == 1)
                {
                    attackIntervalTime = 0.f;
                    phase = 2;
                    attackType = Random::GetInstance()->rand(0, 1);
                    attackDeltaTime = 1.f;
                }
                else if (phase == 2)
                {
                    if (attackType == 0)
                    {
                        attackIntervalTime -= deltaTime;
                        bossLevitation += deltaTime;
                        if (attackIntervalTime <= 0.f)
                        {
                            BulletInfo bInfo;
                            bInfo.boomEffName = "hit";
                            bInfo.caster = this;
                            bInfo.collisionLayer = 2;
                            bInfo.damage = status.attack;
                            bInfo.pos = position + D3DXVECTOR2(-sprite.GetTextureWidth() / 2, -80);
                            D3DXVECTOR2 dir = (PlayerManager::GetInstance()->g_pPlayer->position +
                                D3DXVECTOR2(0, Random::GetInstance()->rand(-80, 80))) - bInfo.pos;
                            D3DXVec2Normalize(&dir, &dir);
                            bInfo.moveDir = dir;
                            bInfo.moveSpeed = 500.f;
                            bInfo.name = "enemy1";

                            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));

                            bulletCount++;
                            attackIntervalTime = 0.1f;
                        }
                        if (bulletCount >= 12)
                        {
                            phase = 1;
                            attackDeltaTime = 1.f;
                            bulletCount = 0;
                        }
                    }
                    else
                    {
                        attackIntervalTime -= deltaTime;
                        if (attackIntervalTime <= 0.f)
                        {
                            int rotate = -160 - Random::GetInstance()->rand(0, 40);
                            BulletInfo bInfo;
                            bInfo.boomEffName = "hit";
                            bInfo.caster = this;
                            bInfo.collisionLayer = 2;
                            bInfo.damage = status.attack;
                            D3DXVECTOR2 dir = D3DXVECTOR2(ANGLETOX(rotate), ANGLETOY(rotate));
                            D3DXVec2Normalize(&dir, &dir);
                            bInfo.moveDir = dir;
                            bInfo.moveSpeed = 150.f;
                            bInfo.name = "enemy2";
                            bInfo.pos = position + D3DXVECTOR2(-sprite.GetTextureWidth() / 2, -80);
                            SceneManager::GetInstance()->AddObject(new SplitBullet(bInfo));
                            bulletCount++;
                            attackIntervalTime = 0.5f;
                        }
                        if (bulletCount >= 3)
                        {
                            bulletCount = 0;
                            phase = 1;
                            attackDeltaTime = 1.f;
                        }
                    }
                }
            }
        }break;
        case s2_boss:
        {
            if (phase == 1)
            {
                bossLevitation += deltaTime;
            }
            attackDeltaTime -= deltaTime;
            if (attackDeltaTime <= 0.f)
            {
                if (phase == 1)
                {
                    attackIntervalTime = 0.f;
                    phase = 2;
                    attackType = Random::GetInstance()->rand(0, 2);
                }
                else if (phase == 2)
                {
                    if (attackType == 0)
                    {
                        PlayerManager::GetInstance()->laserSound->Play(false);
                        BulletInfo bInfo;
                        bInfo.boomEffName = "hit";
                        bInfo.caster = this;
                        bInfo.collisionLayer = 0;
                        bInfo.damage = status.attack;
                        bInfo.moveDir = D3DXVECTOR2(0, 0);
                        bInfo.moveSpeed = 0.f;
                        bInfo.name = "laser";
                        bInfo.pos = position + D3DXVECTOR2(-800, -20);
                        SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
                        phase = 3;
                        attackDeltaTime = 1.f;
                    }
                    else if (attackType == 1)
                    {
                        attackIntervalTime -= deltaTime;
                        bossLevitation += deltaTime;
                        if (attackIntervalTime <= 0.f)
                        {
                            int rotate = -160 - Random::GetInstance()->rand(0, 40);
                            BulletInfo bInfo;
                            bInfo.boomEffName = "hit";
                            bInfo.caster = this;
                            bInfo.collisionLayer = 2;
                            bInfo.damage = status.attack;
                            D3DXVECTOR2 dir = D3DXVECTOR2(ANGLETOX(rotate), ANGLETOY(rotate));
                            D3DXVec2Normalize(&dir, &dir);
                            bInfo.moveDir = dir;
                            bInfo.moveSpeed = 150.f;
                            bInfo.name = "enemy2";
                            bInfo.pos = position + D3DXVECTOR2(-sprite.GetTextureWidth() / 2, -80);
                            SceneManager::GetInstance()->AddObject(new SplitBullet(bInfo));
                            bulletCount++;
                            attackIntervalTime = 0.3f;
                        }
                        if (bulletCount >= 4)
                        {
                            bulletCount = 0;
                            phase = 1;
                            attackDeltaTime = 3.f;
                        }
                    }
                    else
                    {
                        attackIntervalTime -= deltaTime;
                        if (attackIntervalTime <= 0.f)
                        {
                            for (int i = 0; i < 7 + (bulletCount % 2); ++i)
                            {
                                float rotate = 0.f;
                                if (bulletCount % 2 == 0)
                                    rotate = -156.f - 8.f * (float)i;
                                else
                                    rotate = -152.f - 8.f * (float)i;
                                BulletInfo bInfo;
                                bInfo.boomEffName = "hit";
                                bInfo.caster = this;
                                bInfo.collisionLayer = 2;
                                bInfo.damage = status.attack;
                                D3DXVECTOR2 dir = D3DXVECTOR2(ANGLETOX(rotate), ANGLETOY(rotate));
                                D3DXVec2Normalize(&dir, &dir);
                                bInfo.moveDir = dir;
                                bInfo.moveSpeed = 500.f;
                                bInfo.name = "enemy2";
                                bInfo.pos = position + D3DXVECTOR2(130, 40);
                                SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
                            }
                            bulletCount++;
                            attackIntervalTime = 0.3f;
                        }
                        if (bulletCount > 6)
                        {
                            bulletCount = 0;
                            phase = 1;
                            attackDeltaTime = 3.f;
                        }
                    }
                }
                else if (phase == 3)
                {
                    attackIntervalTime += deltaTime;
                    if (attackIntervalTime >= 2.f)
                    {
                        PlayerManager::GetInstance()->laserSound->Stop();
                        phase = 1;
                        attackDeltaTime = 1.f;
                    }
                }
            }
        }break;
        }
    }

    Character::Update(deltaTime);
    
    if (enemyTypeIsBoss[info.type])
    {
        position = realPosition + D3DXVECTOR2(0, sinf(bossLevitation) * 100.f);
    }
    else if (enemyTypeIsMoveEffect[info.type])
    {
        position = realPosition + D3DXVECTOR2(0, sinf(levitation) * 10.f);
    }
}

void Enemy::OnCollision(Object* another, float deltaTime)
{
    if (another->tag == "Character")
    {
        auto character = static_cast<Character*>(another);
        if (character->isHit || isDead)
            return;

        character->OnHit(status.attack);

        OnHit(character->status.attack);
    }
}

void Enemy::LateUpdate(float deltaTime)
{
    if (realPosition.x < -500 ||
        realPosition.x > 1780 ||
        realPosition.y < -500 ||
        realPosition.y > 1220)
        isRemove = true;
}

void Enemy::Render()
{
    sprite.r = r;
    sprite.g = g;
    sprite.b = b;
    sprite.pos = position;
    sprite.Render();
#ifdef _DEBUG
    Object::Render();
#endif // _DEBUG
}

void Enemy::OnHit(int damage)
{
    Character::OnHit(damage);

    if (status.hp <= 0)
    {
        static int expTable[Count] = { 10, 5, 100, 200, 20, 10, 200, 400, 0 };
        PlayerManager::GetInstance()->g_pPlayer->status.exp += expTable[info.type];
        PlayerManager::GetInstance()->score += expTable[info.type] * 100;
    }

    PlayerManager::GetInstance()->score += 15;
}
