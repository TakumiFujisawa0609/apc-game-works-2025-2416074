#include "GameScene.h"
#include"../Object/Actor/Player/Player.h"
#include"../Object/Actor/Stage/Stage.h"
#include"../Object/Actor/ActorBase.h"
#include"../Object/Actor/Item/Item.h"
#include"../Object/Actor/Stage/Door.h"
#include"../Object/Actor/Enemey/Enemy.h"
#include"../Object/Actor/Stage/NightDoom.h"
#include"../Manager/Camera.h"
#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/SoundManager.h"
#include"../Application.h"
#include <DxLib.h>

GameScene::GameScene()
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Load(void)
{
    // ステージ生成
    stage_ = new Stage();
    stage_->Init();

    //プレイヤー生成
	Player* player = new Player();
    allActor_.push_back(player);

	//敵生成
	Enemy* enemy = new Enemy();
	allActor_.push_back(enemy);

    //ナイトドーム生成
    nDoom = new NightDoom();
    nDoom->Init();

    //アイテム生成
	item_ = new Item();
    item_->Init();

	//ドア生成
	door_ = new Door();
	door_->Init();
    door_->SetItem(item_);

    // カメラ生成
    camera_ = new Camera();
    camera_->Init();
    
	OkCollision_ = true;

    clear_ = false;

	camera_->ChangeMode(Camera::MODE::FOLLOW); // 追従カメラに変更

    //すべてのアクターを初期化
	for (auto actor : allActor_) 
    {
        actor->Load();
		actor->Init();
		

        if (actor->GetType() == ActorBase::ACTOR_TYPE::PLAYER)
        {
            camera_->SetPlayer(static_cast<Player*>(actor));
        }
        else if (actor->GetType() == ActorBase::ACTOR_TYPE::ENEMY)
        {
            // GameSceneへの参照を渡す
            static_cast<Enemy*>(actor)->SetGameScene(this);
        }
	}

    Init();

    startTime_ = GetNowCount();

}

void GameScene::Init(void)
{
   
}

