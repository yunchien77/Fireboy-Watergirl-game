#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"


class Character : public Util::GameObject {
public:
    explicit Character(const std::string& imagePath)
        : GameObject(std::make_shared<Util::Image>(imagePath), 10),
          m_ImagePath(imagePath), isMoving(false), currentSprite(false) {}

    Character(const Character&) = delete;
    Character(Character&&) = delete;
    Character& operator=(const Character&) = delete;
    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }
    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void SetImage(const std::string& imagePath) {
        m_ImagePath = imagePath;
        SetDrawable(std::make_shared<Util::Image>(imagePath));
    }

    void SetPosition(const glm::vec2& position) {
        m_Transform.translation = position;
    }

    void Move(int deltaX, int deltaY) {
        isMoving = (deltaX != 0 || deltaY != 0);
        glm::vec2 newPos = GetPosition() + glm::vec2(deltaX, deltaY);
        SetPosition(newPos);
        UpdateAnimation();
    }

    virtual void UpdateAnimation() = 0;

protected:
    std::string m_ImagePath;
    bool isMoving;
    bool currentSprite;

};

#endif // CHARACTER_HPP
