#include "Fireboy.hpp"
#include "config.hpp"

Fireboy::Fireboy()
    : Character(RESOURCE_DIR "/material/character/fireboy-front.png"), animationFrame(0) {}

void Fireboy::UpdateAnimation() {
    if (isMoving) {
        if (animationFrame % 5 == 0) {
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
