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
