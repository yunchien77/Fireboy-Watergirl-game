#include "Mechanism/Fan.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include "config.hpp"
#include <cmath>
#include <iostream>
#include <map>
#include <random>

Fan::Fan(const glm::vec2 &position, float strength, FanColor color)
    : m_InitialPosition(position), m_Strength(strength),
      m_MaxWindHeight(200.0f), m_AnimationTime(0.0f), m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath()));
  SetPosition(position);
  SetPivot({0.0f, 14.0f});
  SetZIndex(20);
}

std::string Fan::GetImagePath() const { return GetImagePath(m_Color); }

std::string Fan::GetImagePath(FanColor color) {
  std::string base = RESOURCE_DIR "/material/props/fan/fan-";
  switch (color) {
  case FanColor::BLUE:
    base += "blue";
    break;
  case FanColor::GREEN:
    base += "green";
    break;
  case FanColor::PINK:
    base += "pink";
    break;
  case FanColor::RED:
    base += "red";
    break;
  case FanColor::WHITE:
    base += "white";
    break;
  case FanColor::YELLOW:
    base += "yellow";
    break;
  }
  base += ".png";
  return base;
}

FanColor Fan::GetColor() const { return m_Color; }

bool Fan::IsCharacterInWindZone(Character *character) const {
  if (!character)
    return false;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 fanPos = m_Transform.translation;
  glm::vec2 fanSize = GetScaledSize();

  // 風扇的左右邊界
  float fanLeft = fanPos.x - (fanSize.x / 2.0f);
  float fanRight = fanPos.x + (fanSize.x / 2.0f);

  // 角色的左右邊界
  glm::vec2 charSize = character->GetSize();
  float charLeft = charPos.x - (charSize.x / 2.0f);
  float charRight = charPos.x + (charSize.x / 2.0f);

  // 檢查角色是否在風扇正上方（水平重疊）
  bool horizontalOverlap = (charRight > fanLeft) && (charLeft < fanRight);

  // 檢查角色是否在風扇上方
  bool isAboveFan = charPos.y > (fanPos.y - 28.0);

  //   if (isAboveFan && horizontalOverlap) {
  //     if (character->IsFireboy()) {
  //       std::cout << "Fireboy in wind zone" << std::endl;
  //     } else {
  //       std::cout << "Watergirl in wind zone" << std::endl;
  //     }
  //   }

  return isAboveFan && horizontalOverlap;
}

void Fan::ApplyWindForce(Character *character) {
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 fanPos = m_Transform.translation;

  if (!IsCharacterInWindZone(character)) {
    if (character->IsAffectedByWind()) {
      character->ApplyExternalForce(-0.1f);
    } else {
      character->SetAffectedByWind(false);
    }
    return;
  }

  character->SetAffectedByWind(true);

  float distanceFromFan = std::abs(charPos.y - (fanPos.y - 16.0f));

  if (distanceFromFan > m_MaxWindHeight) {
    character->SetAffectedByWind(false);
    return;
  }

  // 改進距離因子的計算，使其更加平滑
  float distanceFactor =
      1.0f - std::pow(distanceFromFan / m_MaxWindHeight, 0.75);

  // 提高最小力度因子，確保較為穩定的基本浮力
  float minForceFactor = 0.2f;
  distanceFactor = std::max(minForceFactor, distanceFactor);

  // 減小風力變化的振幅並減慢週期，從0.01f改為0.003f，使風更平穩和自然
  float windVariation = std::sin(m_AnimationTime * 0.003f) * 0.01f + 0.99f;

  // 調整理想高度，減少上下波動
  float idealHeight = fanPos.y + m_MaxWindHeight * 0.65f;
  float heightDifference = idealHeight - charPos.y;

  // 減小高度調整因子的影響
  float heightAdjustFactor = std::min(1.0f, std::abs(heightDifference) / 60.0f);

  // 在接近理想高度時提供更穩定的力度
  if (std::abs(heightDifference) < 15.0f) {
    heightAdjustFactor = 0.95f;
  } else if (heightDifference < 0) {
    // 減小當角色高於理想高度時的調整幅度
    heightAdjustFactor *= 0.8f;
  } else {
    // 輕微增加當角色低於理想高度時的調整力度
    heightAdjustFactor *= 1.1f;
  }

  float baseForce = m_Strength * 0.06f;

  // 使用一個靜態map來記錄每個角色的上一次力度，便於平滑過渡
  static std::map<Character *, float> lastForces;
  float lastForce = lastForces.count(character) ? lastForces[character] : 0.0f;

  float newForce =
      baseForce * distanceFactor * windVariation * heightAdjustFactor;

  // 增加平滑程度
  float smoothedForce = lastForce * 0.9f + newForce * 0.1f;

  lastForces[character] = smoothedForce;

  character->ApplyExternalForce(smoothedForce);
}

void Fan::Update(float deltaTime) {
  m_AnimationTime += deltaTime * 1000.0f; // Update animation time
}

void Fan::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

const SDL_Rect &Fan::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);

  return m_Rect;
}

void Fan::SetInitialPosition(const glm::vec2 &pos) { m_InitialPosition = pos; }

void Fan::Respawn() { SetPosition(m_InitialPosition); }