#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <SDL.h>
#include <memory>

// Add this function to your App class in App.hpp
void App::ResetGame() {
  // Reset game state variables
  m_CurrentState = State::GAME_PLAY;
  m_IsGridLoaded = false;

  // Hide win screen elements
  m_GameWinBackground->SetVisible(false);
  m_ContinueButton->SetVisible(false);

  // Clear any existing characters (to avoid duplicates)
  if (m_Fireboy) {
    m_Fireboy->SetVisible(false);
    m_Fireboy.reset(); // This will free the shared_ptr
  }

  if (m_Watergirl) {
    m_Watergirl->SetVisible(false);
    m_Watergirl.reset(); // This will free the shared_ptr
  }

  // Reset doors
  m_Fireboy_Door->SetOpen(false);
  m_Fireboy_Door->SetCharacterAtDoor(false);
  m_Fireboy_Door->SetVisible(false);

  m_Watergirl_Door->SetOpen(false);
  m_Watergirl_Door->SetCharacterAtDoor(false);
  m_Watergirl_Door->SetVisible(false);

  for (auto &trap : m_Traps) {
    trap->SetVisible(false);
  }

  for (auto &gem : m_Gems) {
    gem->SetVisible(false);
  }
}