void GameScene::Update(void)
{
   
    auto& ins = InputManager::GetInstance();

    if (ins.IsTrgDown(KEY_INPUT_TAB))
    {
        SoundManager::GetInstance().Play(SoundManager::SRC::CARSOL, Sound::TIMES::ONCE);

        isPaused_ = !isPaused_;
        if (isPaused_)
        {
            menuSelection_ = 0;
            // SoundManager::GetInstance().PauseBGM();
        }
        else
        {
           
            // SoundManager::GetInstance().ResumeBGM();
        }
    }
    // --- ポーズ中の処理 ---
    if (GetJoypadNum() == 0)
    {
        if (isPaused_)
        {
            // メニュー操作
            // 上キーまたはWキーで選択肢を上に移動
            if (ins.IsTrgDown(KEY_INPUT_UP) || ins.IsTrgDown(KEY_INPUT_W))
            {
                menuSelection_--;
                if (menuSelection_ < 0) menuSelection_ = 1; // 選択肢は2つ (0と1)
				SoundManager::GetInstance().Play(SoundManager::SRC::CARSOL, Sound::TIMES::ONCE);
            }
            // 下キーまたはSキーで選択肢を下に移動
            else if (ins.IsTrgDown(KEY_INPUT_DOWN) || ins.IsTrgDown(KEY_INPUT_S))
            {
                menuSelection_++;
                if (menuSelection_ > 1) menuSelection_ = 0;
                SoundManager::GetInstance().Play(SoundManager::SRC::CARSOL, Sound::TIMES::ONCE);
            }

            // 決定 (Spaceキー)
            if (ins.IsNew(KEY_INPUT_SPACE))
            {
                SoundManager::GetInstance().Play(SoundManager::SRC::CARSOL, Sound::TIMES::ONCE);

                if (menuSelection_ == 0) // 0: 続ける
                {
                    isPaused_ = false; // ポーズ解除
                }
                else if (menuSelection_ == 1) // 1: ゲームを終了する (タイトル画面へ戻る)
                {
                    SoundManager::GetInstance().AllStop();
                    SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
                    return; // シーン遷移後の処理をスキップ
                }
                else if (menuSelection_ == 2)
                {
                    //終了

                }
            }

            // ポーズ中はこれ以上のゲーム更新処理をスキップ
            return;
        }
    }
    else
    {
        if (isPaused_)
        {
            //接続されてるコントローラーを取得
            InputManager::JOYPAD_IN_STATE padstate = (ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1));

            // メニュー操作
            // 上キーまたはWキーで選択肢を上に移動
            if (ins.IsTrgDown(KEY_INPUT_UP) || ins.IsTrgDown(KEY_INPUT_W))
            {
                menuSelection_--;
                if (menuSelection_ < 0) menuSelection_ = 1; // 選択肢は2つ (0と1)
            }
            // 下キーまたはSキーで選択肢を下に移動
            else if (ins.IsTrgDown(KEY_INPUT_DOWN) || ins.IsTrgDown(KEY_INPUT_S))
            {
                menuSelection_++;
                if (menuSelection_ > 1) menuSelection_ = 0;
            }

            // 決定 (Enterキー)
            if (ins.IsNew(KEY_INPUT_SPACE))
            {
                if (menuSelection_ == 0) // 0: 続ける
                {
                    isPaused_ = false; // ポーズ解除
                }
                else if (menuSelection_ == 1) // 1: ゲームを終了する (タイトル画面へ戻る)
                {
                    SoundManager::GetInstance().AllStop();
                    SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
                    return; // シーン遷移後の処理をスキップ
                }
            }

            // ポーズ中はこれ以上のゲーム更新処理をスキップ
            return;
        }
    }

    // カメラ更新（ここで視点移動なども反映される）
    camera_->Update();
    // プレイヤーやステージの更新
    stage_->Update();
    //アイテム更新
    item_->Update();

    // ドア更新
    Player* player = GetPlayer(); // GetPlayer() でプレイヤーのポインタを取得
    if (player != nullptr)
    {
        // プレイヤーの位置を取得して、door_->Update に渡す！
        VECTOR playerPos = player->GetPos();
        door_->Update(playerPos);
    }

    nDoom->Update();


 
     //敵のスタック対策ロジック
    if (isNoCollisionMode_)
    {
        // 当たり判定無効モード中の処理
        noCollisionTimer_++;
        if (noCollisionTimer_ >= ENEMY_STATCK_TIME) 
        {
            isNoCollisionMode_ = false;
            OkCollision_ = true;
            noCollisionTimer_ = 0;
            stuckTimer_ = 0; // スタックタイマーもリセット
        }
    }
    else
    {
      
        if (isEnemyStuck_)
        {
            // スタック中ならタイマー増加
            stuckTimer_++;
            if (stuckTimer_ >= 60) // 1秒(60フレーム)以上スタックが続いた
            {
                isNoCollisionMode_ = true;
                OkCollision_ = false; // 当たり判定を無効にする
                stuckTimer_ = 0;

                // これで、当たり判定OFF期間にすぐに再検知されるのを防ぐ
                isEnemyStuck_ = false;
            }
        }
        else
        {
            // スタックしていないならタイマーリセット
            stuckTimer_ = 0;
        }
    }

    // 次のフレームのためにスタックフラグをリセット
    // 衝突判定関数で true に設定される
    isEnemyStuck_ = false;

    //ここまで敵のスタック対策ロジック

    // 全てのアクターを回す
    for (auto actor : allActor_)
    {
        // 更新処理
        actor->Update();

        // 当たり判定（プレイヤーの壁/ドア/アイテムなど）

        WallCollision(actor);
        if (door_->GetDown() == false)
        {
            DoorCollision(actor);
        }
        ItemCollision(actor);
        ItemCollision2(actor);
        ItemCollision3(actor);
        ItemCollision4(actor);

        // 敵のスタック検知
        // Player以外のActorBase*を引数に取っているので、呼び出す前にGetTypeでチェックが必要
        if (actor->GetType() == ActorBase::ACTOR_TYPE::ENEMY)
        {
            EnemyStack(actor);
        }

        // 敵の当たり判定
        if (actor->GetType() == ActorBase::ACTOR_TYPE::ENEMY && OkCollision_ == true)
        {
            EnemyCollision(actor);
        }
    }

    if (GetPlayer()->GetClear() == true)
    {
        SceneManager::GetInstance().SetGameClearState(true);
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::LOAD);

        SoundManager::GetInstance().AllStop();

        return;
    }
    if (GetEnemy()->GetGameOver() == true)
    {
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SCARE);

        SoundManager::GetInstance().AllStop();

        return;
    }
    /*  if (enemyStack_)
      {
          countDown_++;
      }
      else
      {
          countDown_ = 0;
      }

      if (countDown_ >= 60)
      {
          OkCollision_ = false;
          countDown_ = 0;
      }*/

}

