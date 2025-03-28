#include "Character/Watergirl.hpp"
#include "config.hpp"

Watergirl::Watergirl()
    : Character(RESOURCE_DIR "/material/character/watergirl-front.png", 30),
      animationFrame(0) {}

void Watergirl::UpdateAnimation() {
  if (m_IsJumping) {
    SetImage(RESOURCE_DIR "/material/character/watergirl-front.png");
    animationFrame = 0;
  } else if (isMoving) {
    if (animationFrame % 10 == 0) {
      if (currentSprite) {
        SetImage(RESOURCE_DIR "/material/character/watergirl-side-run.png");
      } else {
        SetImage(RESOURCE_DIR "/material/character/watergirl-side.png");
      }
      currentSprite = !currentSprite;
    }
    animationFrame++;
  } else {
    SetImage(RESOURCE_DIR "/material/character/watergirl-front.png");
    animationFrame = 0;
  }
}

float Watergirl::getX() const { return GetPosition().x; }

float Watergirl::getY() const { return GetPosition().y; }

int Watergirl::getWidth() const { return static_cast<int>(GetSize().x); }

int Watergirl::getHeight() const { return static_cast<int>(GetSize().y); }

const SDL_Rect &Watergirl::getRect() const {
  static SDL_Rect rect;
  glm::vec2 pos = GetPosition();
  glm::vec2 size = GetSize();
  rect.x = static_cast<int>(pos.x - size.x / 2);
  rect.y = static_cast<int>(pos.y - size.y / 2);
  rect.w = static_cast<int>(size.x);
  rect.h = static_cast<int>(size.y);
  return rect;
}
