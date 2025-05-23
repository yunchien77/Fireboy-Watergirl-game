#ifndef APP_HPP
#define APP_HPP

#include "Character/Character.hpp"
#include "Game/CollisionManager.hpp"
#include "Game/GameMechanicsManager.hpp"
#include "Game/InputHandler.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Fan.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"
#include "Object/BackgroundImage.hpp"
#include "Object/GridSystem.hpp"
#include "Object/Option.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp"

#include <vector>

class App {
public:
  enum class State {
    START,        // 遊戲封面
    LEVEL_SELECT, // 關卡選擇
    GAME_PLAY,    // 遊戲開始
    GAME_WIN,     // 遊戲贏
    GAME_PAUSE,   // 遊戲暫停
    LOADING,      // 載入中
    END,          // 遊戲結束
  };

  App();

  State GetCurrentState() const { return m_CurrentState; }

  //------------------- 遊戲狀態控制函數 -------------------
  void Start();
  void LevelSelect();
  void GamePlay();
  void GamePause();
  void GameWin();
  void Loading(float deltaTime);
  void End(); // NOLINT(readability-convert-member-functions-to-static)

  // 遊戲物件初始化
  void InitializeBackgrounds();
  void InitializeLevelBackgrounds();
  void InitializeLevelButtons();
  std::shared_ptr<Option> CreateLevelButton(const std::string &imagePath,
                                            const glm::vec2 &position,
                                            int levelNumber, bool interactable);
  void ShowLevelBackground(int levelNumber);
  void SetLevelSelectVisible(bool visible);
  void InitializeDoors();
  void InitializeGameUI();
  void HideAllLevelBackgrounds();
  void InitializePauseMenu();
  void SetPauseMenuVisible(bool visible);

  //------------------- 關卡與遊戲管理函數 -------------------
  // 重設遊戲狀態
  void ResetGame();

  // 載入關卡網格檔案
  bool LoadLevelGrid(int levelNumber);

  // 載入關卡資源與設定
  bool LoadLevel(int levelNumber);

  // 重設當前關卡狀態
  void ResetGameLevel();

  //------------------- 遊戲邏輯相關函數 -------------------
  // 檢查角色碰撞
  bool CheckCharacterCollision(const glm::vec2 &position, glm::vec2 size,
                               bool isFireboy, int deltaX = 0);

  // 檢查角色與箱子的碰撞
  bool CheckBoxCollision(std::shared_ptr<Character> character);

  // 檢查遊戲勝利條件
  bool CheckWinCondition();

  // 檢查寶石收集狀態
  bool GetGemCollectionStatus();

  void InitializeManagers();
  void HandleAllCollisions();

  //------------------- game win -------------------
  void InitializeWinUI(bool gemsCollected);
  void ShowWinBackground();
  void ShowCharacterIndicator();
  void ShowGemCollectionIndicator(bool gemsCollected);
  void ShowLevelResult(bool gemsCollected);
  void UpdateCurrentLevelButton(bool gemsCollected);
  std::shared_ptr<Option> GetCurrentLevelButton();
  void HandleWinScreenInput();
  void HandleContinueButtonClick(const glm::vec2 &gameCoords);
  void UnlockNextLevel();
  void HandleExitConditions();

  //------------------- getter -------------------
  // 取得網格系統參考
  GridSystem &GetGridSystem() { return m_GridSystem; }

  // 取得角色指標
  std::shared_ptr<Character> GetFireboy() { return m_Fireboy; }
  std::shared_ptr<Character> GetWatergirl() { return m_Watergirl; }

private:
  bool m_IsInvincibleMode = false;
  //------------------- 輔助函數 -------------------
  // 轉換滑鼠座標從窗口座標到游戲座標
  glm::vec2 ConvertToGameCoordinates(int screenX, int screenY) {
    int windowWidth, windowHeight;
    SDL_Window *window = SDL_GL_GetCurrentWindow();
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    return glm::vec2(screenX - (windowWidth / 2), // 中心化X座標
                     (windowHeight / 2) - screenY // 中心化並翻轉Y座標
    );
  }

