#pragma once
class Scene
{
protected:
    vector<Object*> objects;
    vector<Object*> addObjects;
public:
    void AddObject(Object* obj);

    virtual void Init() = 0;
    virtual void Term() = 0;
    virtual void Update(float deltaTime);
    virtual void CollisionUpdate(float deltaTime);
    virtual void LateUpdate(float deltaTime);
    virtual void Render();

    void ClearScene();
};

class SceneManager : public Singleton<SceneManager>
{
    Scene* curScene = nullptr;
    Scene* nextScene = nullptr;
    string sceneName = "";
    map<string, Scene*> sceneMap;
public:
    string GetSceneName()
    {
        return sceneName;
    }

    void AddObject(Object* obj)
    {
        if (curScene)
            curScene->AddObject(obj);
    }

    void AddScene(string name, Scene* scene)
    {
        sceneMap.insert(make_pair(name, scene));
    }

    void ChangeScene(string name)
    {
        auto sf = sceneMap.find(name);
        if (sf != sceneMap.end())
        {
            sceneName = name;
            nextScene = sf->second;
        }
    }

    void CheckChangeScene()
    {
        if (nextScene)
        {
            if (curScene)
            {
                curScene->Term();
                curScene->ClearScene();
            }
            curScene = nextScene;
            nextScene = nullptr;
            curScene->Init();
        }
    }

    void Update(float deltaTime)
    {
        if (curScene)
        {
            curScene->Update(deltaTime);
            curScene->CollisionUpdate(deltaTime);
            curScene->LateUpdate(deltaTime);
        }
        if (KeyManager::GetInstance()->GetKeyState(VK_F4) == EKEY_PRESS)
        {
            ChangeScene("Title");
        }
        if (KeyManager::GetInstance()->GetKeyState(VK_F5) == EKEY_PRESS)
        {
            ChangeScene("Stage0");
        }
        if (KeyManager::GetInstance()->GetKeyState(VK_F6) == EKEY_PRESS)
        {
            ChangeScene("Stage1");
        }
        if (KeyManager::GetInstance()->GetKeyState(VK_F10) == EKEY_PRESS)
        {
            ChangeScene("Editor");
        }
    }

    void Render()
    {
        if (curScene)
            curScene->Render();
    }
};