#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "Util/GameObject.hpp"
#include "Interface/ITriggerable.hpp"
#include <glm/glm.hpp>

enum class PlatformColor { BLUE, GREEN, ORANGE, PINK, WHITE, YELLOW };

class Platform : public Util::GameObject, public ITriggerable {
public:
    Platform(PlatformColor color, const glm::vec2 &pos, const glm::vec2 &moveOffset);

    void OnTriggered() override;
    void OnReleased() override;
    void UpdateAnimation(float deltaTime);
    void SetPosition(const glm::vec2 &position);

    PlatformColor GetColor() const;

private:
    glm::vec2 m_InitialPosition;
    glm::vec2 m_MoveOffset;
    glm::vec2 m_TargetPosition;
    bool m_ShouldMove = false;
    bool m_IsAnimating = false;
    PlatformColor m_Color;

    std::string GetImagePath(PlatformColor color) const;
};

#endif // PLATFORM_HPP
