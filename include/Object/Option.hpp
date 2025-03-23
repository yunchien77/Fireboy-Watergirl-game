#ifndef OPTION_HPP
#define OPTION_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>

class Option : public Util::GameObject {
public:
  Option(const std::string &imagePath, const glm::vec2 &position = {0, 0},
         int zIndex = 40)
      : GameObject(std::make_unique<Util::Image>(imagePath), zIndex) {

    glm::vec2 size = GetScaledSize();
    m_Width = size.x;
    m_Height = size.y;
    SetPosition(position);
  }

  // 設置按鈕位置
  void SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
    m_Position = position;
  }

  // 啟用/禁用按鈕
  void SetInteractable(bool interactable) { m_Interactable = interactable; }
  bool IsInteractable() const { return m_Interactable; }

  // 設置點擊事件回調
  void SetOnClickCallback(std::function<void()> callback) {
    m_OnClickCallback = callback;
  }

  // 檢查是否點擊到按鈕
  bool IsClicked(int mouseX, int mouseY) const {
    if (!m_Interactable || !m_Visible) {
      return false;
    }

    // 檢查滑鼠座標是否在按鈕範圍內
    bool isClicked = (mouseX >= m_Position.x - m_Width / 2 &&
                      mouseX <= m_Position.x + m_Width / 2 &&
                      mouseY >= m_Position.y - m_Height / 2 &&
                      mouseY <= m_Position.y + m_Height / 2);
    return isClicked;
  }

  // 處理點擊事件
  bool HandleClick(int mouseX, int mouseY) {
    if (IsClicked(mouseX, mouseY)) {
      if (m_OnClickCallback) {
        m_OnClickCallback();
      }
      return true;
    }
    return false;
  }

  // 設置可見性
  void SetVisible(bool visible) {
    m_Visible = visible;
    GameObject::SetVisible(visible);
  }

  // 獲取可見性
  bool IsVisible() const { return m_Visible; }

  // 獲取按鈕尺寸
  int GetWidth() const { return m_Width; }
  int GetHeight() const { return m_Height; }

private:
  glm::vec2 m_Position = {0, 0};
  int m_Width = 0;
  int m_Height = 0;
  bool m_Interactable = true; // 預設為可互動
  bool m_Visible = true;      // 追蹤按鈕可見性
  std::function<void()> m_OnClickCallback = nullptr;
};

#endif // OPTION_HPP