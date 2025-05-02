// Level3.cpp
#include "Level/Level3.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Interface/IGem.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"

bool Level3::Initialize() {
  //   初始化角色 Fireboy
  if (!m_Fireboy) {
    m_Fireboy = std::make_shared<Fireboy>();
    glm::vec2 fireboyInitPos = m_GridSystem.CellToGamePosition(3, 28);
    m_Fireboy->SetPosition(fireboyInitPos);
    m_Fireboy->SetSpawnPoint(fireboyInitPos);
    m_Root.AddChild(m_Fireboy);
  }

  // 初始化角色 Watergirl
  if (!m_Watergirl) {
    m_Watergirl = std::make_shared<Watergirl>();
    glm::vec2 watergirlInitPos = m_GridSystem.CellToGamePosition(3, 24);
    m_Watergirl->SetPosition(watergirlInitPos);
    m_Watergirl->SetSpawnPoint(watergirlInitPos);
    m_Root.AddChild(m_Watergirl);
  }

  // Fireboy 的門
  glm::vec2 fireboyDoorPos = m_GridSystem.CellToGamePosition(30, 5);
  m_Fireboy_Door->SetPosition(fireboyDoorPos);
  m_Fireboy_Door->SetOpen(false);
  m_Fireboy_Door->SetVisible(true);

  // Watergirl 的門
  glm::vec2 watergirlDoorPos = m_GridSystem.CellToGamePosition(34, 5);
  m_Watergirl_Door->SetPosition(watergirlDoorPos);
  m_Watergirl_Door->SetOpen(false);
  m_Watergirl_Door->SetVisible(true);

  std::vector<std::pair<int, int>> waterTrapCoords = {{28, 28}};

  for (const auto &[row, col] : waterTrapCoords) {
    auto waterTrap =
        std::make_shared<LiquidTrap>(CellType::WATER, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    pos.y -= 5.0f;
    waterTrap->SetPosition(pos);
    m_Traps.push_back(waterTrap);
    m_Root.AddChild(waterTrap);
  }

  // 岩漿座標列表
  std::vector<std::pair<int, int>> lavaTrapCoords = {{20, 28}};

  for (const auto &[row, col] : lavaTrapCoords) {
    auto lavaTrap =
        std::make_shared<LiquidTrap>(CellType::LAVA, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    pos.y -= 5.0f;
    lavaTrap->SetPosition(pos);
    m_Traps.push_back(lavaTrap);
    m_Root.AddChild(lavaTrap);
  }

  // 毒池座標列表
  std::vector<std::pair<int, int>> poisionTrapCoords = {{26, 22}};

  for (const auto &[row, col] : poisionTrapCoords) {
    auto poisionTrap =
        std::make_shared<LiquidTrap>(CellType::POISON, SizeType::SMALL);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    pos.y -= 3.0f;
    poisionTrap->SetPosition(pos);
    m_Traps.push_back(poisionTrap);
    m_Root.AddChild(poisionTrap);
  }

  // 火寶石座標 (row, col)
  std::vector<std::pair<int, int>> fireGemCoords = {{20, 26}, {7, 14}, {12, 3}};

  for (const auto &[row, col] : fireGemCoords) {
    auto gem = std::make_shared<Gem>(GemType::FIRE);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }

  // 水寶石座標 (row, col)
  std::vector<std::pair<int, int>> waterGemCoords = {
      {28, 26}, {23, 15}, {2, 5}};

  for (const auto &[row, col] : waterGemCoords) {
    auto gem = std::make_shared<Gem>(GemType::WATER);
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    gem->SetPosition(pos);
    gem->SetInitialPosition(pos);
    m_Gems.push_back(gem);
    m_Root.AddChild(gem);
  }


  // lever
  auto greenLever = std::make_shared<Lever>(
      LeverColor::GREEN, m_GridSystem.CellToGamePosition(10, 19));
  m_Levers.push_back(greenLever);
  m_Root.AddChild(greenLever);

  // platform
  std::vector<std::tuple<int, int, int, int, PlatformColor>> platformInfos = {
      {2, 15, 0, -80, PlatformColor::GREEN}, // lever 控制
      {35, 12, 0, -80, PlatformColor::PINK}  // button 控制
  };

  std::vector<std::shared_ptr<Platform>>
      linkedPlatforms; // 暫存 platform 指標供後續連接

  for (const auto &[row, col, dx, dy, color] : platformInfos) {
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    pos.x += 12.0f;

    auto platform = std::make_shared<Platform>(color, pos, glm::vec2(dx, dy));

    platform->SetPosition(pos);
    m_Platforms.push_back(platform);
    m_Root.AddChild(platform);
    linkedPlatforms.push_back(platform); // 存起來給 lever/button 用
  }

  m_Fireboy->SetPlatforms(m_Platforms);
  m_Watergirl->SetPlatforms(m_Platforms);

  // green lever
  greenLever->linkTrigger(linkedPlatforms[0].get());

  // button
  // Pink Button 1（左下）
  {
    glm::vec2 pos = m_GridSystem.CellToGamePosition(15, 15);
    pos.y += 20;

    auto button1 = std::make_shared<Button>(ButtonColor::PINK, pos);
    button1->linkTrigger(linkedPlatforms[1].get()); // 正確綁 pink platform
    m_Buttons.push_back(button1);
    m_Root.AddChild(button1);
  }

  // Pink Button 2（右上）
  {
    glm::vec2 pos = m_GridSystem.CellToGamePosition(31, 11);
    pos.y += 20;

    auto button2 = std::make_shared<Button>(ButtonColor::PINK, pos);
    button2->linkTrigger(linkedPlatforms[1].get()); // 同樣綁 pink platform
    m_Buttons.push_back(button2);
    m_Root.AddChild(button2);
  }

  // Box
  std::vector<std::pair<int, int>> boxCoords = {{24, 8}};

  std::vector<std::shared_ptr<Box>> boxes;

  for (const auto &[row, col] : boxCoords) {
    auto box = std::make_shared<Box>();
    glm::vec2 pos = m_GridSystem.CellToGamePosition(row, col);
    pos.y -= 12.0f;
    box->SetPosition(pos.x, pos.y);
    boxes.push_back(box);
    m_Root.AddChild(box);
  }

  m_Fireboy->SetBoxes(boxes);
  m_Watergirl->SetBoxes(boxes);

  for (auto& box : boxes) {
    box->OnCollisionWithCharacter(m_Fireboy);
    box->OnCollisionWithCharacter(m_Watergirl);
  }


  return true;
}
