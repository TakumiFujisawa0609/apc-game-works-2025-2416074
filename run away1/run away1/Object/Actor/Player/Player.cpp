#include "Player.h"
#include <cmath>
#include"../../../Manager/InputManager.h"
#include"../../../Manager/Camera.h"
#include"../../../Utility/AsoUtility.h"
#include"../../../Manager/SoundManager.h"
#include"../ActorBase.h"
#include"../../../Manager/SceneManager.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Load()
{
    // リソースロード
	InitLoad();
}

void Player::LoadEnd()
{
    Init();   
}

void Player::Init()
{
	// Transform初期化
	InitTransform();
	// アニメーションの初期化
	InitAnimation();
	// 初期化後の個別処理
	InitPost();
}

void Player::Update()
{
    Move();

    auto& ins = InputManager::GetInstance();
    if (pos_.z < -57.0f)
    {
        pos_ = INITPOS;
        rot_ = VGet(0.0f, 0.0f, 0.0f);
        verticalSpeed_ = 0.0f;
        isGrounded_ = true;
        announce = true;
    }

    if (announce == true)
    {
        countDown_++;
        if (countDown_ == 120)
        {
            announce = false;
			countDown_ = 0;
        }
    }
  
    // モデル更新
    MV1SetPosition(modelId_, pos_);
    MV1SetRotationXYZ(modelId_, rot_);

    //デバッグ用
    if (pos_.x > gameClearPos_.x && pos_.z <= gameClearPos_.z)
    {
        gameClear_ = true;
    }
    else
    {
        gameClear_ = false;
    }

	
}

void Player::Draw()
{
    //MV1DrawModel(modelId_);
    ActorBase::Draw();

   //DrawFormatString(0, 60, 0xFFFFFF, "pos:(%.2f,%.2f,%.2f)", pos_.x, pos_.y, pos_.z);
   // DrawFormatString(0, 20, 0xFFFFFF, "rot:(%.2f,%.2f,%.2f)", rot_.x, rot_.y, rot_.z);

     //スタミナ残量の計算
    float staminaRatio = currentStamina_ / maxStamina_;

    //ゲージの実幅を計算
    int currentBarWidth = static_cast<int>(STAMINA_BAR_WIDTH * staminaRatio);

    //ゲージの色を決定
    unsigned int barColor = COLOR_GREEN;
    if (staminaRatio < 0.3f) {
        barColor = COLOR_RED; // 30%以下で赤
    }
    else if (staminaRatio < 0.6f) {
        barColor = COLOR_YELLOW; // 60%以下で黄色
    }

    //ゲージ背景
    DrawBox(
        STAMINA_BAR_X,
        STAMINA_BAR_Y,
        STAMINA_BAR_X + STAMINA_BAR_WIDTH,
        STAMINA_BAR_Y + STAMINA_BAR_HEIGHT,
        COLOR_BLACK,
        TRUE
    );

    DrawBox(
        STAMINA_BAR_X,
        STAMINA_BAR_Y,
        STAMINA_BAR_X + STAMINA_BAR_WIDTH,
        STAMINA_BAR_Y + STAMINA_BAR_HEIGHT,
        COLOR_BLACK,
        TRUE
    );

    //ゲージ実線 (スタミナ残量)
    DrawBox(
        STAMINA_BAR_X + STAMINA_BAR_THICKNESS,
        STAMINA_BAR_Y + STAMINA_BAR_THICKNESS,
        STAMINA_BAR_X + currentBarWidth - STAMINA_BAR_THICKNESS,
        STAMINA_BAR_Y + STAMINA_BAR_HEIGHT - STAMINA_BAR_THICKNESS,
        barColor,
        TRUE
    );

    // スタミナ値をテキスト表示
    DrawFormatString(
        STAMINA_BAR_X + STAMINA_BAR_WIDTH + 10,
        STAMINA_BAR_Y,
        COLOR_WHITE,
        "Stamina: %.1f / %.0f",
        currentStamina_,
        maxStamina_
    );

    // DrawFormatString(0, 20, 0xFFFFFF, "isGrounded: %d", isDashing_); // デバッグ用表示は、必要なければここで削除できます

    if (announce)
    {
        DrawString(800, 500, "最初っからだ", 0xFFFFFF);
    }

}

