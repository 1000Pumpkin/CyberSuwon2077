#include "pch.h"

void LoadingScene::Init()
{
    curIndex = 0;
    assetFiles = GetAssetFiles("Assets");
    sprite = Sprite("Assets/loading/*.png");
    sprite.pos = D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    sprite.isLoop = true;
}

void LoadingScene::Term()
{
}

void LoadingScene::Update(float deltaTime)
{
    if (curIndex == 0)
    {
        PlayerManager::GetInstance()->titleBgm = new Sound("Assets/sound/titlebgm.wav", 1);
        PlayerManager::GetInstance()->gameBgm = new Sound("Assets/sound/gamebgm.mp3", 1);
        PlayerManager::GetInstance()->gameBgm->SetVolume(200);
        PlayerManager::GetInstance()->playerShotSound = new Sound("Assets/sound/shot.wav", 30);
        PlayerManager::GetInstance()->loseSound = new Sound("Assets/sound/youlose.wav", 1);
        PlayerManager::GetInstance()->winSound = new Sound("Assets/sound/youwin.wav", 1);
        PlayerManager::GetInstance()->laserSound = new Sound("Assets/sound/laser.wav", 1);
        PlayerManager::GetInstance()->skillSound = new Sound("Assets/sound/cantuse.wav", 10);
        PlayerManager::GetInstance()->hitSound = new Sound("Assets/sound/hit.mp3", 50);
        PlayerManager::GetInstance()->hitSound->SetVolume(1000);
    }
    sprite.Update(deltaTime);
    if (curIndex >= assetFiles.size())
    {
        SceneManager::GetInstance()->ChangeScene("Title");
        return;
    }

    TextureManager::GetInstance()->GetTexture(assetFiles[curIndex]);
    curIndex++;
}

void LoadingScene::Render()
{
    sprite.Render();
}

vector<string> LoadingScene::GetAssetFiles(string path)
{
    vector<string> result;

    char findPath[255];
    sprintf(findPath, "%s/*.*", path.c_str());

    _finddata_t fd;
    intptr_t handle;

    if ((handle = _findfirst(findPath, &fd)) != -1L)
    {
        do
        {
            char buffer[255];
            sprintf(buffer, "%s/%s", path.c_str(), fd.name);
            if (fd.attrib & _A_SUBDIR && fd.name[0] != '.')
            {
                vector<string> returnVector = GetAssetFiles(buffer);
                result.insert(result.end(), returnVector.begin(), returnVector.end());
            }
            else if (fd.size != 0 && fd.name[0] != '.')
            {
                result.push_back(buffer);
            }
        } while (_findnext(handle, &fd) == 0);
        _findclose(handle);
    }

    return result;
}
