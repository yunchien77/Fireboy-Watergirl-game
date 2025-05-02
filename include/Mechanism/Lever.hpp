#ifndef LEVER_HPP
#define LEVER_HPP

#include "Util/GameObject.hpp"
#include "Character/Character.hpp"
#include "Interface/ITriggerable.hpp"
#include "Object/Color.hpp"
#include <vector>

using LeverColor = Color;

class Lever : public Util::GameObject {
public:
    Lever(LeverColor color, const glm::vec2 &pos);
    void update(Character *fb, Character *wg);
    void linkTrigger(ITriggerable *target);
    LeverColor GetColor() const;
    const SDL_Rect& getRect() const;
    void SetPosition(const glm::vec2 &position);

private:
    bool m_IsOn = false;
    LeverColor m_Color;
    mutable SDL_Rect m_Rect{};
    std::vector<ITriggerable *> m_Triggers;
    bool m_WasInteracting = false;

    void Toggle();
    std::string GetImagePath(LeverColor color, bool isOn) const;
    glm::vec2 m_LastFireboyPos;
    glm::vec2 m_LastWatergirlPos;

};

#endif
