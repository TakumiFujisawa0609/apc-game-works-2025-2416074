#include "JumpScare.h"
#include"../Manager/SceneManager.h"
#include"../Manager/SoundManager.h"
#include<Dxlib.h>
JumpScare::JumpScare()
{
}

JumpScare::~JumpScare()
{
}

void JumpScare::Load(void)
{
    image_ = LoadGraph("Data/Image/Scenes/GameOver.ong.png");
    black_ = LoadGraph("Data/UI/Scenes/Black.png");
    count_ = 0;
    shakeDuration_ = 0;
	shakeMagnitude_ = 0;
    shakeOffsetX_ = 0;
    shakeOffsetY_ = 0;
    
}

void JumpScare::Init(void)
{
	
}

void JumpScare::Update(void)
{
	count_++;
    //最初の一度だけ再生
    auto& sm = SoundManager::GetInstance();

    sm.Play(SoundManager::SRC::GAMEOVER, Sound::TIMES::ONCE);

    // 揺れをトリガーする（例：Updateに入った瞬間、または特定のカウント）
    if (count_ == 1) // 最初のフレームで揺れを開始
    {
        shakeDuration_ = 30; // 60フレーム（約1秒）揺らす
        shakeMagnitude_ = 20; // 最大5ピクセルずらす
    }

    // 揺れの残り時間があれば減らす
    if (shakeDuration_ > 0)
    {
        shakeOffsetX_ = GetRand(shakeMagnitude_ * 2) - shakeMagnitude_;
        shakeOffsetY_ = GetRand(shakeMagnitude_ * 2) - shakeMagnitude_;
        shakeDuration_--;

        if (shakeDuration_ <= 0)
        {
            shakeOffsetX_ = 0;
            shakeOffsetY_ = 0;
        }
    }

	if (count_ == 30)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::OVER);
	}
}

void JumpScare::Draw(void)
{
    int offsetX = shakeOffsetX_;
    int offsetY = shakeOffsetY_;


    if (shakeDuration_ > 0)
    {
        int max_rand = 2 * shakeMagnitude_;

        offsetX = GetRand(max_rand) - shakeMagnitude_;
        offsetY = GetRand(max_rand) - shakeMagnitude_;
    }

    // 計算したオフセットを適用して描画
    // DrawGraph(X座標 + offsetX, Y座標 + offsetY, グラフィックハンドル, 透過フラグ)
    DrawGraph(0.0, 0.0, black_, true);

    DrawGraph(0 + offsetX, 0 + offsetY, LoadGraph("Data/Image/Scenes/GameOver.ong.png"), FALSE);

   // DrawFormatString(0, 0, 0xFFFFFF, "count_%d",count_);

}

void JumpScare::Release(void)
{
}
