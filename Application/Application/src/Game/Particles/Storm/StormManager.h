#pragma once
//C++
#include <vector>
#include <string>

//Engine
#include <Float2.h>
#include <Float3.h>

/// <summary>
/// エミッターの情報を格納する構造体
/// </summary>
struct EmitterParam {
	Cygnus::Float3 position;	/* エミッターの座標 */
	float angle;				/* エミッターの現在の回転角度 */
	float radius;				/* エミッターの半径 */
	float speed;				/* エミッターの回転速度 */
};

/// <summary>
/// 竜巻の情報を格納する構造体
/// </summary>
struct StormParam {
	std::string name;		/* 竜巻の識別名 */
	Cygnus::Float3* position;	/* 竜巻の座標 */
	std::vector<EmitterParam> emitterPositions; /* 竜巻のエミッターを格納するコンテナ */
};

/// <summary>
/// 竜巻パーティクル管理クラス
/// </summary>
class StormManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンスの取得を行います。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static StormManager* GetInstance();

	/// <summary>
	/// 更新処理を行います。
	/// </summary>
	void Update(float dt);

	/// <summary>
	/// デバッグ処理を行います。
	/// </summary>
	void Debug();

	/// <summary>
	/// 存在確認
	/// </summary>
	/// <param name="name">識別名</param>
	/// <returns>存在するか</returns>
	bool CheckExist(const std::string& name);

	/// <summary>
	/// 作成
	/// </summary>
	/// <param name="name">識別名</param>
	/// <param name="position">対象の座標</param>
	/// <returns>成功したか</returns>
	bool Create(const std::string& name, Cygnus::Float3* position);

	/// <summary>
	/// 削除
	/// </summary>
	/// <param name="name">識別名</param>
	/// <returns>成功したか</returns>
	bool Delete(const std::string& name);

private:
	// =========================================================
	// Member Variables
	// =========================================================

	//竜巻の情報を格納するコンテナ
	std::vector<StormParam> stormParams_;	/* 竜巻の情報を格納するコンテナ */

	///竜巻の制御パラメーター
	const Cygnus::Float2 heightRange_ = Cygnus::Float2(0, 16);	/* 竜巻の発生高さの範囲 */
	const Cygnus::Float2 radiusRange_ = Cygnus::Float2(4, 6);	/* 竜巻の半径の範囲 */
	const Cygnus::Float2 speedRange_ = Cygnus::Float2(20, 40);	/* 竜巻の回転速度の範囲 */
	const int emitterCount_ = 60;						/* 竜巻のエミッターの数 */


	///デバッグ用変数
	std::string debugName_ = "debugStorm";						/* デバッグ用の識別名 */
	Cygnus::Float3 debugPosition_ = Cygnus::Float3(0.0f, 0.0f, 0.0f);	/* デバッグ用の座標 */


};

