#include "GameClearScene.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include"../Object/Actor/ActorBase.h"
#include"../Object/Actor/Enemey/Enemy.h"
#include"../Object/Actor/Player/Player.h"
#include"../Manager/Camera.h"
#include"../Manager/SoundManager.h"
#include"../Object/Actor/Stage/NightDoom.h"
#include"../Application.h"
GameClearScene::GameClearScene()
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Load(void)
{
	pPos_ = VGet(0.05f, -17.89f, 27.27f);

	
	//プレイヤー生成
	Player* player = new Player();
	allActor_.push_back(player);

	//ナイトドーム生成
	nDoom = new NightDoom();
	nDoom->Init();

	// カメラ生成
	camera_ = new Camera();
	camera_->Init();

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
			static_cast<Enemy*>(actor)->SetGameClearScene(this);
		}
	}
	GetPlayer()->SetPos(pPos_);
	clearPos_ = { 0.27f,-17.89f,104.21f };

	gameOver_ = false;

	exit_ = MV1LoadModel("Data/Exit.mv1");
	forest_=MV1LoadModel("Data/Morimori.mv1");
	forest2_ = MV1LoadModel("Data/Morimori.mv1");

	pos_1 = { 0.0f,0.0f,0.0f };
	pos_2 = { 119.50f,-53.50f,-12.0f };
	pos_3 = { 119.50f,-53.50f,72.5f };

	scl_ = { 0.01f,0.01f,0.01f };

	rot_ = { 0.0f,0.0f,0.0f };

	MV1SetPosition(exit_, pos_1);
	MV1SetPosition(forest_, pos_2);
	MV1SetPosition(forest2_, pos_3);

	MV1SetScale(exit_, scl_);
	MV1SetScale(forest_, scl_);
	MV1SetScale(forest2_, scl_);

	MV1SetRotationXYZ(exit_,rot_);
	MV1SetRotationXYZ(forest2_, rot_);
	MV1SetRotationXYZ(forest_, rot_);

}

void GameClearScene::Init(void)
{

}

void GameClearScene::Update(void)
{
	auto& ins = InputManager::GetInstance();


	//if (GetJoypadNum() == 0)
	//{
	//	if (ins.IsTrgUp(KEY_INPUT_SPACE))
	//	{
	//		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	//	}
	//}
	//else
	//{
	//	// 接続されているゲームパッド１の情報を取得
	//	InputManager::JOYPAD_IN_STATE padState =
	//		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	//	// Bボタンが押されたらゲームシーンへ
	//	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))
	//	{
	//		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	//	}
	//}

	// 全てのアクターを回す
	for (auto actor : allActor_)
	{
		// 更新処理
		actor->Update();

		// 当たり判定（プレイヤーの壁/ドア/アイテムなど）
		// ... (FieldCollision, WallCollision, DoorCollision, ItemCollision... はそのまま) ...
		WallCollision(actor);


		//// 敵のスタック検知
		//// Player以外のActorBase*を引数に取っているので、呼び出す前にGetTypeでチェックが必要
		//if (actor->GetType() == ActorBase::ACTOR_TYPE::ENEMY)
		//{
		//	EnemyStack(actor);
		//}

		//// 敵の当たり判定
		//if (actor->GetType() == ActorBase::ACTOR_TYPE::ENEMY)
		//{
		//	EnemyCollision(actor);
		//}
	}

	//カメラ更新
	camera_->Update();

	nDoom->Update();

	Cleared();

	//-15.0fより小さい15より大きい
	if (GetPlayer()->GetPos().x < -15.0f || GetPlayer()->GetPos().x>15.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SCARE);
		gameOver_ = true;
		SceneManager::GetInstance().SetGameOverState(true);
		SoundManager::GetInstance().AllStop();
		return;
	}
	else
	{
		gameOver_ = false;
	}
	
	//デバッグ
	//if (ins.IsNew(KEY_INPUT_W)) { pos_3.z += 0.5; }
	//if (ins.IsNew(KEY_INPUT_S)) { pos_3.z -= 0.5; }
	//if (ins.IsNew(KEY_INPUT_A)) { pos_3.x -= 0.5; }
	//if (ins.IsNew(KEY_INPUT_D)) { pos_3.x += 0.5; }

	//if (ins.IsNew(KEY_INPUT_E)) pos_3.y += 0.5f;
	//if (ins.IsNew(KEY_INPUT_Q)) pos_3.y -= 0.5f;

	MV1SetPosition(exit_, pos_1);
	MV1SetPosition(forest_, pos_2);
	MV1SetPosition(forest2_, pos_3);
	MV1SetScale(exit_, scl_);
	MV1SetScale(forest_, scl_);
	MV1SetScale(forest2_, scl_);
	MV1SetRotationXYZ(exit_, rot_);
	MV1SetRotationXYZ(forest_, rot_);
	MV1SetRotationXYZ(forest2_, rot_);

	if (cleared_== true)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::FINAL);
		SoundManager::GetInstance().AllStop();

		return;
	}
}

