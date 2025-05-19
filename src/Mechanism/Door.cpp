#include "Mechanism/Door.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"

Door::Door(const std::string &closedImagePath,
           const std::string &halfOpenImagePath,
           const std::string &fullyOpenImagePath, bool isFireDoor, float zIndex)
    : GameObject(std::make_shared<Util::Image>(closedImagePath), zIndex),
      m_IsFireDoor(isFireDoor), m_CurrentState(DoorState::CLOSED),
      m_IsCharacterAtDoor(false), m_AnimationCounter(0) {

  m_DoorImages[DoorState::CLOSED] = closedImagePath;
  m_DoorImages[DoorState::HALF_OPEN] = halfOpenImagePath;
  m_DoorImages[DoorState::FULLY_OPEN] = fullyOpenImagePath;

  SetPivot(glm::vec2(-GetScaledSize().x / 2 + 15.0f,
                     -GetScaledSize().y / 2 + 15.0f));
}

bool Door::IsOpen() const { return m_CurrentState == DoorState::FULLY_OPEN; }

bool Door::IsFireDoor() const { return m_IsFireDoor; }

void Door::SetOpen(bool isOpen) {
  if (isOpen && m_CurrentState != DoorState::FULLY_OPEN) {
    m_CurrentState = DoorState::FULLY_OPEN;
    UpdateDoorImage();
  } else if (!isOpen && m_CurrentState != DoorState::CLOSED) {
    m_CurrentState = DoorState::CLOSED;
    UpdateDoorImage();
  }
}

void Door::OnCharacterEnter(Character *character) {
  m_IsCharacterAtDoor = true;
  m_AnimationCounter = 0;
}

// void Door::OnCharacterExit(Character *character) {
//   m_IsCharacterAtDoor = false;
//   m_AnimationCounter = 0;
// }

bool Door::IsCharacterAtDoor() const { return m_IsCharacterAtDoor; }

void Door::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

glm::vec2 Door::GetPosition() const { return m_Transform.translation; }

void Door::OpenDoor() {
  if (m_CurrentState == DoorState::CLOSED) {
    m_CurrentState = DoorState::HALF_OPEN;
    UpdateDoorImage();
  } else if (m_CurrentState == DoorState::HALF_OPEN) {
    m_CurrentState = DoorState::FULLY_OPEN;
    UpdateDoorImage();
  }
}

void Door::CloseDoor() {
  if (m_CurrentState == DoorState::FULLY_OPEN) {
    m_CurrentState = DoorState::HALF_OPEN;
    UpdateDoorImage();
  } else if (m_CurrentState == DoorState::HALF_OPEN) {
    m_CurrentState = DoorState::CLOSED;
    UpdateDoorImage();
  }
}

void Door::Update(float deltaTime) {
  m_AnimationCounter++;
  if (m_AnimationCounter >= 10) {
    m_AnimationCounter = 0;
    if (m_IsCharacterAtDoor && m_CurrentState != DoorState::FULLY_OPEN) {
      OpenDoor();
    } else if (!m_IsCharacterAtDoor && m_CurrentState != DoorState::CLOSED) {
      CloseDoor();
    }
  }
}

void Door::UpdateDoorImage() {
  SetDrawable(std::make_shared<Util::Image>(m_DoorImages[m_CurrentState]));
}

void Door::OnTriggered() {
  // Door can be triggered by switches or other mechanisms
  if (m_CurrentState != DoorState::FULLY_OPEN) {
    OpenDoor();
  }
}

void Door::OnReleased() {
  // Door can be released by switches or other mechanisms
  if (m_CurrentState != DoorState::CLOSED && !m_IsCharacterAtDoor) {
    CloseDoor();
  }
}

const SDL_Rect &Door::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);

  return m_Rect;
}

void Door::SetCharacterAtDoor(bool isAtDoor) {
  m_IsCharacterAtDoor = isAtDoor;
  m_AnimationCounter = 0;
}

void Door::UpdateAnimation() {
  m_AnimationCounter++;
  if (m_AnimationCounter >= 10) {
    m_AnimationCounter = 0;
    if (m_IsCharacterAtDoor && m_CurrentState != DoorState::FULLY_OPEN) {
      OpenDoor();
    } else if (!m_IsCharacterAtDoor && m_CurrentState != DoorState::CLOSED) {
      CloseDoor();
    }
  }
}