// Level.cpp
#include "Level/Level.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Interface/IGem.hpp"
#include "Level/LevelFactory.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Util/Logger.hpp"
#include <iostream>

Level::Level(GridSystem &gridSystem,
             Util::Renderer &root, // 使用Renderer
             std::vector<std::shared_ptr<IGem>> &gems,
             std::vector<std::shared_ptr<Gate>> &triggers,
             std::vector<std::shared_ptr<Button>> &buttons,
             std::vector<std::shared_ptr<ITrap>> &traps,
             std::shared_ptr<Character> &fireboy,
             std::shared_ptr<Character> &watergirl,
             std::shared_ptr<Door> &fireboyDoor,
             std::shared_ptr<Door> &watergirlDoor)
    : m_GridSystem(gridSystem), m_Root(root), m_Gems(gems),
      m_Triggers(triggers), m_Buttons(buttons), m_Traps(traps),
      m_Fireboy(fireboy), m_Watergirl(watergirl), m_Fireboy_Door(fireboyDoor),
      m_Watergirl_Door(watergirlDoor) {}

void Level::Cleanup() {
  for (auto &gem : m_Gems) {
    m_Root.RemoveChild(gem);
  }
  m_Gems.clear();

  for (auto &gate : m_Triggers) {
    m_Root.RemoveChild(gate);
  }
  m_Triggers.clear();

  for (auto &button : m_Buttons) {
    m_Root.RemoveChild(button);
  }
  m_Buttons.clear();
}

bool Level::LoadGrid(int levelNumber) {
  std::string gridFilePath =
      RESOURCE_DIR "/map/level" + std::to_string(levelNumber) + "_grid.txt";

  bool success = m_GridSystem.LoadFromFile(gridFilePath);
  if (success) {
    LOG_INFO("Successfully loaded grid for level {}", levelNumber);
  }
  return success;
}