#pragma once

#include <BaseScene.h>
#include <Camera.h>
#include <Engine/Collider/CollisionManager.h>
#include <Engine/Texture/PostEffectManager.h>
#include <Engine/Util/TimeManager.h>
#include <Input.h>
#include <LightManager.h>
#include <ModelManager.h>
#include <Object3D.h>
#include <SoundManager.h>
#include <Sprite.h>
#include <SpriteCommon.h>
#include <TextureManager.h>

#include <Engine/DirectX/ShadowMapManager.h>
#include <Engine/Model/SkyBoxManager.h>
#include <Engine/ParticleEffect/ParticleEffectManager.h>
#include <Engine/3D/LineDrawer.h>
#include <Engine/3D/LightCamera.h>
#include <DirectXBase.h>
#include <SRVManager.h>
#include <CommandManager.h>
#include <ImguiWrapper.h>

class OtherSceneBase : public Cygnus::BaseScene
{
public:
	virtual ~OtherSceneBase() = default;

	// 各シーン共通の初期化の枠組み
	void CommonInitialize();

	// 各シーン共通の描画フロー
	void Draw() override;

	void SetTransition(bool isTransition) { isTransition_ = isTransition; } /* シーン遷移するかどうかのフラグをセット */
	bool IsTransition() const { return isTransition_; } /* シーン遷移するかどうかのフラグを取得 */

protected:
	// --- シーンごとに描画中身が違う部分を差し込むための仮想関数 ---
	virtual void DrawShadowNormal(ID3D12GraphicsCommandList* cmd) {}  /* 通常モデルのシャドウパス描画 */
	virtual void DrawShadowSkinned(ID3D12GraphicsCommandList* cmd) {} /* スキニングモデルのシャドウパス描画 */
	virtual void DrawMainScene3D() {}                                 /* メインシーンパスの3D描画 */
	virtual void DrawBackBuffer3D() {}                                /* バックバッファ直接パスの3D描画 */
	virtual void DrawUI() = 0;                                        /* スプライト・UIの描画 */
	virtual void CheckSceneTransition() = 0;                          /* シーンチェンジ・遷移処理 */
	virtual void Debug() {}                                           /* デバッグ表示用 */

protected:
	// ----- System -----
	std::unique_ptr<Cygnus::Camera> camera_ = nullptr;             /* 3Dカメラクラス */
	std::unique_ptr<Cygnus::SpriteCommon> spriteCommon_ = nullptr; /* スプライト共通処理 */
	Cygnus::Input* input_ = nullptr;                               /* 入力管理クラス */
	Cygnus::LightManager* lightManager_ = nullptr;                 /* 各ライト管理クラス */
	bool isTransition_ = false;									   /* シーン遷移するかどうかのフラグ */

	// ----- Objects -----
	std::unique_ptr<Cygnus::Object3D> objectGround_;	           /* 地面オブジェクト（仮） */

	// ----- Others -----
	uint32_t shadowMapHandle_;                                     /* シャドウマップテクスチャ */
	std::unique_ptr<Cygnus::PostEffectManager> postEffectManager_; /* ポストエフェクト管理クラス */
};