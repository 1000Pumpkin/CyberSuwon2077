#include "pch.h"

void Player::Init()
{
    Character::Init();

    char buffer[255];
    for (int i = 0; i < Count; ++i)
    {
        sprintf(buffer, "Assets/player/%s/*.png", stateStrs[i].c_str());
        sprite[i] = Sprite(buffer);
    }
    collisionLayer = 1;
    SetMask(sprite[0]);
    collisionMasks[0].Resize(0.5f);

    CLine line;
    line.vP1 = D3DXVECTOR2(10, 10);
    line.vP2 = D3DXVECTOR2(-10, -10);
    SetLine(line);
    line.vP1 = D3DXVECTOR2(-10, 10);
    line.vP2 = D3DXVECTOR2(10, -10);
    SetLine(line);

    realPosition = D3DXVECTOR2(100, 360);

    status = PlayerManager::GetInstance()->playerStatus;
    status.hp = status.GetMaxHp(status.level);
    hitDuration = 0.5f;

    skills.push_back(new SkillShield(this));
    skills.push_back(new SkillReflect(this));
    skills.push_back(new SkillBomb(this));
    for (auto iter : skills)
        SceneManager::GetInstance()->AddObject(iter);
}

void Player::Term()
{
}

void Player::Update(float deltaTime)
{
    if (PlayerManager::GetInstance()->isPause)
        return;
    if (SceneManager::GetInstance()->GetSceneName() == "Editor")
        return;

    if (isDead)
    {
        state = Dead;
        static float effIntervalTime = 0.1f;
        static float effDeltaTime = effDeltaTime;

        effDeltaTime += deltaTime;
        if (effDeltaTime >= effIntervalTime)
        {
            EffectInfo effInfo;
            effInfo.aniTime = 0.f;
            effInfo.duration = 0.f;
            effInfo.name = "explode1";
            effInfo.pos = position + D3DXVECTOR2(Random::GetInstance()->rand(-10, 10), Random::GetInstance()->rand(-10, 10));
            effInfo.rotate = Random::GetInstance()->rand(0, 360);

            SceneManager::GetInstance()->AddObject(new Effect(effInfo));

            effDeltaTime = 0.f;
        }
        position.y -= 200.f * deltaTime;
        if (position.y <= 0.f)
        {
            SceneManager::GetInstance()->ChangeScene("Result");
        }
        return;
    }

    D3DXVECTOR2 moveDir = D3DXVECTOR2(0, 0);
    if (KeyManager::GetInstance()->GetKeyState(VK_LEFT) == EKEY_DOWN)
    {
        moveDir.x -= 1;
    }
    if (KeyManager::GetInstance()->GetKeyState(VK_RIGHT) == EKEY_DOWN)
    {
        moveDir.x += 1;
    }
    if (KeyManager::GetInstance()->GetKeyState(VK_DOWN) == EKEY_DOWN)
    {
        moveDir.y -= 1;
    }
    if (KeyManager::GetInstance()->GetKeyState(VK_UP) == EKEY_DOWN)
    {
        moveDir.y += 1;
    }
    if (KeyManager::GetInstance()->GetKeyState(VK_TAB) == EKEY_PRESS)
    {
        if (++shotType >= ShotCount)
            shotType = 0;
    }
    if (KeyManager::GetInstance()->GetKeyState(VK_SPACE) == EKEY_DOWN)
    {
        isAttack = true;
    }

    if (KeyManager::GetInstance()->GetKeyState(VK_F1) == EKEY_PRESS)
    {
        noCollision = !noCollision;
    }

    if (KeyManager::GetInstance()->GetKeyState(VK_F2) == EKEY_PRESS)
    {
        if(status.level < status.maxLevel)
            status.exp += status.GetMaxExp(status.level);
    }

    if (noCollision)
        collisionLayer = 0;
    else
        collisionLayer = 1;

    fireCoolTime -= deltaTime;

    if (isAttack && fireCoolTime <= 0.f)
    {
        PlayerManager::GetInstance()->playerShotSound->Play(false);
        isAttack = false;
        switch (shotType)
        {
        case Direct:
        {
            BulletInfo bInfo;
            bInfo.boomEffName = "hit";
            bInfo.caster = this;
            bInfo.collisionLayer = 1;
            bInfo.damage = status.attack;
            bInfo.moveDir = D3DXVECTOR2(1, 0);
            bInfo.moveSpeed = 500.f;
            bInfo.name = "direct";
            bInfo.pos = position + D3DXVECTOR2(30, 0);
            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
            fireCoolTime = 60.f / status.attackSpeed;
        }break;
        case Radiation:
        {
            BulletInfo bInfo;
            bInfo.boomEffName = "hit";
            bInfo.caster = this;
            bInfo.collisionLayer = 1;
            bInfo.damage = status.attack * 0.7f;
            bInfo.moveSpeed = 500.f;
            bInfo.name = "radiation";
            bInfo.pos = position + D3DXVECTOR2(30, 0);
            bInfo.moveDir = D3DXVECTOR2(1, 0);
            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
            D3DXVECTOR2 dir = D3DXVECTOR2(1, Random::GetInstance()->rand(-100, 100) / 500.f);
            D3DXVec2Normalize(&dir, &dir);
            bInfo.moveDir = dir;
            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
            dir = D3DXVECTOR2(1, Random::GetInstance()->rand(-100, 100) / 500.f);
            D3DXVec2Normalize(&dir, &dir);
            bInfo.moveDir = dir;
            SceneManager::GetInstance()->AddObject(new Bullet(bInfo));
            fireCoolTime = 60.f / status.attackSpeed * 2.5f;
        }break;
        }
    }

    moveEffTime -= deltaTime;
    if (moveEffTime <= 0.f && !PlayerManager::GetInstance()->isBossAppear)
    {
        EffectInfo effInfo;
        effInfo.aniTime = 0.f;
        effInfo.duration = 1.f;
        effInfo.name = "playermove";
        effInfo.pos = position + D3DXVECTOR2(-30, -20);
        effInfo.rotate = 0.f;
        SceneManager::GetInstance()->AddObject(new MoveEffect(effInfo));
        moveEffTime = 0.3f;
    }

    this->moveDir = moveDir;

    state = Idle;
    if (D3DXVec2Length(&moveDir) > 1.f)
        state = Idle;
    else if (moveDir.x < 0)
        state = Left;
    else if (moveDir.x > 0)
        state = Right;
    else if (moveDir.y > 0)
        state = Up;
    else if (moveDir.y < 0)
        state = Down;

    static char skillKeys[3] = { 'Q','W','E' };
    for (int i = 0; i < 3; ++i)
    {
        if (KeyManager::GetInstance()->GetKeyState(skillKeys[i]) == EKEY_PRESS)
        {
            if (!skills[i]->DoCast())
            {
                g_pUI->AppearMessage(UI::MessageType::cant_use_skill);
                PlayerManager::GetInstance()->skillSound->Play(false);
            }
        }
    }

    Character::Update(deltaTime);

    position = realPosition + D3DXVECTOR2(0, sinf(levitation) * 10.f);
}

