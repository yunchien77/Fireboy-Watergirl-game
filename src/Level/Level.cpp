// Level.cpp
#include "Level/Level.hpp"
#include "Interface/IGem.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Fan.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"
#include "Util/Logger.hpp"
#include <iostream>

Level::Level(GridSystem &gridSystem, Util::Renderer &root,
             std::vector<std::shared_ptr<IGem>> &gems,
             std::vector<std::shared_ptr<Gate>> &gates,
             std::vector<std::shared_ptr<Button>> &buttons,
             std::vector<std::shared_ptr<ITrap>> &traps,
             std::vector<std::shared_ptr<Lever>> &levers,
             std::vector<std::shared_ptr<Platform>> &platforms,
             std::vector<std::shared_ptr<Box>> &boxes,
             std::vector<std::shared_ptr<Fan>> &fans,
             std::shared_ptr<Character> &fireboy,
             std::shared_ptr<Character> &watergirl,
             std::shared_ptr<Door> &fireboyDoor,
             std::shared_ptr<Door> &watergirlDoor)
    : m_GridSystem(gridSystem), m_Root(root), m_Gems(gems), m_Triggers(gates),
      m_Buttons(buttons), m_Traps(traps), m_Levers(levers),
      m_Platforms(platforms), m_Boxes(boxes), m_Fans(fans), m_Fireboy(fireboy),
      m_Watergirl(watergirl), m_Fireboy_Door(fireboyDoor),
      m_Watergirl_Door(watergirlDoor) {}

void Level::Cleanup() {
  for (auto &gem : m_Gems)
    m_Root.RemoveChild(gem);
  m_Gems.clear();
  for (auto &gate : m_Triggers)
    m_Root.RemoveChild(gate);
  m_Triggers.clear();
  for (auto &button : m_Buttons)
    m_Root.RemoveChild(button);
  m_Buttons.clear();
  for (auto &lever : m_Levers)
    m_Root.RemoveChild(lever);
  m_Levers.clear();
  for (auto &platform : m_Platforms)
    m_Root.RemoveChild(platform);
  m_Platforms.clear();
  for (auto &box : m_Boxes)
    m_Root.RemoveChild(box);
  m_Boxes.clear();
  for (auto &fan : m_Fans)
    m_Root.RemoveChild(fan);
  m_Fans.clear();
  for (auto &trap : m_Traps)
    m_Root.RemoveChild(trap);
  m_Traps.clear();
}

bool Level::LoadGrid(int levelNumber) {
  std::string gridFilePath =
      RESOURCE_DIR "/map/level" + std::to_string(levelNumber) + "_grid.txt";
  bool success = m_GridSystem.LoadFromFile(gridFilePath);
  if (success)
    LOG_INFO("Successfully loaded grid for level {}", levelNumber);
  return success;
}

void Level::InitCharacter(std::shared_ptr<Character> &character,
                          const glm::ivec2 &gridPos) {
  glm::vec2 pos = m_GridSystem.CellToGamePosition(gridPos.x, gridPos.y);
  character->SetPosition(pos);
  character->SetSpawnPoint(pos);
  m_Root.AddChild(character);
}

void Level::InitDoor(std::shared_ptr<Door> &door, const glm::ivec2 &gridPos) {
  glm::vec2 pos = m_GridSystem.CellToGamePosition(gridPos.x, gridPos.y);
  door->SetPosition(pos);
  door->SetOpen(false);
  door->SetVisible(true);
}

void Level::InitGems(GemType type, const std::vector<glm::ivec2> &coords) {
  for (const auto &coord : coords) {
    auto gem = std::make_shared<Gem>(type);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(coord.x, coord.y);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }
}

void Level::InitLiquidTraps(CellType type, SizeType size,
                            const std::vector<glm::ivec2> &coords) {
  for (const auto &coord : coords) {
    int row = coord.x;
    int col = coord.y;
    auto trap = std::make_shared<LiquidTrap>(type, size);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    trap->SetPosition(pos);
    m_Traps.push_back(trap);
    m_Root.AddChild(trap);
  }
}

void Level::InitBoxes(const std::vector<glm::ivec2> &coords) {
  for (const auto &coord : coords) {
    int row = coord.x;
    int col = coord.y;
    auto box = std::make_shared<Box>();
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    box->SetPosition(pos);
    box->SetInitialPosition(pos);
    box->SetGridSystem(&m_GridSystem);
    m_Boxes.push_back(box);
    m_Root.AddChild(box);
  }
}

void Level::InitFans(
    const std::vector<std::tuple<int, int, float, FanColor>> &fanInfos) {
  for (const auto &[row, col, strength, color] : fanInfos) {
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    auto fan = std::make_shared<Fan>(pos, strength, color);
    fan->SetPosition(pos);
    fan->SetInitialPosition(pos);
    m_Fans.push_back(fan);
    m_Root.AddChild(fan);
  }
}

std::shared_ptr<Gate> Level::InitGate(GateColor color,
                                      const glm::ivec2 &cellPos) {
  glm::vec2 pos = m_GridSystem.CellToGamePosition(cellPos.x, cellPos.y);
  pos.y -= 15.0f;
  auto gate = std::make_shared<Gate>(color, pos);
  m_Triggers.push_back(gate);
  m_Root.AddChild(gate);
  return gate;
}

std::shared_ptr<Button> Level::InitButton(ButtonColor color,
                                          const glm::ivec2 &cellPos,
                                          ITriggerable *target) {
  glm::vec2 pos = m_GridSystem.CellToGamePosition(cellPos.x, cellPos.y);
  pos.y += 20;
  auto button = std::make_shared<Button>(color, pos);
  button->SetInitialPosition(pos);
  button->LinkTrigger(target);
  m_Buttons.push_back(button);
  m_Root.AddChild(button);
  return button;
}

std::shared_ptr<Lever> Level::InitLever(LeverColor color,
                                        const glm::ivec2 &cellPos,
                                        ITriggerable *target) {
  glm::vec2 pos = m_GridSystem.CellToGamePosition(cellPos.x, cellPos.y);
  auto lever = std::make_shared<Lever>(color, pos);
  lever->SetInitialState(pos, false);
  lever->linkTrigger(target);
  m_Levers.push_back(lever);
  m_Root.AddChild(lever);
  return lever;
}

std::shared_ptr<Platform> Level::InitPlatform(PlatformColor color,
                                              const glm::ivec2 &cellPos,
                                              const glm::vec2 &offset) {
  glm::vec2 pos = m_GridSystem.CellToGamePosition(cellPos.x, cellPos.y);
  pos.x += 12.0f;
  auto platform = std::make_shared<Platform>(color, pos, offset);
  platform->SetPosition(pos);
  m_Platforms.push_back(platform);
  m_Root.AddChild(platform);
  return platform;
}