#include "Mechanism/Platform.hpp"

#include "Character/Character.hpp"
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
    if (!m_IsAnimating) {
        m_LastDeltaMovement = {0.0f, 0.0f};  // 沒動就清空
        return;
    }

    glm::vec2 currentPos = m_Transform.translation;
    glm::vec2 direction = m_TargetPosition - currentPos;

    float speed = 80.0f;
    glm::vec2 movement = glm::normalize(direction) * speed * deltaTime;

    // 記錄這一幀平台的實際移動量
    if (glm::length(direction) < glm::length(movement)) {
        m_LastDeltaMovement = direction;  // 最後一步直接補上剩餘距離
        SetPosition(m_TargetPosition);
        m_IsAnimating = false;
    } else {
        m_LastDeltaMovement = movement;  // 正常移動
        SetPosition(currentPos + movement);
    }
}

void Platform::SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
}

PlatformColor Platform::GetColor() const {
    return m_Color;
}

glm::vec2 Platform::GetDeltaMovement() const {
    return m_LastDeltaMovement;
}

bool Platform::IsCharacterOn(Character* character) const {
    SDL_Rect charRect = character->getRect();
    SDL_Rect platRect = getRect();

    // 只偵測腳底碰觸平台頂部
    return SDL_HasIntersection(&charRect, &platRect) &&
           charRect.y + charRect.h - 5 <= platRect.y;
}


const SDL_Rect &Platform::getRect() const {
    glm::vec2 pos = m_Transform.translation;
    glm::vec2 size = GetScaledSize();

    m_Rect.x = static_cast<int>(pos.x);
    m_Rect.y = static_cast<int>(pos.y);
    m_Rect.w = static_cast<int>(size.x);
    m_Rect.h = static_cast<int>(size.y);
    return m_Rect;
}

