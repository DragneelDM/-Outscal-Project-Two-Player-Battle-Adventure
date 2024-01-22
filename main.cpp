#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

using namespace std;

/*------------------------- Parent Class --------------------------*/

class Player {
protected:
  string Name;
  float Health;
  float Damage;
  int DmgBonus;
  int HealingCapacity;
  string VictoryMessage =
      "Defeated"; // Later used on Destructor for Showcasing Victory

  // void initializeBasedOnType(char type) {
  //   switch (type) {
  //   case 'A':
  //     Damage = 7;
  //     HealingCapacity = 7;
  //     Health = 21;
  //     break;
  //   case 'B':
  //     Damage = 15;
  //     HealingCapacity = 7;
  //     Health = 15;
  //     break;
  //   case 'C':
  //     Damage = 21;
  //     HealingCapacity = 7;
  //     Health = 7;
  //     break;
  //   default:
  //     cout << "\nInvalid Type.\n";
  //   }
  // }

public:
  /*------------------------- Constructors --------------------------*/
  bool defeated() { return Health <= 0; }

  // Default Constructor
  // Player(string name)
  //     : Name(name), Health(15), Damage(5), DmgBonus(3), HealingCapacity(5) {}

  // Hard Code Constructor
  Player(string name, int Health, int damage, int dmgBonus, int healingCapacity)
      : Name(name), Health(Health), Damage(damage), DmgBonus(dmgBonus),
        HealingCapacity(healingCapacity) {}

  // Specific Type Constructor
  // Player(string name, char type)
  //     : Name(name), Health(15), Damage(5), DmgBonus(3), HealingCapacity(5) {
  //   initializeBasedOnType(type);
  // }

  /*------------------------- Combat Logic --------------------------*/
  virtual void TakeDamage(int damage) {
    cout << Name << " Takes " << damage << " Damage" << endl;
    Health -= damage;
    cout << "\nRemaining Health: " << Health << endl;
  }

  virtual void GiveDamage(Player &myEnemy) {
    cout << endl << Name + " Attacks\n\n";
    int totalDamage = Damage + getDamage();
    cout << "Dealing " << totalDamage << " damage" << endl;
    myEnemy.TakeDamage(totalDamage);
  }

  virtual void Heal() {
    int healing = getHealing();
    cout << endl << Name << " Heals for " << healing << " Health" << endl;
    Health += healing;
  }

  virtual void SpecialFunction() {}

  /*------------------------- Getter Setters --------------------------*/
  string getName() const { return Name; }
  void setName(string name) { Name = name; }

  virtual void GetType() {
    cout << "Can't Make Player abstract because i have dependencies" << endl;
  };

  void SetVictoryMessage() { VictoryMessage = "Wins"; }

  /*------------------------- Destructor --------------------------*/
  virtual ~Player() = default;

private:
  int getDamage() const { return rand() % DmgBonus; }

  int getHealing() const { return (rand() % (HealingCapacity - 2 + 1)) + 2; }
};

/*------------------------- Child Classes --------------------------*/

class Swordsman : public Player {
public:
  Swordsman(string name) : Player(name, 15, 10, 2, 15) {
    Damage = 15;
    HealingCapacity = 7;
    Health = 15;
  }

  /* Used for Debugging
  void GetType() override { cout << "Swordsman\n" << Damage; } */

  void SpecialFunction() override { cout << "SwordAttack - Zero Damage\n"; }

  ~Swordsman() override {
    cout << Name << " , Swordsman " << VictoryMessage << endl;
  }
};

class Tanker : public Player {
public:
  Tanker(string name) : Player(name, 25, 10, 2, 25) {
    Damage = 7;
    HealingCapacity = 7;
    Health = 21;
  }

  /* Used for Debugging
   void GetType() override { cout << "Tanker\n" << Damage; } */

  void SpecialFunction() override { cout << "TankerAttack - Zero Damage\n"; }

  ~Tanker() override { cout << Name << " , Tanker " << VictoryMessage << endl; }
};

class Mage : public Player {
public:
  Mage(string name) : Player(name, 10, 25, 2, 10) {
    Damage = 21;
    HealingCapacity = 7;
    Health = 7;
  }
  /* Used for Debugging
   void GetType() override { cout << "Mage\n"; } */

  void SpecialFunction() override { cout << "MageAttack - Zero Damage\n"; }

