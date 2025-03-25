#ifndef LIQUID_TRAP_HPP
#define LIQUID_TRAP_HPP

#include "Interface/ITrap.hpp"
#include "Object/GridSystem.hpp"
#include "Util/GameObject.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"

enum class SizeType {
    SMALL,
    LARGE
};

class LiquidTrap : public ITrap, public Util::GameObject {
public:
    explicit LiquidTrap(CellType type, SizeType sizeType = SizeType::LARGE);
    void OnCharacterEnter(Character* character) override;
    void SetImage(const std::string &imagePath);
    void SetPosition(const glm::vec2 &position);

protected:
    std::string m_ImagePath;

private:
    CellType type;
    SizeType sizeType;
};

#endif
