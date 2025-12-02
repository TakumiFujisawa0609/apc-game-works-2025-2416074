#pragma once
#include<Dxlib.h>
#include"../Scene/SceneBase.h"
class GameOverScene :public SceneBase
{
public:
    GameOverScene();
    ~GameOverScene()override;
    void Load(void) override;
    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;
private:
    int image_;
    int text_anim_timer_; // アニメーションタイマー
    int text_;
    int counter_;
    const char* horror_message_; // ホラーメッセージ
    int message_max_width_; // メッセージの最大幅
    bool isText_;

    bool overText_;
};

