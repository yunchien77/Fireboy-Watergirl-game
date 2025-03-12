#include "Watergirl.hpp"
#include "config.hpp"

Watergirl::Watergirl()
    : Character(RESOURCE_DIR "/material/character/watergirl-front.png"), animationFrame(0) {}

void Watergirl::UpdateAnimation() {
    if (isMoving) {
        if (animationFrame % 5 == 0) {
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