void GameScene::Draw(void)
{
    // カメラの描画前設定（SetBeforeDrawFixedPointなどを内部で呼ぶ）
    camera_->SetBeforeDraw();
    //ナイトドーム
    nDoom->Draw();
    // ステージとプレイヤーを描画
    stage_->Draw();
	
	//ドア描画
	door_->Draw();
    // 全てのアクターを回す
    for (auto actor : allActor_)
    {
        // 更新処理
        actor->Draw();
    }
    item_->Debag();
    //アイテム描画
	item_->Draw();

    if (door_ != nullptr && door_->IsOperable())
    {
        // 画面中央下部などにメッセージを表示する
        // PC操作の場合
        if (GetJoypadNum() == 0)
        {
            DrawString(0, 150, "【Eキー】をおせ", GetColor(0, 255, 255));
        }
        // ゲームパッド操作の場合
        else
        {
            DrawString(0, 150, "【Bボタン】をおせ", GetColor(0, 255, 255));
        }
    }

    // UI系描画
    //DrawString(800, 50, "Game Scene", 0xFFFFFF);
    int elapsed_time = GetNowCount() - startTime_;

    if (elapsed_time < 5000) // 5000ミリ秒 (5秒) 未満の場合
    {
        // 画面中央付近に表示する例
        DrawString(0, 150, "カギを四つ集め脱出しろ...", GetColor(255, 255, 0));
    }

    DrawFormatString(0, 0, 0xFFFFFF, "IsGetItem=%d", item_->GetIsGet());
    //DrawFormatString(0, 0, 0xFFFFFF, "GameOver=%d", GetEnemy()->GetGameOver());

  

    // --- メニュー画面の描画 ---
    if (isPaused_)
    {
        DrawMenu();
    }
   
}

void GameScene::Release(void)
{
    // 各オブジェクト解放
    stage_->Release();
    delete stage_;
	// アイテム解放
	item_->Release();
	delete item_;
	//ドア解放
	door_->Release();
	delete door_;
    // 全てのアクターを回す
    for (auto actor : allActor_)
    {
        // 更新処理
        actor->Release();
    }

    camera_->Release();
    delete camera_;

    nDoom->Release();
    delete nDoom;
}

Player* GameScene::GetPlayer()
{
    for (auto actor : allActor_)
    {
        if (actor->GetType() == ActorBase::ACTOR_TYPE::PLAYER)
        {
            return static_cast<Player*>(actor);
        }
    }
    return nullptr;
}

Enemy* GameScene::GetEnemy()
{
    for (auto actor : allActor_)
    {
        if (actor->GetType() == ActorBase::ACTOR_TYPE::ENEMY)
        {
            return static_cast<Enemy*>(actor);
        }
    }
    return nullptr;
}

Item* GameScene::GetItem()
{
    return item_;
}

bool GameScene::CheckGameClear()
{
    return clear_;
}

// ステージの床とプレイヤーの衝突
void GameScene::FieldCollision(ActorBase* actor)
{
    // 座標を所得
    VECTOR actorPos = actor->GetPos();

    // 線分の上座標
    VECTOR startPos = actorPos;
    startPos.y = actorPos.y + 10.0f;

    // 線分の下座標
    VECTOR endPos = actorPos;
    endPos.y = actorPos.y - 10.0f;

    // ステージのモデルを取得
    int modelId = stage_->GetModelId();

    // 線分とモデルの衝突判定
    MV1_COLL_RESULT_POLY res =
        MV1CollCheck_Line(modelId, -1, startPos, endPos);

    // ステージに当たっているか？
    if (res.HitFlag)
    {
        // 当たった場所に戻す
        actor->CollisionStage(res.HitPosition);
    }


}


