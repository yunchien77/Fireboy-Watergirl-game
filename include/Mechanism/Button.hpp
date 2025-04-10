#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Util/GameObject.hpp"
#include "Interface/ITriggerable.hpp"
#include <vector>

enum class ButtonColor {
    BLUE,
    GREEN,
    ORANGE,
    PINK,
    WHITE,
    YELLOW
  };

class Character;

class Button : public Util::GameObject {
public:
    Button(ButtonColor color, const glm::vec2& pos);

    void update(Character* fb, Character* wg);
    void linkTrigger(ITriggerable* target);
    void SetPosition(const glm::vec2 &position);
    const SDL_Rect& getRect() const;

    ButtonColor GetColor() const;

private:
    ButtonColor m_Color;
    mutable SDL_Rect m_Rect;
    bool m_IsPressed = false;
    SDL_Rect m_Box;
    std::vector<ITriggerable*> m_Triggers;

    std::string GetImagePath(ButtonColor color);
};

#endif //BUTTON_HPP
