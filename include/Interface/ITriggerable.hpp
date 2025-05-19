#ifndef ITRIGGERABLE_HPP
#define ITRIGGERABLE_HPP

class ITriggerable {
public:
  virtual void OnTriggered() = 0;
  virtual void OnReleased() = 0;
  virtual ~ITriggerable() = default;
};

#endif