void GameScene::WallCollision(ActorBase* actor)
{
    // 座標を取得
    VECTOR pos = GetPlayer()->GetPos();

    // カプセルの座標
    VECTOR capStartPos = VAdd(pos, GetPlayer()->GetStartCapsulePos());
    VECTOR capEndPos = VAdd(pos, GetPlayer()->GetEndCapsulePos());

    // カプセルとの当たり判定
    auto hits = MV1CollCheck_Capsule(
        stage_->GetModelId(),   // ステージのモデルID
        -1,                     // ステージ全てのポリゴンを指定
        capStartPos,            // カプセルの上
        capEndPos,              // カプセルの下
        GetPlayer()->GetCapsuleRadius() // カプセルの半径
    );

    // 衝突したポリゴン全ての検索
    for (int i = 0; i < hits.HitNum; i++)
    {
        // ポリゴンを1枚に分割
        auto hit = hits.Dim[i];

        // ポリゴン検索を制限(全てを検索すると重いので)
        for (int tryCnt = 0; tryCnt < 10; tryCnt++)
        {
            // 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
            int pHit = HitCheck_Capsule_Triangle(
                capStartPos,               // カプセルの上
                capEndPos,                 // カプセルの下
                GetPlayer()->GetCapsuleRadius(), // カプセルの半径
                hit.Position[0],           // ポリゴン1
                hit.Position[1],           // ポリゴン2
                hit.Position[2]            // ポリゴン3
            );

            // カプセルとポリゴンが当たっていた
            if (pHit)
            {
                // 当たっていたので座標をポリゴンの法線方向に移動させる
                pos = VAdd(pos, VScale(hit.Normal, 0.1f));
                capStartPos = VAdd(capStartPos, VScale(hit.Normal, 0.1f));
                capEndPos = VAdd(capEndPos, VScale(hit.Normal, 0.1f));

                // 複数当たっている可能性があるので再検索
                continue;
            }
        }
    }

    // 検出したポリゴン情報の後始末
    MV1CollResultPolyDimTerminate(hits);

    // 計算した場所にアクターを戻す
    GetPlayer()->CollisionStage(pos);
}

void GameScene::DoorCollision(ActorBase* actor)
{
    // 座標を取得
    VECTOR pos = actor->GetPos();

    // カプセルの座標
    VECTOR capStartPos = VAdd(pos, actor->GetStartCapsulePos());
    VECTOR capEndPos = VAdd(pos, actor->GetEndCapsulePos());

    // カプセルとの当たり判定
    auto hits = MV1CollCheck_Capsule(
        door_->GetModelId(),   // ステージのモデルID
        -1,                     // ステージ全てのポリゴンを指定
        capStartPos,            // カプセルの上
        capEndPos,              // カプセルの下
        actor->GetCapsuleRadius() // カプセルの半径
    );

    // 衝突したポリゴン全ての検索
    for (int i = 0; i < hits.HitNum; i++)
    {
        // ポリゴンを1枚に分割
        auto hit = hits.Dim[i];

        // ポリゴン検索を制限(全てを検索すると重いので)
        for (int tryCnt = 0; tryCnt < 10; tryCnt++)
        {
            // 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
            int pHit = HitCheck_Capsule_Triangle(
                capStartPos,               // カプセルの上
                capEndPos,                 // カプセルの下
                actor->GetCapsuleRadius(), // カプセルの半径
                hit.Position[0],           // ポリゴン1
                hit.Position[1],           // ポリゴン2
                hit.Position[2]            // ポリゴン3
            );

            // カプセルとポリゴンが当たっていた
            if (pHit)
            {
                // 当たっていたので座標をポリゴンの法線方向に移動させる
                pos = VAdd(pos, VScale(hit.Normal, 0.1f));
                capStartPos = VAdd(capStartPos, VScale(hit.Normal, 0.1f));
                capEndPos = VAdd(capEndPos, VScale(hit.Normal, 0.1f));

                // 複数当たっている可能性があるので再検索
                continue;
            }
        }
    }

    // 検出したポリゴン情報の後始末
    MV1CollResultPolyDimTerminate(hits);

    // 計算した場所にアクターを戻す
    actor->CollisionStage(pos);

}

