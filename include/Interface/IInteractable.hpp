#ifndef IINTERACTABLE_HPP
#define IINTERACTABLE_HPP

class Character;

/**
 * @brief Interface for objects that can interact with characters
 *
 * This interface defines the contract for game objects that can
 * be interacted with by player characters.
 */
class IInteractable {
public:
  /**
   * @brief Called when a character enters the interactive area
   *
   * @param character Pointer to the character that entered
   */
  virtual void OnCharacterEnter(Character *character) = 0;

  //   /**
  //    * @brief Called when a character exits the interactive area
  //    *
  //    * @param character Pointer to the character that exited
  //    */
  //   virtual void OnCharacterExit(Character *character) = 0;

  /**
   * @brief Virtual destructor
   */
  virtual ~IInteractable() = default;
};

#endif // IINTERACTABLE_HPP