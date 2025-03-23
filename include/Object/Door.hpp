#ifndef DOOR_HPP
#define DOOR_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

enum class DoorState { CLOSED, HALF_OPEN, FULLY_OPEN };

class Door : public Util::GameObject {
public:
  Door(const std::string &closedImagePath, const std::string &halfOpenImagePath,
       const std::string &fullyOpenImagePath, bool isFireDoor,
       const float zIndex = 20)
      : GameObject(std::make_shared<Util::Image>(closedImagePath), zIndex),
        m_IsFireDoor(isFireDoor), m_CurrentState(DoorState::CLOSED),
        m_IsCharacterAtDoor(false), m_AnimationCounter(0) {

    m_DoorImages[DoorState::CLOSED] = closedImagePath;
    m_DoorImages[DoorState::HALF_OPEN] = halfOpenImagePath;
    m_DoorImages[DoorState::FULLY_OPEN] = fullyOpenImagePath;

    SetPivot(glm::vec2(-GetScaledSize().x / 2 + 15.0f,
                       -GetScaledSize().y / 2 + 15.0f));
  }

  Door(const Door &) = delete;
  Door(Door &&) = delete;
  Door &operator=(const Door &) = delete;
  Door &operator=(Door &&) = delete;

  [[nodiscard]] bool IsOpen() const {
    return m_CurrentState == DoorState::FULLY_OPEN;
  }

  [[nodiscard]] bool IsFireDoor() const { return m_IsFireDoor; }

  void SetOpen(bool isOpen) {
    if (isOpen && m_CurrentState != DoorState::FULLY_OPEN) {
      m_CurrentState = DoorState::FULLY_OPEN;
      UpdateDoorImage();
    } else if (!isOpen && m_CurrentState != DoorState::CLOSED) {
      m_CurrentState = DoorState::CLOSED;
      UpdateDoorImage();
    }
  }

  void SetCharacterAtDoor(bool isAtDoor) {
    m_IsCharacterAtDoor = isAtDoor;
    // Reset animation counter when character state changes
    m_AnimationCounter = 0;
  }

  bool IsCharacterAtDoor() const { return m_IsCharacterAtDoor; }

  void SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
  }

  glm::vec2 GetPosition() const { return m_Transform.translation; }

  // Animate door opening
  void OpenDoor() {
    if (m_CurrentState == DoorState::CLOSED) {
      m_CurrentState = DoorState::HALF_OPEN;
      UpdateDoorImage();
    } else if (m_CurrentState == DoorState::HALF_OPEN) {
      m_CurrentState = DoorState::FULLY_OPEN;
      UpdateDoorImage();
    }
  }

  // Animate door closing
  void CloseDoor() {
    if (m_CurrentState == DoorState::FULLY_OPEN) {
      m_CurrentState = DoorState::HALF_OPEN;
      UpdateDoorImage();
    } else if (m_CurrentState == DoorState::HALF_OPEN) {
      m_CurrentState = DoorState::CLOSED;
      UpdateDoorImage();
    }
  }

  void UpdateAnimation() {
    // Increment counter every frame
    m_AnimationCounter++;

    // Only change state every N frames (this controls animation speed)
    if (m_AnimationCounter >= 10) {
      m_AnimationCounter = 0;

      // This method can be called during game loop to progress animations
      if (m_IsCharacterAtDoor && m_CurrentState != DoorState::FULLY_OPEN) {
        OpenDoor();
      } else if (!m_IsCharacterAtDoor && m_CurrentState != DoorState::CLOSED) {
        CloseDoor();
      }
    }
  }

private:
  void UpdateDoorImage() {
    SetDrawable(std::make_shared<Util::Image>(m_DoorImages[m_CurrentState]));
  }

  std::map<DoorState, std::string> m_DoorImages;
  bool m_IsFireDoor; // true for Fireboy's door, false for Watergirl's door
  DoorState m_CurrentState;
  bool m_IsCharacterAtDoor;
  int m_AnimationCounter; // 用於控制動畫速度的計數器
};

#endif // DOOR_HPP