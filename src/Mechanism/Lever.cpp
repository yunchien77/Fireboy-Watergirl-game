#include "Mechanism/Lever.hpp"
#include "Util/Image.hpp"

Lever::Lever(LeverColor color, const glm::vec2 &pos) : m_Color(color) {
    SetDrawable(std::make_shared<Util::Image>(GetImagePath(color, false)));
    SetPosition(pos);
    SetPivot({0.0f, 0.0f});
    SetZIndex(25);
}

std::string Lever::GetImagePath(LeverColor color, bool isOn) const {
    std::string base = RESOURCE_DIR "/material/props/lever/lever-";
    switch (color) {
        case LeverColor::BLUE:
            base += "blue";
        break;
        case LeverColor::GREEN:
            base += "green";
        break;
        case LeverColor::ORANGE:
            base += "orange";
        break;
        case LeverColor::PINK:
            base += "pink";
        break;
        case LeverColor::WHITE:
            base += "white";
        break;
        case LeverColor::YELLOW:
            base += "yellow";
        break;
    }
    base += isOn ? "-on.png" : "-off.png";
    return base;
}

void Lever::update(Character *fb, Character *wg) {
    auto rect = getRect();
    bool fbOn = SDL_HasIntersection(&fb->getRect(), &rect);
    bool wgOn = SDL_HasIntersection(&wg->getRect(), &rect);
    bool isInteracting = fbOn || wgOn;

    // 當角色第一次靠近時觸發切換（避免重複觸發）
    if (isInteracting && !m_WasInteracting) {
        Toggle();
    }

    m_WasInteracting = isInteracting;
}

void Lever::Toggle() {
    m_IsOn = !m_IsOn;
    SetDrawable(std::make_shared<Util::Image>(GetImagePath(m_Color, m_IsOn)));
    for (auto *t : m_Triggers) {
        if (m_IsOn) {
            t->OnTriggered();
        } else {
            t->OnReleased();
        }
    }
}

void Lever::linkTrigger(ITriggerable *target) { m_Triggers.push_back(target); }

LeverColor Lever::GetColor() const { return m_Color; }

const SDL_Rect &Lever::getRect() const {
    glm::vec2 pos = m_Transform.translation;
    glm::vec2 size = GetScaledSize() * 0.4f; // 可依實際按鈕大小微調縮放
    m_Rect.x = static_cast<int>(pos.x - size.x / 2);
    m_Rect.y = static_cast<int>(pos.y - size.y / 2);
    m_Rect.w = static_cast<int>(size.x);
    m_Rect.h = static_cast<int>(size.y);
    return m_Rect;
}



void Lever::SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
}