#ifndef GEM_HPP
#define GEM_HPP

#include "Interface/IGem.hpp"
#include "Util/GameObject.hpp"
#include "Character/Character.hpp"

enum class GemType {
    FIRE,
    WATER,
    GREEN
};

class Gem : public IGem, public Util::GameObject{
public:
    explicit Gem(GemType type);

    void OnCharacterEnter(Character* character) override;

    const SDL_Rect& getRect() const override;

    void SetPosition(const glm::vec2& position);
    GemType GetType() const;

    void SetInitialPosition(const glm::vec2& pos);
    void Respawn() override;

private:
    GemType m_Type;
    bool m_Collected = false;
    mutable SDL_Rect m_Rect;
    glm::vec2 m_InitialPosition;
};

#endif // GEM_HPP