void Player::Release()
{
    MV1DeleteModel(modelId_);
}

void Player::OnGetItem()
{
    if (!hasItem_) {
        hasItem_ = true;
    }
}

VECTOR Player::GetPos()
{
    return pos_;
}

VECTOR Player::GetRot()
{
    return rot_;
}

bool Player::GetClear()
{
    return gameClear_;
}

void Player::SetPos(VECTOR pos)
{
    pos_ = pos;
    MV1SetPosition(modelId_,pos_);
}

void Player::InitLoad(void)
{
    modelId_ = MV1LoadModel("Data/layer.mv1");
}

void Player::InitTransform(void)
{
   // ActorBase::Update();
    rot_ = VGet(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
   
    baseMoveSpeed_ = MOVE_SPEED;
    moveSpeed_ = baseMoveSpeed_;
    mouseSensitivity_ = MOUSE_SENSITIVITY;
    jumpPow_ = JUMP_POW;

    startCapsulePos_ = { 0.0f,0.8f,0.0f };
    endCapsulePos_ = { 0.0f,-0.8f,0.0f };
    capsuleRadius_ = 1.2f;
    pos_ = INITPOS;
    isCollision_ = true;
	isGrounded_ = true;    

    gameClearPos_ = VGet(27.00f, 41.10f, -20.00f);
    gameClear_ = false;

    MV1SetPosition(modelId_, pos_);
    MV1SetRotationXYZ(modelId_, rot_);
    MV1SetScale(modelId_, VGet(0.01f, 0.01f, 0.01f));

    maxStamina_ = 150.0f;
    currentStamina_ = maxStamina_;
    staminaConsumptionRate_ = 45.0f; // 1秒あたり45消費
    staminaRecoveryRate_ = 10.0f;    // 1秒あたり10回復
    dashSpeedMultiplier_ = 1.75f;     // 速度1.75倍
    isDashing_ = false;

    staminaCoolDownTimer_ = 0.0f;
    announce = false;
}

void Player::InitAnimation(void)
{
}

void Player::InitPost(void)
{

}

void Player::Move()
{
    auto& ins = InputManager::GetInstance();
    float deltaTime = SceneManager::GetInstance().GetDeltaTime();

    const bool wasDashing = isDashing_;

    if (staminaCoolDownTimer_ > 0.0f) {
        staminaCoolDownTimer_ -= deltaTime;
    }

    bool isNowDashing = false;

    VECTOR previousPos = pos_;
    if (GetJoypadNum() == 0)
    {
        //マウスによる視点回転
        int screenCenterX = 640;
        int screenCenterY = 360;
        int mouseX, mouseY;
        GetMousePoint(&mouseX, &mouseY);

        int dx = mouseX - screenCenterX;
        int dy = mouseY - screenCenterY;

        rot_.y += dx * mouseSensitivity_;// 左右
        rot_.x += dy * mouseSensitivity_;// 上下

        // 上下角の制限
        const float limit = DX_PI_F / 2.0f - 0.1f;
        if (rot_.x > limit) rot_.x = limit;
        if (rot_.x < -limit) rot_.x = -limit;

        // マウス位置を中央に戻す
        SetMousePoint(screenCenterX, screenCenterY);

        //移動方向
        VECTOR forward = VGet(sinf(rot_.y), 0, cosf(rot_.y));
        VECTOR right = VGet(cosf(rot_.y), 0, -sinf(rot_.y));

        moveDir_ = VGet(0, 0, 0);


        if (ins.IsNew(KEY_INPUT_W)) moveDir_ = VAdd(moveDir_, forward);
        if (ins.IsNew(KEY_INPUT_S)) moveDir_ = VSub(moveDir_, forward);
        if (ins.IsNew(KEY_INPUT_D)) moveDir_ = VAdd(moveDir_, right);
        if (ins.IsNew(KEY_INPUT_A)) moveDir_ = VSub(moveDir_, right);

        //デバッグ用上下移動
     

        bool wantsToDash = ins.IsNew(KEY_INPUT_LSHIFT) && (moveDir_.x != 0 || moveDir_.z != 0); // 左Shiftと移動入力でダッシュを要求

        // ダッシュ判定: 要求があり、かつスタミナが残っている
        if (wantsToDash && currentStamina_ > 0.0f)
        {
            isDashing_ = true;

            isNowDashing = true; // 現在ダッシュ中

            // スタミナ消費
            currentStamina_ -= staminaConsumptionRate_ * deltaTime;
            if (currentStamina_ < 0.0f) currentStamina_ = 0.0f;

            moveSpeed_ = baseMoveSpeed_ * dashSpeedMultiplier_; // ダッシュ速度に設定


        }
        else
        {
            isDashing_ = false;
            moveSpeed_ = baseMoveSpeed_; // 基本速度に戻す

            isNowDashing = false; // 現在ダッシュ中

            //ダッシュが終了した瞬間にクールタイムを設定
            if (wasDashing && !isNowDashing)
            {
                staminaCoolDownTimer_ = STAMINA_COOLDOWN_DURATION;
            }

            // スタミナ回復
            // 移動中でない、またはダッシュしていない場合に回復
            if (!wantsToDash) // ダッシュ要求がなければ回復（クールタイムを設けるならここに条件を追加）
            {
                if (staminaCoolDownTimer_ <= 0.0f)
                {
                    currentStamina_ += staminaRecoveryRate_ * deltaTime;
                    if (currentStamina_ > maxStamina_) currentStamina_ = maxStamina_;
                }
            }
            //現在のスタミナが50を下回っているかつ、ダッシュしていない場合息切れのSEを再生
            if (currentStamina_ <= 50.0f && !isNowDashing)
            {
				SoundManager::GetInstance().Play(SoundManager::SRC::BLACEOUT, Sound::TIMES::ONCE, true);
            }
			else //それ以外の場合はSEを停止
            {
				SoundManager::GetInstance().Stop(SoundManager::SRC::BLACEOUT);
            }
        }

        //デバッグ用上下移動
         /* if (ins.IsNew(KEY_INPUT_E)) pos_.y += 0.1f;
        if (ins.IsNew(KEY_INPUT_Q)) pos_.y -= 0.1f;*/

        // ジャンプ入力
        if (isGrounded_ && ins.IsNew(KEY_INPUT_SPACE))
        {
            verticalSpeed_ = jumpPow_;// 上方向速度をセット
            isGrounded_ = false;
        }

        if (moveDir_.x != 0 || moveDir_.z != 0)
        {
            moveDir_ = VNorm(moveDir_);
            // ここは元のコードのまま、deltaTimeは乗算しない
            pos_ = VAdd(pos_, VScale(moveDir_, moveSpeed_));
        }
    }
    else
    {
        //接続されてるコントローラーを取得
        InputManager::JOYPAD_IN_STATE padstate = (ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1));

       
        float lx = padstate.AKeyLX;
        float ly = padstate.AKeyLY;

        // 入力があるかチェック
        if (fabs(lx) > 0.1f || fabs(ly) > 0.1f)
        {
            // スティックの方向を正規化
            VECTOR localDir = VNorm(VGet(lx, 0, -ly));


            // --- プレイヤーの向きで回転 ---
            // rot_.y（視点角度）を使ってローカル→ワールドへ変換
            float yaw = rot_.y;
            VECTOR forward = VGet(sinf(yaw), 0, cosf(yaw));
            VECTOR right = VGet(cosf(yaw), 0, -sinf(yaw));

            // 視点方向に基づいた移動ベクトル
            moveDir_ = VAdd(VScale(forward, localDir.z), VScale(right, localDir.x));

            // 移動
            moveDir_ = VNorm(moveDir_);
            pos_ = VAdd(pos_, VScale(moveDir_, moveSpeed_));
        }

        //右スティックで視点操作
        rot_.y += padstate.AKeyRX * mouseSensitivity_ * 0.1;// 左右
        rot_.x += padstate.AKeyRY * mouseSensitivity_ * 0.1;// 上下

        bool wantsToDash = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER) && (moveDir_.x != 0 || moveDir_.z != 0); // 左Shiftと移動入力でダッシュを要求

        // ダッシュ判定: 要求があり、かつスタミナが残っている
        if (wantsToDash && currentStamina_ > 0.0f)
        {
            isDashing_ = true;
            isNowDashing = true; // 現在ダッシュ中
            // スタミナ消費
            currentStamina_ -= staminaConsumptionRate_ * deltaTime;
            if (currentStamina_ < 0.0f) currentStamina_ = 0.0f;

            moveSpeed_ = baseMoveSpeed_ * dashSpeedMultiplier_; // ダッシュ速度に設定

        }
        else
        {
            isDashing_ = false;
            moveSpeed_ = baseMoveSpeed_; // 基本速度に戻す
            isNowDashing = false; // 現在ダッシュ中

            // ★変更: ダッシュが終了した瞬間にクールタイムを設定
            if (wasDashing && !isNowDashing)
            {
                staminaCoolDownTimer_ = STAMINA_COOLDOWN_DURATION;
            }

            // スタミナ回復
            // 移動中でない、またはダッシュしていない場合に回復
            if (!wantsToDash) 
            {
                //クールタイム中でなければ回復処理を実行
                if (staminaCoolDownTimer_ <= 0.0f)
                {
                    currentStamina_ += staminaRecoveryRate_ * deltaTime;
                    if (currentStamina_ > maxStamina_) currentStamina_ = maxStamina_;
                }
            }
        }

    }

    // 重力/垂直方向の速度を deltaTime で更新 (既存コードから変更なし)
    pos_.y += verticalSpeed_ * deltaTime;

    // 実際の移動が行われたかチェック (水平方向)
    // わずかでも水平方向の移動があったらtrue
    bool isActuallyMoving = (VSize({ pos_.x - previousPos.x, 0.0f, pos_.z - previousPos.z }) > 0.001f);

    // isMovingNowフラグを定義
    bool isMovingNow = isActuallyMoving; // 移動しているかどうかの判定

    auto& sm = SoundManager::GetInstance();

    //SE再生
    if (SceneManager::GetInstance().GetGameClearState() == false)
    {
        if (isNowDashing)
        {
           
            sm.Stop(SoundManager::SRC::WALK); //WALKを停止

            // ダッシュSEを再生
            if (!sm.CheckMove(SoundManager::SRC::RUN)) {
                sm.Play(SoundManager::SRC::RUN, Sound::TIMES::ONCE);
            }
        }
        else if (isMovingNow)
        {
          
            sm.Stop(SoundManager::SRC::RUN); // RUN(DASH)を停止

            // 歩行SEを再生
            if (!sm.CheckMove(SoundManager::SRC::WALK)) {
                sm.Play(SoundManager::SRC::WALK, Sound::TIMES::ONCE);
            }
        }
        else 
        {
            // すべての移動BGMを停止
            sm.Stop(SoundManager::SRC::RUN);
            sm.Stop(SoundManager::SRC::WALK);
        }
    }
    else
    {
        if (isNowDashing) 
        {
          
            sm.Stop(SoundManager::SRC::GRASS_WALK);

          
            if (!sm.CheckMove(SoundManager::SRC::GRASS_RUN)) {
                sm.Play(SoundManager::SRC::GRASS_RUN, Sound::TIMES::ONCE);
            }
        }
        else if (isMovingNow)
        {
           
            sm.Stop(SoundManager::SRC::GRASS_RUN);

           
            if (!sm.CheckMove(SoundManager::SRC::GRASS_WALK)) {
                sm.Play(SoundManager::SRC::GRASS_WALK, Sound::TIMES::ONCE);
            }
        }
        else 
        {
            // すべての移動BGMを停止
            sm.Stop(SoundManager::SRC::GRASS_RUN);
            sm.Stop(SoundManager::SRC::GRASS_WALK);
        }
    }

    //isDashing_ と wasDashingLastFrame_ の更新
    isDashing_ = isNowDashing;
    wasDashingLastFrame_ = isNowDashing; // ダッシュの開始・終了を判定するために、この行は残す


}
