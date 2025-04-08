#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <memory>

class BackgroundImage : public Util::GameObject {
public:
  BackgroundImage(const std::string &imagePath, const float zindex = -10)
      : GameObject(std::make_unique<Util::Image>(imagePath), zindex) {}

  void SetPosition(const glm::vec2 &Position) {
    m_Transform.translation = Position;
    m_X = Position.x;
    m_Y = Position.y;
  }

  int GetX() const { return m_X; }
  int GetY() const { return m_Y; }
  // int GetWidth() const { return m_Width; }
  // int GetHeight() const { return m_Height; }

  void SetImage(const std::string &imagePath) {
    SetDrawable(std::make_unique<Util::Image>(imagePath));
  }

private:
  int m_X = 0;
  int m_Y = 0;
  // int m_Width = 0;
  // int m_Height = 0;
};

#endif // BACKGROUND_IMAGE_HPP
