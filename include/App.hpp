#ifndef APP_HPP
#define APP_HPP

#include "Character/Character.hpp"
#include "Interface/IGem.hpp"
#include "Mechanism/Door.hpp"
#include "Object/BackgroundImage.hpp"
#include "Object/GridSystem.hpp"
#include "Object/Option.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp" // IWYU pragma: export
#include <Interface/ITrap.hpp>
#include <memory>
#include <string>


class App {
public:
  enum class State {
    START,        // 遊戲封面
    LEVEL_SELECT, // 關卡選擇
    GAME_PLAY,    // 遊戲開始
    GAME_WIN,     // 遊戲贏
    GAME_OVER,    // 遊戲輸
    END,
  };

  App();

  State GetCurrentState() const { return m_CurrentState; }

  void Start();

  void LevelSelect();

  void GamePlay();

  void GameOver();

  void GameWin();

  void End(); // NOLINT(readability-convert-member-functions-to-static)

  void ResetGame();

  // 載入關卡的網格檔案
  bool LoadLevelGrid(int levelNumber);

  // 取得格子系統
  GridSystem &GetGridSystem() { return m_GridSystem; }

  // 檢查角色碰撞
  bool CheckCharacterCollision(const glm::vec2 &position, glm::vec2 size,
                               bool isFireboy, int deltaX = 0);

  bool LoadLevel(int levelNumber);

  void ResetGameLevel();

  std::shared_ptr<Character> GetFireboy() { return m_Fireboy; }
  std::shared_ptr<Character> GetWatergirl() { return m_Watergirl; }

  // 檢查是否達成勝利條件
  bool CheckWinCondition();

  // 檢查角色與門的互動
  void CheckCharacterDoorInteraction();

private:
  void ValidTask();

private:
  State m_CurrentState = State::START;
  int m_CurrentLevel = 1;
  bool m_IsLevelCompleted = false;

  std::shared_ptr<BackgroundImage> m_TitleBackground; // 遊戲封面背景
  std::shared_ptr<BackgroundImage> m_LevelSelectBackground; // 關卡選擇背景

  std::shared_ptr<Option> m_Level1Button; // 第一關按鈕
  std::shared_ptr<Option> m_Level2Button; // 第二關按鈕
  std::shared_ptr<Option> m_Level3Button; // 第三關按鈕
  std::shared_ptr<Option> m_Level4Button; // 第四關按鈕
  std::shared_ptr<Option> m_Level5Button; // 第五關按鈕
  std::shared_ptr<Option> m_BackButton;   // 返回按鈕

  std::shared_ptr<BackgroundImage> m_Level1Background; // 第一關背景
  std::shared_ptr<BackgroundImage> m_Level2Background; // 第二關背景
  std::shared_ptr<BackgroundImage> m_Level3Background; // 第三關背景
  std::shared_ptr<BackgroundImage> m_Level4Background; // 第四關背景
  std::shared_ptr<BackgroundImage> m_Level5Background; // 第五關背景

  std::shared_ptr<Character> m_Fireboy;
  std::shared_ptr<Character> m_Watergirl;

  std::shared_ptr<Door> m_Fireboy_Door;
  std::shared_ptr<Door> m_Watergirl_Door;

  std::vector<std::shared_ptr<ITrap>> m_Traps;
  std::vector<std::shared_ptr<IGem>> m_Gems;

  std::shared_ptr<BackgroundImage> m_GameWinBackground;
  std::shared_ptr<BackgroundImage> m_GameOverBackground;

  std::shared_ptr<Option> m_EndButton;      // 結束按鈕
  std::shared_ptr<Option> m_MenuButton;     // 返回主選單按鈕
  std::shared_ptr<Option> m_RetryButton;    // 重新開始按鈕
  std::shared_ptr<Option> m_ResumeButton;   // 繼續按鈕
  std::shared_ptr<Option> m_ContinueButton; // 切換道關卡選擇按鈕

  // 網格系統
  GridSystem m_GridSystem;
  bool m_IsGridLoaded = false;

  Util::Renderer m_Root;

  // 轉換滑鼠座標從窗口座標到游戲座標
  glm::vec2 ConvertToGameCoordinates(int screenX, int screenY) {
    int windowWidth, windowHeight;
    SDL_Window *window = SDL_GL_GetCurrentWindow();
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    return glm::vec2(screenX - (windowWidth / 2), // 中心化X座標
                     (windowHeight / 2) - screenY // 中心化並翻轉Y座標
    );
  }
};

#endif