#ifndef GRID_SYSTEM_HPP
#define GRID_SYSTEM_HPP

#include "pch.hpp"
#include <string>
#include <vector>

enum class CellType {
  EMPTY,      // 空的區域，可以通過0
  FLOOR,      // 地板1
  WALL,       // 牆壁2
  DOOR_FIRE,  // 火門3
  DOOR_WATER, // 水門4
  GEM_FIRE,   // 火寶石5
  GEM_WATER,  // 水寶石6
  GEM_GREEN,  // 綠寶石7
  LAVA,       // 岩漿8
  WATER,      // 水池9
  POISON,     // 毒池10
  BUTTON,     // 按鈕11
  LEVER,      // 拉桿12
  PLATFORM,   // 平台13
  FAN,        // 風扇14
  BOX,        // 箱子15
  STONE,      // 小石子16

};

class GridSystem {
public:
  GridSystem();

  bool LoadFromFile(const std::string &filePath);

  // 取得指定位置的格子類型
  CellType GetCell(int x, int y) const;

  // 將格子座標轉換為遊戲座標
  glm::vec2 CellToGamePosition(int gridX, int gridY) const;

  // 將遊戲座標轉換為格子座標
  glm::ivec2 GameToCellPosition(const glm::vec2 &worldPos) const;

  // 檢查角色是否能夠在特定格子類型上移動
  bool CanMoveOn(CellType type, bool isFireboy) const;

  // 檢查角色是否能夠在特定格子類型上站立
  bool CanStandOn(CellType type, bool isFireboy) const;

  // 檢查指定的遊戲座標位置是否有碰撞
  bool CheckCollision(const glm::vec2 &worldPos, glm::vec2 size, bool isFireboy,
                      int deltaX = 0) const;

  // 取得格子系統的邊界限制 (基於背景圖片大小)
  float GetMinX() const { return -m_BackgroundWidth / 2.0f; }
  float GetMaxX() const { return m_BackgroundWidth / 2.0f; }
  float GetMinY() const { return -m_BackgroundHeight / 2.0f; }
  float GetMaxY() const { return m_BackgroundHeight / 2.0f; }

  // 取得格子尺寸
  int GetCellSize() const { return m_CellSize; }

  // 取得網格尺寸
  int GetGridWidth() const { return m_GridWidth; }
  int GetGridHeight() const { return m_GridHeight; }

private:
  std::vector<std::vector<CellType>> m_Grid;
  int m_GridWidth;
  int m_GridHeight;
  int m_CellSize;
  int m_BackgroundWidth;
  int m_BackgroundHeight;

  // 檢查格子座標是否在有效範圍內
  bool IsValidGridPosition(int x, int y) const;
};

#endif // GRID_SYSTEM_HPP