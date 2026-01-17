#pragma once
class Player : public Character
{
public:
    enum State : int
    {
        Idle,
        Left,
        Right,
        Up,
        Down,
        Dead,
        Count
    };

    enum ShotType : int
    {
        Direct,
        Radiation,
        ShotCount
    };

    Sprite sprite[Count];
    string stateStrs[Count] = { "idle", "left", "right", "up", "down", "dead" };
    State state = Idle;
    int shotType = Direct;
    float moveEffTime = 0.f;
    bool isAttack = false;
    float fireCoolTime = 0.f;
    bool noCollision = false;

    virtual void Init();
    virtual void Term();
    virtual void Update(float deltaTime);
    virtual void OnCollision(Object* another, float deltaTime);
    virtual void LateUpdate(float deltaTime);
    virtual void Render();
};

class PlayerManager : public Singleton<PlayerManager>
{
public:
    Player* g_pPlayer = nullptr;
    Status playerStatus;
    bool isBossAppear = false;
    float bossHpRate = 0.f;
    int score = 0;
    float remainingHpPercent = 0.f;
    bool isClear = false;
    bool isEnd = false;
    Sound* titleBgm = nullptr;
    Sound* gameBgm = nullptr;
    Sound* playerShotSound = nullptr;
    Sound* loseSound = nullptr;
    Sound* winSound = nullptr;
    Sound* laserSound = nullptr;
    Sound* skillSound = nullptr;
    Sound* hitSound = nullptr;
    bool isPause = false;

    void Reset()
    {
        g_pPlayer = nullptr;
        StatusInfo info;
        info.attack = 10;
        info.attackSpeed = 300;
        info.exp = 100;
        info.hp = 100;
        info.moveSpeed = 300.f;
        playerStatus = Status(info, 1);
        isBossAppear = false;
        bossHpRate = 0.f;
        score = 0;
        remainingHpPercent = 0.f;
        isClear = false;
        isEnd = false;
        isPause = false;
    }
    void Term()
    {
        delete titleBgm;
        delete gameBgm;
        delete playerShotSound;
        delete loseSound;
        delete winSound;
        delete laserSound;
        delete skillSound;
        delete hitSound;
    }
};