  // 執行必要的任務
  void ValidTask();

  //------------------- 遊戲狀態變數 -------------------
  State m_CurrentState = State::START; // 當前遊戲狀態
  int m_CurrentLevel = 1;              // 當前關卡編號
  int unlockedLevel = 1;               // 已解鎖的最高關卡

  //------------------- 網格與渲染系統 -------------------
  GridSystem m_GridSystem;     // 網格系統
  bool m_IsGridLoaded = false; // 網格是否已載入
  Util::Renderer m_Root;       // 根渲染器

  //------------------- UI元素 -------------------
  // 遊戲封面與選單背景
  std::shared_ptr<BackgroundImage> m_TitleBackground;
  std::shared_ptr<BackgroundImage> m_LevelSelectBackground;
  std::shared_ptr<BackgroundImage> m_GameWinBackground;
  std::shared_ptr<BackgroundImage> m_GamePausedBackground;

  // 關卡背景
  std::shared_ptr<BackgroundImage> m_Level1Background;
  std::shared_ptr<BackgroundImage> m_Level2Background;
  std::shared_ptr<BackgroundImage> m_Level3Background;
  std::shared_ptr<BackgroundImage> m_Level4Background;
  std::shared_ptr<BackgroundImage> m_Level5Background;

  // 關卡選擇按鈕
  std::shared_ptr<Option> m_Level1Button;
  std::shared_ptr<Option> m_Level2Button;
  std::shared_ptr<Option> m_Level3Button;
  std::shared_ptr<Option> m_Level4Button;
  std::shared_ptr<Option> m_Level5Button;
  std::shared_ptr<Option> m_BackButton;

  // 遊戲控制按鈕
  std::shared_ptr<Option> m_pauseButton;    // 暫停按鈕
  std::shared_ptr<Option> m_EndButton;      // 結束按鈕
  std::shared_ptr<Option> m_RetryButton;    // 重新開始按鈕
  std::shared_ptr<Option> m_ResumeButton;   // 繼續按鈕
  std::shared_ptr<Option> m_ContinueButton; // 切換到關卡選擇按鈕

  // 遊戲結果UI元素
  std::shared_ptr<BackgroundImage> m_LevelResult;
  std::shared_ptr<BackgroundImage> m_GemCollectedIndicator;
  std::shared_ptr<BackgroundImage> m_CharacterIndicator;

  //------------------- 角色與遊戲物件 -------------------
  // 遊戲角色
  std::shared_ptr<Character> m_Fireboy;
  std::shared_ptr<Character> m_Watergirl;

  // 出口門
  std::shared_ptr<Door> m_Fireboy_Door;
  std::shared_ptr<Door> m_Watergirl_Door;

  // 遊戲機關與物件集合
  std::vector<std::shared_ptr<LiquidTrap>> m_Traps;
  std::vector<std::shared_ptr<Gem>> m_Gems;
  std::vector<std::shared_ptr<Button>> m_Buttons;
  std::vector<std::shared_ptr<Gate>> m_Gates;
  std::vector<std::shared_ptr<Platform>> m_Platforms;
  std::vector<std::shared_ptr<Lever>> m_Levers;
  std::vector<std::shared_ptr<Box>> m_Boxes;
  std::vector<std::shared_ptr<Fan>> m_Fans;

  //------------------- 載入畫面資源 -------------------
  // 載入畫面動畫資源
  std::shared_ptr<BackgroundImage> m_LoadingBackground1;
  std::shared_ptr<BackgroundImage> m_LoadingBackground2;
  std::shared_ptr<BackgroundImage> m_LoadingBackground3;
  std::shared_ptr<BackgroundImage> m_LoadingBackground5;

  // 載入畫面動畫控制
  int m_LoadingFrameIndex = 0;
  float m_LoadingAnimationTimer = 0.0f;
  float m_LoadingFrameDuration = 0.5f; // 每幀0.5秒

  std::unique_ptr<CollisionManager> m_CollisionManager;
  std::unique_ptr<GameMechanicsManager> m_MechanicsManager;
  std::unique_ptr<InputHandler> m_InputHandler;
};

#endif // APP_HPP