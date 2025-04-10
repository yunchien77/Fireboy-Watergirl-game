#ifndef GATE_HPP
#define GATE_HPP

#include "Util/GameObject.hpp"
#include "Interface/ITriggerable.hpp"
#include <string>
#include <glm/glm.hpp>

enum class GateColor {
    BLUE,
    GREEN,
    ORANGE,
    PINK,
    WHITE,
    YELLOW
};

class Gate : public Util::GameObject, public ITriggerable {
public:
    Gate(GateColor color, const glm::vec2& pos);

    void OnTriggered() override;
    void OnReleased() override;
    void SetOpen(bool open);
    void SetPosition(const glm::vec2& position);
    bool IsOpen() const;
    bool IsBlocking() const;
    GateColor GetColor() const;
    const SDL_Rect& getRect() const;
    void UpdateAnimation(float deltaTime);

private:
    GateColor m_Color;
    bool m_IsOpen = false;
    glm::vec2 m_InitialPosition;
    mutable SDL_Rect m_Rect;
    bool m_IsAnimating = false;
    bool m_ShouldOpen = false;

    std::string GetImagePath(GateColor color);
};

#endif // GATE_HPP
