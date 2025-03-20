#include "GridSystem.hpp"
#include "Util/Logger.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

// (Debug)
std::string CellTypeToString(CellType type) {
  switch (type) {
  case CellType::EMPTY:
    return "EMPTY";
  case CellType::FLOOR:
    return "FLOOR";
  case CellType::WALL:
    return "WALL";
  case CellType::DOOR_FIRE:
    return "DOOR_FIRE";
  case CellType::DOOR_WATER:
    return "DOOR_WATER";
  case CellType::GEM_FIRE:
    return "GEM_FIRE";
  case CellType::GEM_WATER:
    return "GEM_WATER";
  case CellType::GEM_GREEN:
    return "GEM_GREEN";
  case CellType::LAVA:
    return "LAVA";
  case CellType::WATER:
    return "WATER";
  case CellType::POISON:
    return "POISON";
  case CellType::BUTTON:
    return "BUTTON";
  case CellType::LEVER:
    return "LEVER";
  case CellType::PLATFORM:
    return "PLATFORM";
  case CellType::FAN:
    return "FAN";
  case CellType::BOX:
    return "BOX";
  case CellType::STONE:
    return "STONE";
  default:
    return "UNKNOWN";
  }
}

GridSystem::GridSystem()
    : m_CellSize(25), m_BackgroundWidth(975), m_BackgroundHeight(725) {

  m_GridWidth = m_BackgroundWidth / m_CellSize;
  m_GridHeight = m_BackgroundHeight / m_CellSize;

  m_Grid.resize(m_GridHeight,
                std::vector<CellType>(m_GridWidth, CellType::EMPTY));

  LOG_INFO("GridSystem initialized with empty grid: {}x{}", m_GridWidth,
           m_GridHeight);
}

GridSystem::GridSystem(const std::vector<std::vector<CellType>> &levelData,
                       int backgroundWidth, int backgroundHeight, int cellSize)
    : m_Grid(levelData), m_CellSize(cellSize),
      m_BackgroundWidth(backgroundWidth), m_BackgroundHeight(backgroundHeight) {

  m_GridWidth = m_Grid[0].size();
  m_GridHeight = m_Grid.size();

  LOG_INFO("GridSystem initialized with level data: {}x{}", m_GridWidth,
           m_GridHeight);
}

CellType GridSystem::GetCell(int x, int y) const {
  if (IsValidGridPosition(x, y)) {
    return m_Grid[y][x];
  }
  return CellType::EMPTY;
}

glm::vec2 GridSystem::CellToGamePosition(int gridX, int gridY) const {
  // 計算相對於中心點的世界座標
  float halfWidth = m_BackgroundWidth / 2.0f;
  float halfHeight = m_BackgroundHeight / 2.0f;

  // 轉換格子座標到世界座標 (左上角為0,0)
  float worldX = -halfWidth + (gridX * m_CellSize) + (m_CellSize / 2.0f);

  // y值從上往下遞增，所以需要反轉y軸
  float worldY = halfHeight - (gridY * m_CellSize) - (m_CellSize / 2.0f);

  return glm::vec2(worldX, worldY);
}

glm::ivec2 GridSystem::GameToCellPosition(const glm::vec2 &worldPos) const {
  // 計算相對於左上角的格子座標
  float halfWidth = m_BackgroundWidth / 2.0f;
  float halfHeight = m_BackgroundHeight / 2.0f;

  // 計算相對於左上角原點的位置
  float relativeX = worldPos.x + halfWidth;
  float relativeY = halfHeight - worldPos.y; // 反轉y軸

  // 轉換為格子座標
  int gridX = static_cast<int>(relativeX / m_CellSize);
  int gridY = static_cast<int>(relativeY / m_CellSize);

  // 確保在有效範圍內
  gridX = std::clamp(gridX, 0, m_GridWidth - 1);
  gridY = std::clamp(gridY, 0, m_GridHeight - 1);

  return glm::ivec2(gridX, gridY);
}

bool GridSystem::CanMoveOn(CellType type, bool isFireboy) const {
  switch (type) {
  case CellType::EMPTY:
  case CellType::BUTTON:
  case CellType::LEVER:
  case CellType::PLATFORM:
  case CellType::GEM_FIRE:
  case CellType::GEM_WATER:
    return true;
  case CellType::FLOOR:
    return true;
  case CellType::WALL:
    return false;
  case CellType::LAVA:
    return isFireboy; // 只有火男可以在岩漿上移動
  case CellType::WATER:
    return !isFireboy; // 只有水女可以在水上移動
  case CellType::DOOR_FIRE:
    return isFireboy; // 只有火男可以通過火門
  case CellType::DOOR_WATER:
    return !isFireboy; // 只有水女可以通過水門
  default:
    return false;
  }
}

// 檢查指定的遊戲座標位置是否有碰撞
bool GridSystem::CheckCollision(const glm::vec2 &worldPos, glm::vec2 size,
                                bool isFireboy, int deltaX) const {
  // 根據移動方向確定需要檢查的側面
  // 向右移動，只檢查右側
  if (deltaX > 0) {
    // 向右移動，只檢查右側
    glm::vec2 rightEdge = glm::vec2(worldPos.x + (size.x / 2.0f), worldPos.y);
    glm::ivec2 gridPosRight = GameToCellPosition(rightEdge);
    CellType cellTypeRight = GetCell(gridPosRight.x, gridPosRight.y);
    return !CanMoveOn(cellTypeRight, isFireboy);
  } // 向左移動，只檢查左側
  else if (deltaX < 0) {
    glm::vec2 leftEdge = glm::vec2(worldPos.x - (size.x / 2.0f), worldPos.y);
    glm::ivec2 gridPosLeft = GameToCellPosition(leftEdge);
    CellType cellTypeLeft = GetCell(gridPosLeft.x, gridPosLeft.y);
    return !CanMoveOn(cellTypeLeft, isFireboy);
  }
  // 垂直移動或原地不動，檢查角色所在位置
  else {
    glm::ivec2 gridPos = GameToCellPosition(worldPos);
    CellType cellType = GetCell(gridPos.x, gridPos.y);
    return !CanMoveOn(cellType, isFireboy);
  }
}

bool GridSystem::IsValidGridPosition(int x, int y) const {
  return (x >= 0 && x < m_GridWidth && y >= 0 && y < m_GridHeight);
}