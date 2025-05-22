// Level1.cpp
#include "Level/Level1.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/LiquidTrap.hpp"

bool Level1::Initialize() {
    // 初始化 Character 和 Door
    if (!m_Fireboy)
        m_Fireboy = std::make_shared<Fireboy>();
    InitCharacter(m_Fireboy, {35, 5});

    if (!m_Watergirl)
        m_Watergirl = std::make_shared<Watergirl>();
    InitCharacter(m_Watergirl, {3, 17});

    InitDoor(m_Fireboy_Door, {32, 14});
    InitDoor(m_Watergirl_Door, {4, 27});

    // 初始化Trap
    InitLiquidTraps(CellType::WATER, SizeType::SMALL, {{27, 7}, {22, 7}, {17, 7}, {19, 12}});
    InitLiquidTraps(CellType::LAVA, SizeType::SMALL, {{19, 19}, {14, 19}, {9, 19}, {28, 22}});

    // 初始化Gem
    InitGems(GemType::FIRE, {{24, 6}, {19, 6}, {14, 6}, {3, 10}});
    InitGems(GemType::WATER, {{11, 18}, {16, 18}, {21, 18}, {34, 22}});

    return true;
}
