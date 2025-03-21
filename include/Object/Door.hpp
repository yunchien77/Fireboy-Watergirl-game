#ifndef DOOR_HPP
#define DOOR_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <string>

class Door : public Util::GameObject {
public:
  explicit Door(const std::string &imagePath)
      : GameObject(std::make_shared<Util::Image>(imagePath), 10),
        m_ImagePath(imagePath), m_IsOpen(false) {
    SetPivot(glm::vec2(-GetScaledSize().x / 2 + 15.0f,
                       -GetScaledSize().y / 2 + 15.0f));
  }

  Door(const Door &) = delete;
  Door(Door &&) = delete;
  Door &operator=(const Door &) = delete;
  Door &operator=(Door &&) = delete;

  [[nodiscard]] const std::string &GetImagePath() const { return m_ImagePath; }
  [[nodiscard]] bool IsOpen() const { return m_IsOpen; }

  void SetOpen(bool isOpen) {
    m_IsOpen = isOpen;
    SetVisible(!isOpen);
  }

  void SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
  }

  void SetImage(const std::string &imagePath) {
    m_ImagePath = imagePath;
    SetDrawable(std::make_shared<Util::Image>(imagePath));
  }

  void UpdateAnimation() {}

private:
  std::string m_ImagePath;
  bool m_IsOpen; // 門是否打開
};

#endif // DOOR_HPP