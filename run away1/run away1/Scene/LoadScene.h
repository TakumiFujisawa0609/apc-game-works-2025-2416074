#pragma once
#include "../Scene/SceneBase.h"
#include <DxLib.h>
#include<thread>
#include<atomic>

class GameClearScene;
class GameScene;
class LoadScene : public SceneBase
{
public:
    LoadScene();
    ~LoadScene();

    void Load(void) override;
    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;

private:
    int image_;
    int loadingAngle_;   // âÒì]äpìx
    int timer_;

   
    bool cleared_;

    GameScene* gameScn_ = nullptr;
	GameClearScene* gameClearScn_ = nullptr;
    bool loadComplete = false; 

    std::thread loadThread_;

    int loadedModelHandle_ = -1;

    std::atomic<bool> isLoadCompleted_ = false;

    float elapsedTime_ = 0.0f; // åoâﬂéûä‘åvë™óp
    const float MIN_LOAD_TIME_MS = 2000.0f; // ç≈í·2ïbä‘ï\é¶

};
