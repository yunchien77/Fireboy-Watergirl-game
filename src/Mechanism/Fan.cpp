#include "Mechanism/Fan.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include "config.hpp"
#include <cmath>
#include <iostream>
#include <random>

Fan::Fan(const glm::vec2 &position, float strength, float windWidth)
    : m_InitialPosition(position), m_Strength(strength), m_WindWidth(windWidth),
      m_WindHeight(190.0f), // Default wind height effect
      m_MaxWindHeight(230.0f), m_AnimationTime(0.0f) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath()));
  SetPosition(position);
  SetPivot({0.0f, 14.0f});
  SetZIndex(20);

  // Initialize wind particles
  m_WindParticles.reserve(30); // Reserve space for particles
  GenerateWindParticles();
}

std::string Fan::GetImagePath() const {
  return RESOURCE_DIR
      "/material/props/fan/fan-white.png"; // You'll need to create this asset
}

void Fan::Update(float deltaTime) {
  UpdateAnimation(deltaTime);
  UpdateWindParticles(deltaTime);
}

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

  if (isAboveFan && horizontalOverlap) {
    if (character->IsFireboy()) {
      std::cout << "Fireboy in wind zone" << std::endl;
    } else {
      std::cout << "Watergirl in wind zone" << std::endl;
    }
  }

  return isAboveFan && horizontalOverlap;
}

void Fan::ApplyWindForce(Character *character, float deltaTime) {
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 fanPos = m_Transform.translation;

  // 如果角色不在风区，清除风力影响
  if (!IsCharacterInWindZone(character)) {
    character->SetAffectedByWind(false);
    return;
  }

  // 设置角色受风力影响
  character->SetAffectedByWind(true);

  // 计算风力强度基于距离的衰减
  float distanceFromFan = std::abs(charPos.y - (fanPos.y - 16.0f));

  // 限制最大高度 - 这是关键修改
  if (distanceFromFan > m_MaxWindHeight) {
    character->SetAffectedByWind(false);
    return; // 超过最大高度，不再应用风力
  }

  float distanceFactor =
      1.0f - std::min(1.0f, distanceFromFan / m_MaxWindHeight);

  // 增加最小风力确保持续向上效果
  float minForceFactor = 0.7f;
  distanceFactor = std::max(minForceFactor, distanceFactor);

  // 计算垂直风力 - 确保值足够大以克服重力
  float verticalForce = m_Strength * distanceFactor * deltaTime * 0.3f;

  // 应用风力到角色
  character->ApplyExternalForce(verticalForce);
}

void Fan::UpdateAnimation(float deltaTime) { m_AnimationTime += deltaTime; }

void Fan::UpdateWindParticles(float deltaTime) {
  // Update existing particles
  for (auto it = m_WindParticles.begin(); it != m_WindParticles.end();) {
    it->lifeTime -= deltaTime;
    it->position.y -= it->speed * deltaTime;

    // Add some horizontal wobble
    it->position.x += std::sin(it->lifeTime * 5.0f) * 0.5f * deltaTime;

    if (it->lifeTime <= 0.0f) {
      it = m_WindParticles.erase(it);
    } else {
      ++it;
    }
  }

  // Generate new particles
  if (m_WindParticles.size() < 30 &&
      m_AnimationTime - std::floor(m_AnimationTime) < 0.1f) {
    GenerateWindParticles();
  }
}

void Fan::GenerateWindParticles() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> xDist(-m_WindWidth / 2.0f,
                                              m_WindWidth / 2.0f);
  std::uniform_real_distribution<float> yDist(0.0f, 20.0f);
  std::uniform_real_distribution<float> speedDist(40.0f, 80.0f);
  std::uniform_real_distribution<float> lifeDist(1.0f, 2.5f);

  glm::vec2 fanPos = m_Transform.translation;

  // Create 3-5 new particles
  for (int i = 0; i < 5; ++i) {
    WindParticle particle;
    particle.position = fanPos + glm::vec2(xDist(gen), -yDist(gen));
    particle.speed = speedDist(gen);
    particle.maxLifeTime = lifeDist(gen);
    particle.lifeTime = particle.maxLifeTime;
    m_WindParticles.push_back(particle);
  }
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