  ~Mage() override { cout << Name << " , Mage " << VictoryMessage << endl; }
};

/*------------------------- Game Manager Class --------------------------*/

class GameManager {
  unique_ptr<Player> Player1;
  unique_ptr<Player> Player2;
  Player *currentPlayer;

public:
  GameManager(unique_ptr<Player> &Player1, unique_ptr<Player> &Player2) {
    this->Player1 = move(Player1);
    this->Player2 = move(Player2);
    // Setting Up
    currentPlayer = this->Player1.get();
  }

  // Using Pointers because i want to have the child instead of the player
  // Using Unique Pointers because why not
  static unique_ptr<Player> CreatePlayer(const string &PlayerCount) {

    // HardCoding the Intro
    if (PlayerCount == "Player1") {
      cout << "\n---------------------------------------------------------\n";
      cout << "Welcome to 2 Player Battle Adventure Game \n\n";
      cout << "Each player can choose between 3 character types. \n"
           << "Type-A - Tanker → High Health, Low Damage, Low Healing. \n"
           << "Type B - Swordsman → Avg Health, Avg Damage, Avg Healing. \n"
           << "Type C - Mage → Low Health, High Damage, Avg Healing. \n\n";
      cout << "\n---------------------------------------------------------\n";
    }

    string Name;
    char Type;
    cout << PlayerCount << " - Name your character: ";
    cin >> Name;

    cout << Name + " - Choose your character type (Type A/B/C): ";
    cin >> Type;
    cin.ignore(numeric_limits<streamsize>::max(),
               '\n'); // Had issues with cin reading unnessary Lines
    Type = toupper(Type);

    // Verifying Type
    switch (Type) {
    case 'A':
      cout << endl << Name + ", Tanker has been assigned" << endl;
      return make_unique<Tanker>(Name);
    case 'B':
      cout << endl << Name + ", Swordsman has been assigned" << endl;
      return make_unique<Swordsman>(Name);
    case 'C':
      cout << endl << Name + ", Mage has been assigned" << endl;
      return make_unique<Mage>(Name);
    default:
      cout << "\nInvalid Entry. Valid entries are 'A', 'B', and 'C' \n";
      return CreatePlayer(PlayerCount);
    }
  }

  void StartGame() {
    int input;
    cout << "\n"
         << currentPlayer->getName() << "'s Turn \n"
         << "Press 1 to attack \n"
         << "Press 2 to heal \n"
         << "Press 3 to use special ability \n"
         << "Press 4 to give up \n"
         << "Enter your choice: ";
    cin >> input;

    switch (input) {
      // ATTACK
    case 1: {
      Player &enemy =
          *((currentPlayer == Player1.get()) ? Player2 : Player1).get();
      currentPlayer->GiveDamage(enemy);
      if (enemy.defeated()) {
        GameOver();
      } else {
        // Reversing Current Player
        currentPlayer =
            (currentPlayer == Player1.get()) ? Player2.get() : Player1.get();
        StartGame();
      }
      break;
    }
      // HEAL
    case 2:
      currentPlayer->Heal();

      // Reversing Current Player
      currentPlayer =
          (currentPlayer == Player1.get()) ? Player2.get() : Player1.get();

      StartGame();
      break;

      // SPECIAL
    case 3:
      currentPlayer->SpecialFunction();

      // Reversing Current Player
      currentPlayer =
          (currentPlayer == Player1.get()) ? Player2.get() : Player1.get();

      StartGame();
      break;
      // GIVE UP
    case 4:
      // Need to reverse because the currentPlayer gave up.
      currentPlayer =
          (currentPlayer == Player1.get()) ? Player2.get() : Player1.get();

      GameOver();
      break;
    default:
      cout << "\nYour Entry is Invalid" << endl;
      StartGame();
      break;
    }
  }

  void GameOver() {
    cout << "Game Over\n\n";
    currentPlayer->SetVictoryMessage();
  }
};

int main() {

  // Really Dumb but i had a lot of dependecy issues, so I a have to do it this
  // way.
  unique_ptr<Player> Player1 = GameManager::CreatePlayer("Player1");
  unique_ptr<Player> Player2 = GameManager::CreatePlayer("Player2");
  GameManager Game(Player1, Player2);
  cout << "\n---------------------------------------------------------\n";

  cout << "Game Starts \n\n";
  Game.StartGame();

  return 0;
}
