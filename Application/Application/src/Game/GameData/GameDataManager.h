#pragma once

class GameDataManager
{
public:
    static GameDataManager* GetInstance()
    {
        static GameDataManager instance;
        return &instance;
    }

    // 選択したステージのセッターとゲッター
    void SetTargetStage(int stageId) { targetStage_ = stageId; }
    int GetTargetStage() const { return targetStage_; }

private:
    GameDataManager() = default;
    ~GameDataManager() = default;

    int targetStage_ = 0; // デフォルトのステージ
};
