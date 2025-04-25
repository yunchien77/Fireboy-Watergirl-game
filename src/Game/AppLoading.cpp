#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <SDL.h>
#include <iostream>
#include <memory>
#include <string>

void App::Loading(float deltaTime) {
  LOG_TRACE("Loading Animation");

  // Initialize loading backgrounds if they don't exist
  if (!m_LoadingBackground1) {
    m_LoadingBackground1 = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/loading1.png", 10);
    m_Root.AddChild(m_LoadingBackground1);
  }
  if (!m_LoadingBackground2) {
    m_LoadingBackground2 = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/loading2.png", 10);
    m_Root.AddChild(m_LoadingBackground2);
  }
  if (!m_LoadingBackground3) {
    m_LoadingBackground3 = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/loading3.png", 10);
    m_Root.AddChild(m_LoadingBackground3);
  }

  // Hide all loading backgrounds initially
  m_LoadingBackground1->SetVisible(false);
  m_LoadingBackground2->SetVisible(false);
  m_LoadingBackground3->SetVisible(false);

  // Show the current frame
  switch (m_LoadingFrameIndex) {
  case 0:
    m_LoadingBackground1->SetVisible(true);
    // std::cout << "Loading frame 1" << std::endl;
    break;
  case 1:
    m_LoadingBackground1->SetVisible(false);
    m_LoadingBackground2->SetVisible(true);
    // std::cout << "Loading frame 2" << std::endl;
    break;
  case 2:
    m_LoadingBackground2->SetVisible(false);
    m_LoadingBackground3->SetVisible(true);
    // std::cout << "Loading frame 3" << std::endl;
    break;
  }

  // Update animation timer
  m_LoadingAnimationTimer += deltaTime;
  if (m_LoadingAnimationTimer >= m_LoadingFrameDuration) {
    m_LoadingAnimationTimer = 0.0f;
    m_LoadingFrameIndex++;

    // When we've shown all three frames, transition to the next state
    if (m_LoadingFrameIndex >= 3) {
      // Reset for next time
      m_LoadingFrameIndex = 0;

      // Hide all loading backgrounds
      m_LoadingBackground1->SetVisible(false);
      m_LoadingBackground2->SetVisible(false);
      m_LoadingBackground3->SetVisible(false);

      // Transition to the next state (usually LEVEL_SELECT)
      m_CurrentState = State::LEVEL_SELECT;

      // If transitioning to level select, show the appropriate elements
      if (m_CurrentState == State::LEVEL_SELECT) {
        m_LevelSelectBackground->SetVisible(true);
        m_Level1Button->SetVisible(true);
        m_Level2Button->SetVisible(true);
        m_Level3Button->SetVisible(true);
        m_Level4Button->SetVisible(true);
        m_Level5Button->SetVisible(true);
        m_BackButton->SetVisible(true);
      }
    }
  }

  // Handle escape key
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}