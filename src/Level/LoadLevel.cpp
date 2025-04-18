#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Level/LevelFactory.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Gate.hpp"
#include "Object/GridSystem.hpp"
#include <Mechanism/Gem.hpp>
#include <Mechanism/LiquidTrap.hpp>

// 載入地圖網格，並初始化 GridSystem
bool App::LoadLevelGrid(int levelNumber) {
  std::cout << "Loading level " << levelNumber << " grid..." << std::endl;
  std::cout << "current state: " << static_cast<int>(m_CurrentState)
            << std::endl;

  // 清理現有資源
  for (auto &trap : m_Traps) {
    m_Root.RemoveChild(trap);
  }
  m_Traps.clear();

  // 創建並初始化關卡
  auto level = LevelFactory::CreateLevel(
      levelNumber, m_GridSystem, m_Root, m_Gems, m_Triggers, m_Buttons, m_Traps,
      m_Fireboy, m_Watergirl, m_Fireboy_Door, m_Watergirl_Door);

  if (!level) {
    return false;
  }

  // 加載網格
  if (!level->LoadGrid(levelNumber)) {
    return false;
  }

  m_IsGridLoaded = true;

  // 初始化關卡
  if (!level->Initialize()) {
    return false;
  }

  for (int y = 0; y < m_GridSystem.GetGridHeight(); ++y) {
    for (int x = 0; x < m_GridSystem.GetGridWidth(); ++x) {
      std::cout << static_cast<int>(m_GridSystem.GetCell(x, y)) << " ";
    }
    std::cout << std::endl;
  }

  return true;
}