void GameClearScene::Draw(void)
{

	// カメラの描画前設定（SetBeforeDrawFixedPointなどを内部で呼ぶ）
	camera_->SetBeforeDraw();

	
	//ナイトドーム
	nDoom->Draw();
	MV1DrawModel(exit_);
	MV1DrawModel(forest_);
	MV1DrawModel(forest2_);
	// 全てのアクターを回す
	for (auto actor : allActor_)
	{
		// 更新処理
		actor->Draw();
	}

	//カメラ描画
	//camera_->DrawDebug();

	

	/*DrawFormatString(0, 0, 0xFFFFFF, "pos:(%.2f,%.2f,%.2f)", pos_1.x, pos_1.y, pos_1.z);

	DrawFormatString(0, 40, 0xFFFFFF, "pos:(%.2f,%.2f,%.2f)", pos_2.x, pos_2.y, pos_2.z);

	DrawFormatString(0, 80, 0xFFFFFF, "pos:(%.2f,%.2f,%.2f)", pos_3.x, pos_3.y, pos_3.z);

	DrawFormatString(0, 100, 0xFFFFFF, "gameOver=%d",gameOver_);*/
	
}

void GameClearScene::Release(void)
{
	// 全てのアクターを回す
	for (auto actor : allActor_)
	{
		// actor->Release(); はアクター内部の解放
		// actor 自体のメモリ解放は別途必要 (通常はデストラクタまたはReleaseで)
		// 今回は allActor_ が ActorBase* を保持しているので、ここで delete するのが一般的
		delete actor;
	}
	allActor_.clear(); // listをクリア

	// モデルの解放
	MV1DeleteModel(exit_);
	MV1DeleteModel(forest_);

	// カメラの解放
	if (camera_ != nullptr)
	{
		delete camera_;
		camera_ = nullptr; // 解放後に必ず nullptr にする
	}

	// ナイトドームの解放
	if (nDoom != nullptr)
	{
		delete nDoom;
		nDoom = nullptr; // 解放後に必ず nullptr にする
	}
}

Player* GameClearScene::GetPlayer()
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
//
//Enemy* GameClearScene::GetEnemy()
//{
//	for (auto actor : allActor_)
//	{
//		if (actor->GetType() == ActorBase::ACTOR_TYPE::ENEMY)
//		{
//			return static_cast<Enemy*>(actor);
//		}
//	}
//	return nullptr;
//}

void GameClearScene::Cleared()
{
	if (GetPlayer()->GetPos().x > clearPos_.x && GetPlayer()->GetPos().z > clearPos_.z)
	{
		cleared_ = true;
	}
	else
	{
		cleared_ = false;
	}
}

