#pragma once
#include"../Scene/SceneBase.h"
#include<Dxlib.h>
class JumpScare:public SceneBase
{
public:
    JumpScare();
    ~JumpScare();
    void Load(void) override;
    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;
private:
    int image_;
    int black_;
    int count_;
    int shakeDuration_; // 揺れの残りフレーム数
    int shakeMagnitude_; // 揺れの最大幅（ピクセル）
    int shakeOffsetX_;   // X方向のオフセット
    int shakeOffsetY_;   // Y方向のオフセット
};

