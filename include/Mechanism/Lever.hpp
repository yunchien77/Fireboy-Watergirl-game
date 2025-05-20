#ifndef LEVER_HPP
#define LEVER_HPP

#include "Character/Character.hpp"
#include "Interface/ITriggerable.hpp"
#include "Object/Color.hpp"
#include "Mechanism/MechanismBase.hpp"
#include <vector>

using LeverColor = Color;

class Lever : public MechanismBase {
public:
    Lever(LeverColor color, const glm::vec2 &pos);

    using MechanismBase::Update;
    void Update(Character *fb, Character *wg);

    void linkTrigger(ITriggerable *target);
    LeverColor GetColor() const;
    const SDL_Rect& getRect() const;

    void SetInitialPosition(const glm::vec2 &pos);
    void SetInitialState(const glm::vec2& pos, bool isOn);
    void Respawn() override;

private:
    void Toggle();
    std::string GetImagePath(LeverColor color, bool isOn) const;

    bool m_IsOn = false;
    bool m_InitialIsOn = false;
    LeverColor m_Color;
    mutable SDL_Rect m_Rect{};
    glm::vec2 m_InitialPosition;
    std::vector<ITriggerable *> m_Triggers;
};

#endif // LEVER_HPP
