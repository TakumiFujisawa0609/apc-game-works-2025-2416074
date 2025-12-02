#pragma once
#include "../Scene/SceneBase.h"
#include <DxLib.h>

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
    int loadingAngle_;   // ‰ñ“]Šp“x
    int timer_;

    bool cleared_;
    GameScene* gameScn_;
};
