#include "Mechanism/Platform.hpp"
#include "Util/Image.hpp"

Platform::Platform(PlatformColor color, const glm::vec2 &pos, const glm::vec2 &moveOffset)
    : m_Color(color), m_InitialPosition(pos), m_MoveOffset(moveOffset) {
    SetDrawable(std::make_shared<Util::Image>(GetImagePath(color)));
    SetPosition(pos);
    SetPivot({0.0f, 0.0f});
    SetZIndex(25);
}

std::string Platform::GetImagePath(PlatformColor color) const {
    std::string base = RESOURCE_DIR "/material/props/platform/platform-";
    switch (color) {
        case PlatformColor::BLUE: base += "blue"; break;
        case PlatformColor::GREEN: base += "green"; break;
        case PlatformColor::ORANGE: base += "orange"; break;
        case PlatformColor::PINK: base += "pink"; break;
        case PlatformColor::WHITE: base += "white"; break;
        case PlatformColor::YELLOW: base += "yellow"; break;
    }
    base += ".png";
    return base;
}

void Platform::OnTriggered() {
    m_TargetPosition = m_InitialPosition + m_MoveOffset;
    m_ShouldMove = true;
    m_IsAnimating = true;
}

void Platform::OnReleased() {
    m_TargetPosition = m_InitialPosition;
    m_ShouldMove = false;
    m_IsAnimating = true;
}

void Platform::UpdateAnimation(float deltaTime) {
    if (!m_IsAnimating) return;

    glm::vec2 currentPos = m_Transform.translation;
    glm::vec2 direction = m_TargetPosition - currentPos;

    float speed = 100.0f;
    if (glm::length(direction) < 1.0f) {
        SetPosition(m_TargetPosition);
        m_IsAnimating = false;
    } else {
        SetPosition(currentPos + glm::normalize(direction) * speed * deltaTime);
    }
}

void Platform::SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
}

PlatformColor Platform::GetColor() const {
    return m_Color;
}
