#pragma once
#include<Dxlib.h>
#include"../Scene/SceneBase.h"
class Caution :public SceneBase
{
public:
    Caution();
    ~Caution();
    void Load(void) override;
    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;
private:
    int image_;
    bool sound_;
};

