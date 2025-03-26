#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <SDL.h>
#include <memory>

void App::LevelSelect() {
  LOG_TRACE("Level Select");

  if (m_CurrentState == State::LEVEL_SELECT) {
    m_TitleBackground->SetVisible(false);
    m_LevelSelectBackground->SetVisible(true);

    for (auto& trap : m_Traps) {
      trap->SetVisible(false);
    }

    // 顯示所有關卡按鈕
    m_Level1Button->SetVisible(true);
    m_Level2Button->SetVisible(true);
    m_Level3Button->SetVisible(true);
    m_Level4Button->SetVisible(true);
    m_Level5Button->SetVisible(true);
    m_BackButton->SetVisible(true);

    // 獲取滑鼠狀態
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    glm::vec2 gameCoords = ConvertToGameCoordinates(mouseX, mouseY);

    // 檢查滑鼠點擊
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      // 檢查每一個關卡按鈕是否被點擊
      // 如果任何一個按鈕處理了點擊（返回true），就不再檢查其他按鈕
      if (m_Level1Button->HandleClick(gameCoords.x, gameCoords.y)) {
        // 按鈕1的點擊已經在回調函數中處理
      } else if (m_Level2Button->HandleClick(gameCoords.x, gameCoords.y)) {
        // 按鈕2的點擊已經在回調函數中處理
      } else if (m_Level3Button->HandleClick(gameCoords.x, gameCoords.y)) {
        // 按鈕3的點擊已經在回調函數中處理
      } else if (m_Level4Button->HandleClick(gameCoords.x, gameCoords.y)) {
        // 按鈕4的點擊已經在回調函數中處理
      } else if (m_Level5Button->HandleClick(gameCoords.x, gameCoords.y)) {
        // 按鈕5的點擊已經在回調函數中處理
      } else if (m_BackButton->HandleClick(gameCoords.x, gameCoords.y)) {
        // 返回按鈕的點擊已經在回調函數中處理
      }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
      m_CurrentState = State::END;
    }
  }
  m_Root.Update();
}