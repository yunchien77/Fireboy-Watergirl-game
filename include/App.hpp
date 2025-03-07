#ifndef APP_HPP
#define APP_HPP

#include "BackgroundImage.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp" // IWYU pragma: export
#include <memory>

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

private:
  void ValidTask();

private:
  State m_CurrentState = State::START;
  int m_CurrentLevel = 0;          // 遊戲關卡
  bool m_IsLevelCompleted = false; // 關卡結束與否

  std::shared_ptr<BackgroundImage> m_TitleBackground;
  std::shared_ptr<BackgroundImage> m_LevelBackground;

  Util::Renderer m_Root;
};

#endif