void GameClearScene::WallCollision(ActorBase* actor)
{
	// 座標を取得
	VECTOR pos = GetPlayer()->GetPos();

	// カプセルの座標
	VECTOR capStartPos = VAdd(pos, GetPlayer()->GetStartCapsulePos());
	VECTOR capEndPos = VAdd(pos, GetPlayer()->GetEndCapsulePos());

	// カプセルとの当たり判定
	auto hits = MV1CollCheck_Capsule(
		exit_,   // ステージのモデルID
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

void GameClearScene::ForestCollision(ActorBase* actor)
{

	// 座標を取得
	VECTOR pos = GetPlayer()->GetPos();

	// カプセルの座標
	VECTOR capStartPos = VAdd(pos, GetPlayer()->GetStartCapsulePos());
	VECTOR capEndPos = VAdd(pos, GetPlayer()->GetEndCapsulePos());

	// カプセルとの当たり判定
	auto hits = MV1CollCheck_Capsule(
		forest_,   // ステージのモデルID
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

//void GameClearScene::EnemyCollision(ActorBase* actor)
//{
//	// 座標を取得
//	VECTOR pos = GetEnemy()->GetPos();
//
//	// カプセルの座標
//	VECTOR capStartPos = VAdd(pos, GetEnemy()->GetStartCapsulePos());
//	VECTOR capEndPos = VAdd(pos, GetEnemy()->GetEndCapsulePos());
//
//	// カプセルとの当たり判定
//	auto hits = MV1CollCheck_Capsule(
//		exit_,   // ステージのモデルID
//		-1,                     // ステージ全てのポリゴンを指定
//		capStartPos,            // カプセルの上
//		capEndPos,              // カプセルの下
//		GetEnemy()->GetCapsuleRadius() // カプセルの半径
//	);
//
//	// 衝突したポリゴン全ての検索
//	for (int i = 0; i < hits.HitNum; i++)
//	{
//		// ポリゴンを1枚に分割
//		auto hit = hits.Dim[i];
//
//		// ポリゴン検索を制限(全てを検索すると重いので)
//		for (int tryCnt = 0; tryCnt < 10; tryCnt++)
//		{
//			// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
//			int pHit = HitCheck_Capsule_Triangle(
//				capStartPos,               // カプセルの上
//				capEndPos,                 // カプセルの下
//				GetEnemy()->GetCapsuleRadius(), // カプセルの半径
//				hit.Position[0],           // ポリゴン1
//				hit.Position[1],           // ポリゴン2
//				hit.Position[2]            // ポリゴン3
//			);
//
//			// カプセルとポリゴンが当たっていた
//			if (pHit)
//			{
//				// 当たっていたので座標をポリゴンの法線方向に移動させる
//				pos = VAdd(pos, VScale(hit.Normal, 0.1f));
//				capStartPos = VAdd(capStartPos, VScale(hit.Normal, 0.1f));
//				capEndPos = VAdd(capEndPos, VScale(hit.Normal, 0.1f));
//
//				// 複数当たっている可能性があるので再検索
//				continue;
//			}
//		}
//	}
//
//	// 検出したポリゴン情報の後始末
//	MV1CollResultPolyDimTerminate(hits);
//
//	// 計算した場所にアクターを戻す
//	GetEnemy()->CollisionStage(pos);
//}
//
//void GameClearScene::EnemyStack(ActorBase* actor)
//{
//	// Enemy 以外は対象外
//	if (actor->GetType() != ActorBase::ACTOR_TYPE::ENEMY) return;
//
//	Enemy* enemy = static_cast<Enemy*>(actor);
//
//	// 座標を取得 (この関数では動かさないので取得するだけでOK)
//	VECTOR pos = enemy->GetPos();
//
//	// カプセルの座標
//	VECTOR capStartPos = VAdd(pos, enemy->GetStartCapsulePos());
//	VECTOR capEndPos = VAdd(pos, enemy->GetEndCapsulePos());
//
//	// カプセルとの当たり判定
//	auto hits = MV1CollCheck_Capsule(
//		exit_,   // ステージのモデルID
//		-1,                     // ステージ全てのポリゴンを指定
//		capStartPos,            // カプセルの上
//		capEndPos,              // カプセルの下
//		enemy->GetCapsuleRadius() // カプセルの半径
//	);
//
//	// 衝突したポリゴン全ての検索
//	if (hits.HitNum > 0)
//	{
//		// **壁に当たっている！**
//		// isEnemyStuck_をtrueにする
//		//isEnemyStuck_ = true;
//	}
//
//	// 検出したポリゴン情報の後始末
//	MV1CollResultPolyDimTerminate(hits);
//}
