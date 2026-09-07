#include "DangerWarningUI.h"

// Engine
#include <TextureManager.h>
#include <TimeManager.h>
#include <Easing.h>

// Application
#include <src/Game/Objects/FlyingObject/FlyingObjectManager.h>
#include <src/Game/Objects/Tether/Tether.h>
#include <src/Game/Objects/Player/Player.h>
#include <src/Game/Util/Utility.h>

void DangerWarningUI::Initialize(Cygnus::SpriteCommon* spriteCommon) { 
	uint32_t texWarning = Cygnus::TextureManager::Load("warning.png"); 

	items_.resize(kMaxWarnings);

	// スプライトのプール初期化
	items_.resize(kMaxWarnings);

	for (size_t i = 0; i < kMaxWarnings; ++i) {
		items_[i].sprite = std::make_unique<Cygnus::Sprite>();
		items_[i].sprite->Initialize(spriteCommon, texWarning);
		items_[i].sprite->SetAnchorPoint({0.5f, 0.5f});
		items_[i].sprite->SetSize({64.0f, 64.0f});
		items_[i].alphaTimer = 0.0f;
		items_[i].blinkTimer = 0.0f;
		items_[i].isTargeted = false;
	}
}

void DangerWarningUI::Update(Player* player, Tether* tether, FlyingObjectManager* flyingObjectManager) {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 全項目のターゲットフラグをリセット
	for (auto& item : items_) {
		item.isTargeted = false;
	}

	if (player && tether && flyingObjectManager) {
		const auto& objects = flyingObjectManager->GetObjects();
		const auto& tetherNodes = tether->GetNodes();
		Cygnus::Float3 playerPos = player->GetTranslate();

		size_t spriteIndex = 0;

		for (const auto& obj : objects) {
			if (spriteIndex >= kMaxWarnings) break;
			if (obj->GetCategory() != ObjectCategory::Meteor || obj->IsDead()) continue;

			Cygnus::Float3 meteorPos = obj->GetTranslate();
			bool isDangerous = false;

			/* プレイヤーとの距離判定 */
			float distToPlayer = Cygnus::Float3::Length(meteorPos - playerPos);
			if (distToPlayer <= kDangerDistance) {
				isDangerous = true;
			}

			/* 命綱（各ノード）との距離判定 */
			if (!isDangerous) {
				for (const auto& node : tetherNodes) {
					float distToNode = Cygnus::Float3::Length(meteorPos - node.position);
					if (distToNode <= kDangerDistance) {
						isDangerous = true;
						break;
					}
				}
			}

			// 危険範囲に入っている場合、目標座標をセットしてフラグを立てる
			if (isDangerous) {
				Cygnus::Float3 screenPos = Utility::WorldToScreen(meteorPos);
				items_[spriteIndex].position = {screenPos.x, screenPos.y};
				items_[spriteIndex].isTargeted = true;
				spriteIndex++;
			}
		}

		// アルファ値のアニメーション計算とスプライト更新
		for (auto& item : items_) {
			// 危険状態ならフェードイン、安全状態ならフェードアウト
			if (item.isTargeted) {
				item.alphaTimer = (std::min)(item.alphaTimer + dt, kFadeDuration);
				item.blinkTimer += dt * kBlinkSpeed;
			} else {
				item.alphaTimer = (std::max)(item.alphaTimer - dt, 0.0f);
				// フェードアウト完了時に点滅タイマーをリセット
				if (item.alphaTimer <= 0.0f) {
					item.blinkTimer = 0.0f;
				}
			}

			// アルファ用タイマーが0より大きい場合のみ描画計算
			if (item.alphaTimer > 0.0f) {
				// サイン波で点滅アルファを計算
				float sinWave = (std::sinf(item.blinkTimer) + 1.0f) * 0.5f;
				float baseAlpha = kMinAlpha + sinWave * (kMaxAlpha - kMinAlpha);

				// 出現・消滅時のフェード率を掛ける
				float fadeFactor = Cygnus::Easing::EaseOutQuad(item.alphaTimer / kFadeDuration);
				float currentAlpha = baseAlpha * fadeFactor;

				item.sprite->SetPosition(item.position);
				item.sprite->SetColor({1.0f, 1.0f, 1.0f, currentAlpha});
				item.sprite->Update();
			}
		}
	}
}

void DangerWarningUI::Draw() {
	for (auto& item : items_) {
		// タイマーが0より大きいときのみ描画
		if (item.alphaTimer > 0) {
			item.sprite->Draw();
		}
	}
}