#include "Character/Fireboy.hpp"
#include "config.hpp"

Fireboy::Fireboy()
    : Character(RESOURCE_DIR "/material/character/fireboy-front.png", 30),
      animationFrame(0) {}

void Fireboy::UpdateAnimation() {
  if (m_IsJumping) {
    SetImage(RESOURCE_DIR "/material/character/fireboy-front.png");
    animationFrame = 0;
  } else if (isMoving) {
    if (animationFrame % 7 == 0) {
      if (currentSprite) {
        SetImage(RESOURCE_DIR "/material/character/fireboy-side-run.png");
      } else {
        SetImage(RESOURCE_DIR "/material/character/fireboy-side.png");
      }
      currentSprite = !currentSprite;
    }
    animationFrame++;
  } else {
    SetImage(RESOURCE_DIR "/material/character/fireboy-front.png");
    animationFrame = 0;
  }
}

float Fireboy::getX() const {
  return GetPosition().x;
}

float Fireboy::getY() const {
  return GetPosition().y;
}

int Fireboy::getWidth() const {
  return static_cast<int>(GetSize().x);
}

int Fireboy::getHeight() const {
  return static_cast<int>(GetSize().y);
}

const SDL_Rect& Fireboy::getRect() const {
  static SDL_Rect rect;
  glm::vec2 pos = GetPosition();
  glm::vec2 size = GetSize();
  rect.x = static_cast<int>(pos.x - size.x / 2);
  rect.y = static_cast<int>(pos.y - size.y / 2);
  rect.w = static_cast<int>(size.x);
  rect.h = static_cast<int>(size.y);
  return rect;
}