void GameScene::ItemCollision(ActorBase* actor)
{
    // Player 以外は対象外
    if (actor->GetType() != ActorBase::ACTOR_TYPE::PLAYER) return;

    Player* player = static_cast<Player*>(actor);

    // すでにアイテム取得済みなら何もしない
    if (item_->IsGotten()) return;

    // プレイヤー位置とカプセル
    VECTOR pos = player->GetPos();
    VECTOR capStartPos = VAdd(pos, player->GetStartCapsulePos());
    VECTOR capEndPos = VAdd(pos, player->GetEndCapsulePos());

    // アイテムとの衝突判定
    auto hits = MV1CollCheck_Capsule(
        item_->GetModelId(),
        -1,
        capStartPos,
        capEndPos,
        player->GetCapsuleRadius()
    );

    if (hits.HitNum > 0)
    {
        // ヒットした場合
        player->OnGetItem();
        item_->SetGotten(true);
        item_->SetIsGet(true);
    }

    // 結果の後始末
    MV1CollResultPolyDimTerminate(hits);
}

void GameScene::ItemCollision2(ActorBase* actor)
{
    // Player 以外は対象外
    if (actor->GetType() != ActorBase::ACTOR_TYPE::PLAYER) return;

    Player* player = static_cast<Player*>(actor);

    // すでにアイテム取得済みなら何もしない
    if (item_->IsGotten2()) return;

    // プレイヤー位置とカプセル
    VECTOR pos = player->GetPos();
    VECTOR capStartPos = VAdd(pos, player->GetStartCapsulePos());
    VECTOR capEndPos = VAdd(pos, player->GetEndCapsulePos());

    // アイテムとの衝突判定
    auto hits = MV1CollCheck_Capsule(
        item_->GetModelId2(),
        -1,
        capStartPos,
        capEndPos,
        player->GetCapsuleRadius()
    );

    if (hits.HitNum > 0)
    {
        // ヒットした場合
        player->OnGetItem();
        item_->SetGotten2(true);
        item_->SetIsGet(true);
    }

    // 結果の後始末
    MV1CollResultPolyDimTerminate(hits);
}

void GameScene::ItemCollision3(ActorBase* actor)
{
    // Player 以外は対象外
    if (actor->GetType() != ActorBase::ACTOR_TYPE::PLAYER) return;

    Player* player = static_cast<Player*>(actor);

    // すでにアイテム取得済みなら何もしない
    if (item_->IsGotten3()) return;

    // プレイヤー位置とカプセル
    VECTOR pos = player->GetPos();
    VECTOR capStartPos = VAdd(pos, player->GetStartCapsulePos());
    VECTOR capEndPos = VAdd(pos, player->GetEndCapsulePos());

    // アイテムとの衝突判定
    auto hits = MV1CollCheck_Capsule(
        item_->GetModelId3(),
        -1,
        capStartPos,
        capEndPos,
        player->GetCapsuleRadius()
    );

    if (hits.HitNum > 0)
    {
        // ヒットした場合
        player->OnGetItem();
        item_->SetGotten3(true);
        item_->SetIsGet(true);
    }

    // 結果の後始末
    MV1CollResultPolyDimTerminate(hits);
}

void GameScene::ItemCollision4(ActorBase* actor)
{
    // Player 以外は対象外
    if (actor->GetType() != ActorBase::ACTOR_TYPE::PLAYER) return;

    Player* player = static_cast<Player*>(actor);

    // すでにアイテム取得済みなら何もしない
    if (item_->IsGotten4()) return;

    // プレイヤー位置とカプセル
    VECTOR pos = player->GetPos();
    VECTOR capStartPos = VAdd(pos, player->GetStartCapsulePos());
    VECTOR capEndPos = VAdd(pos, player->GetEndCapsulePos());

    // アイテムとの衝突判定
    auto hits = MV1CollCheck_Capsule(
        item_->GetModelId4(),
        -1,
        capStartPos,
        capEndPos,
        player->GetCapsuleRadius()
    );

    if (hits.HitNum > 0)
    {
        // ヒットした場合
        player->OnGetItem();
        item_->SetGotten4(true);
        item_->SetIsGet(true);
    }

    // 結果の後始末
    MV1CollResultPolyDimTerminate(hits);
}
    

