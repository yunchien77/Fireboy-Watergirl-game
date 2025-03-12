#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <memory>

class BackgroundImage : public Util::GameObject {
public:
    BackgroundImage(const std::string &imagePath)
        : GameObject(std::make_unique<Util::Image>(imagePath), -10) {

        auto image = std::make_unique<Util::Image>(imagePath);
        m_Width = GetWidth();
        m_Height = GetHeight();
    }

    void SetPosition(const glm::vec2& Position) {
        m_Transform.translation = Position;
        m_X = Position.x;
        m_Y = Position.y;
    }
    int GetX() const { return m_X; }
    int GetY() const { return m_Y; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    int m_X;
    int m_Y;
    int m_Width;
    int m_Height;
};

#endif // BACKGROUND_IMAGE_HPP
