#ifndef APP_HPP
#define APP_HPP

#include "BackgroundImage.hpp"
#include "Button.hpp"
#include "Character/Character.hpp"
#include "GridSystem.hpp"
#include "LevelData.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp" // IWYU pragma: export
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

  // 載入關卡的網格檔案
  bool LoadLevelGrid(int levelNumber);

  // 取得格子系統
  GridSystem &GetGridSystem() { return m_GridSystem; }

  // 檢查角色碰撞
  bool CheckCharacterCollision(const glm::vec2 &position, glm::vec2 size,
                               bool isFireboy, int deltaX = 0);

  bool LoadLevel(int levelNumber);

private:
  void ValidTask();

private:
  State m_CurrentState = State::START;
  int m_CurrentLevel = 1;
  bool m_IsLevelCompleted = false;

  std::shared_ptr<BackgroundImage> m_TitleBackground; // 遊戲封面背景
  std::shared_ptr<BackgroundImage> m_LevelSelectBackground; // 關卡選擇背景

  std::shared_ptr<Button> m_Level1Button; // 第一關按鈕
  std::shared_ptr<Button> m_Level2Button; // 第二關按鈕
  std::shared_ptr<Button> m_Level3Button; // 第三關按鈕
  std::shared_ptr<Button> m_Level4Button; // 第四關按鈕
  std::shared_ptr<Button> m_Level5Button; // 第五關按鈕
  std::shared_ptr<Button> m_BackButton;   // 返回按鈕

  std::shared_ptr<BackgroundImage> m_Level1Background; // 第一關背景
  std::shared_ptr<BackgroundImage> m_Level2Background; // 第二關背景
  std::shared_ptr<BackgroundImage> m_Level3Background; // 第三關背景
  std::shared_ptr<BackgroundImage> m_Level4Background; // 第四關背景
  std::shared_ptr<BackgroundImage> m_Level5Background; // 第五關背景

  std::shared_ptr<Character> m_Fireboy;
  std::shared_ptr<Character> m_Watergirl;

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