void GameScene::EnemyCollision(ActorBase* actor)
{
    // 座標を取得
    VECTOR pos = GetEnemy()->GetPos();

    // カプセルの座標
    VECTOR capStartPos = VAdd(pos, GetEnemy()->GetStartCapsulePos());
    VECTOR capEndPos = VAdd(pos, GetEnemy()->GetEndCapsulePos());

    // カプセルとの当たり判定
    auto hits = MV1CollCheck_Capsule(
        stage_->GetModelId(),   // ステージのモデルID
        -1,                     // ステージ全てのポリゴンを指定
        capStartPos,            // カプセルの上
        capEndPos,              // カプセルの下
        GetEnemy()->GetCapsuleRadius() // カプセルの半径
    );

    // 衝突したポリゴン全ての検索
    for (int i = 0; i < hits.HitNum; i++)
    {
        // ポリゴンを1枚に分割
        auto hit = hits.Dim[i];

        // ポリゴン検索を制限(全てを検索すると重いので)
        for (int tryCnt = 0; tryCnt < 10; tryCnt++)
        {
            // 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
            int pHit = HitCheck_Capsule_Triangle(
                capStartPos,               // カプセルの上
                capEndPos,                 // カプセルの下
                GetEnemy()->GetCapsuleRadius(), // カプセルの半径
                hit.Position[0],           // ポリゴン1
                hit.Position[1],           // ポリゴン2
                hit.Position[2]            // ポリゴン3
            );

            // カプセルとポリゴンが当たっていた
            if (pHit)
            {
                // 当たっていたので座標をポリゴンの法線方向に移動させる
                pos = VAdd(pos, VScale(hit.Normal, 0.1f));
                capStartPos = VAdd(capStartPos, VScale(hit.Normal, 0.1f));
                capEndPos = VAdd(capEndPos, VScale(hit.Normal, 0.1f));

                // 複数当たっている可能性があるので再検索
                continue;
            }
        }
    }

    // 検出したポリゴン情報の後始末
    MV1CollResultPolyDimTerminate(hits);

    // 計算した場所にアクターを戻す
    GetEnemy()->CollisionStage(pos);
}

void GameScene::EnemyStack(ActorBase* actor)
{
    // Enemy 以外は対象外
    if (actor->GetType() != ActorBase::ACTOR_TYPE::ENEMY) return;

    Enemy* enemy = static_cast<Enemy*>(actor);

    // 座標を取得
    VECTOR pos = enemy->GetPos();

    // カプセルの座標
    VECTOR capStartPos = VAdd(pos, enemy->GetStartCapsulePos());
    VECTOR capEndPos = VAdd(pos, enemy->GetEndCapsulePos());

    // カプセルとの当たり判定
    auto hits = MV1CollCheck_Capsule(
        stage_->GetModelId(),   // ステージのモデルID
        -1,                     // ステージ全てのポリゴンを指定
        capStartPos,            // カプセルの上
        capEndPos,              // カプセルの下
        enemy->GetCapsuleRadius() // カプセルの半径
    );

    // 衝突したポリゴン全ての検索
    if (hits.HitNum > 0)
    {
        // **壁に当たっている！**
        // isEnemyStuck_をtrueにする
        isEnemyStuck_ = true;
    }

    // 検出したポリゴン情報の後始末
    MV1CollResultPolyDimTerminate(hits);
}

void GameScene::DrawMenu()
{
    // 画面サイズを取得 (必要に応じて適切な値を使用)
    int screenW, screenH;

    GetDrawScreenSize(&screenW, &screenH);

    // メニュー背景の描画 (半透明の黒)
    // 画面全体を覆い、ゲーム画面が透けて見えるようにする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180); 
    DrawBox(0, 0, screenW, screenH, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードを元に戻す

    // --- タイトルと情報 ---
    int base_y = screenH / 2 - 100;
    int margin_y = 50;
    int color = GetColor(255, 255, 255); // 白
    int select_color = GetColor(255, 255, 0); // 黄色

    // メニュータイトル
    DrawString(screenW / 2 - 80, base_y, "PAUSE MENU", color);

    // --- メニュー選択肢 ---
    base_y += margin_y * 3;

    const char* options[] = {
        "ゲームに戻る",
        "タイトル戻る",
    };

    for (int i = 0; i < 2; i++)
    {
        int draw_color = (menuSelection_ == i) ? select_color : color;
        DrawString(screenW / 2 - 120, base_y + margin_y * i, options[i], draw_color);
    }

    // 操作説明
    DrawString(screenW / 2 - 700, screenH - 50, "【↑/↓キー】選択 【Spaceキー】決定 【Tabキー】ポーズ解除", color);
}



