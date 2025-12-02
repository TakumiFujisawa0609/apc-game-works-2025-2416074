#include "Stage.h"
#include"../../../Manager/InputManager.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
    modelId = MV1LoadModel("Data/FinaleFile.mv1");

    pos_ = { 0.0f,0.0f,0.0f };
    scale_ = { 0.01f,0.01f,0.01f };

    MV1SetPosition(modelId, pos_);
    MV1SetRotationXYZ(modelId, VGet(0, 0, 0));
    MV1SetScale(modelId, scale_);
    // デフォルトライトを有効にする (必須)
    SetLightEnable(TRUE);

    // 光の色を白く設定 (明るさを強調)
    SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));

    // 光の方向を設定 (例: 斜め上、やや手前から照らす方向)
    // X, Y, Z 全て負の値にすることで、モデルの左上斜め前から光が当たるようにします。
    // Yはマイナスにすると「上から下へ」の向きになります。
    SetLightDirection(VGet(1.0f, -1.0f, -1.0f));
}

void Stage::Update(void)
{

	// ステージの位置更新
	MV1SetPosition(modelId, pos_);
}

void Stage::Draw(void)
{
	//モデルの描画
	
	MV1DrawModel(modelId);

	//左上にステージと表示
	//DrawString(50, 50, "Stage", 0xFFFFFF);
}

void Stage::Release(void)
{
	MV1DeleteModel(modelId);
}

int Stage::GetModelId() const
{
    return modelId;
}