void Player::OnCollision(Object* another, float deltaTime)
{
    if (another->tag == "Item")
    {
        another->isRemove = true;
        auto item = static_cast<Item*>(another);
        
        switch (item->info.type)
        {
        case ItemType::hp:
        {
            status.hp += status.GetMaxHp(status.level) * 0.2f;
            if (status.hp > status.GetMaxHp(status.level))
                status.hp = status.GetMaxHp(status.level);
            g_pUI->AppearMessage(UI::MessageType::item_get_hp);
        }break;
        case ItemType::resetcool:
        {
            for (auto& iter : skills)
            {
                iter->deltaTime = 0.f;
                iter->state = Skill::State::Ready;
            }
            g_pUI->AppearMessage(UI::MessageType::item_get_resetcool);
        }break;
        case ItemType::levelup:
        {
            if (status.level < Status::maxLevel)
            {
                status.exp += status.GetMaxExp(status.level);
            }
            g_pUI->AppearMessage(UI::MessageType::item_get_levelup);
        }break;
        }
    }
}

void Player::LateUpdate(float deltaTime)
{
    D3DXVECTOR2 imageSize = D3DXVECTOR2(sprite[state].GetTextureWidth() / 2, sprite[state].GetTextureHeight() / 2);
    if (realPosition.x < imageSize.x)
        realPosition.x = imageSize.x;
    if (realPosition.x > SCREEN_WIDTH - imageSize.x)
        realPosition.x = SCREEN_WIDTH - imageSize.x;
    if (realPosition.y < imageSize.y)
        realPosition.y = imageSize.y;
    if (realPosition.y > SCREEN_HEIGHT - imageSize.y)
        realPosition.y = SCREEN_HEIGHT - imageSize.y;
}

void Player::Render()
{
    sprite[state].r = r;
    sprite[state].g = g;
    sprite[state].b = b;
    sprite[state].pos = position;
    sprite[state].Render();
#ifdef _DEBUG
    Object::Render();
#endif // _DEBUG
}
