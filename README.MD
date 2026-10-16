    # ⚔️ Goblin RPG - C++ OOP Console Game

A text-based console RPG developed in **C++** to demonstrate the practical application of fundamental and advanced **Object-Oriented Programming (OOP)** concepts.

---

##  About the Game (Gameplay)

In **Goblin RPG**, the player creates a character and embarks on a series of randomly generated adventures. Throughout the game, the player can:
* **Explore the World (Adventure):** Encounter enemies (Goblins) whose stats scale dynamically based on the player's level and the game's difficulty, discover special locations for EXP bonuses (magic libraries, ancient temples), or find safe places to rest and recover health.
* **Engage in Turn-Based Combat:** A dynamic combat system where the attack order is decided randomly, enemies have a dodge mechanic (`sneakChance`), and the player can choose to fight or flee.
* **Manage Economy & Inventory:** Earn `coins` from battles and visit the `Shop` to purchase weapons (`Attack_Item`) and potions (`Health_Item`) classified by rarity tiers (*Common, Rare, Epic, Legendary*).
* **Interact with the Shop:** Filter available items based on the current budget, inspect specific item classifications, and dynamically supply the shop with new custom items.
* **Track History (Action Log):** All major actions and events are recorded in real-time by the `GameManager`.

---

## 🛠 Implemented OOP Concepts

This project covers the following C++ / OOP paradigms and requirements:

### 1. Encapsulation & Abstraction
* **Abstract Base Class (`Item`):** Defines a common interface for all in-game items using pure virtual methods (`getDamage()`, `getHealth()`, `getSpeed()`, `Details()`).
* Class attributes are kept `private`, with controlled access and modification provided via *getter* and *setter* methods.

### 2. Inheritance & Polymorphism (Upcasting & Downcasting)
* **`Item` Hierarchy:** Derived classes `Attack_Item` and `Health_Item` inherit from the abstract `Item` class and override its pure virtual methods.
* **`Enemy` Hierarchy:** The `Goblin` class inherits from the base `Enemy` class, extending its functionality with a `sneakChance` mechanic and overriding the virtual `Attack()` method.
* **Upcasting:** Polymorphic storage of items using `vector<Item*>` in both the player's `Inventory` and the shop (`shopItems`).
* **Downcasting (`dynamic_cast`):** Used in the shop inspection feature (Menu -> Option 4) to determine at runtime whether an `Item*` points to an `Attack_Item` (Weapon) or a `Health_Item` (Potion).

### 3. Design Patterns
* **Singleton (`GameManager`):** Ensures a single global instance manages the game state (`GameRunning`), world difficulty (`Difficulty`), and event history (`actionLog`). The copy constructor and assignment operator are explicitly deleted (`= delete`).

### 4. Exception Handling (Custom Exceptions)
* **`InsufficientCoinsException`:** Thrown when the player attempts to purchase an item without having enough coins.
* **`InventoryFullException`:** Thrown when attempting to add an item to an inventory that has reached its maximum capacity (`Capacity = 20`).
* Both classes inherit from `std::exception` and override the `what() const noexcept` method.

### 5. Generic Programming (Templates) & STL
* **Function Template (`showAffordableItems<T>`):** Generically filters and displays items from a vector of pointers that the player can afford based on their current coin balance.
* **STL Containers:** Extensive use of `std::vector` (for dynamic item and memory management) and `std::list` (for storing the action log inside `GameManager`).

### 6. Operator Overloading & The Rule of Three
* **Copy Constructors, Assignment Operators (`=`), and Destructors (including `virtual` destructors):** Properly implemented across `Player`, `Enemy`, `Goblin`, and `Item` classes to ensure safe resource and memory management.
* **Overloaded Operators:**
    * `operator<<` (displays the complete player profile and inventory).
    * `operator>>` (reads a new `Attack_Item` interactively from standard input).
    * `operator+=` (adds `EXP` to the player and automatically triggers `LevelUp()` when thresholds are met).
    * `operator<` (compares `Item` objects by price).

### 7. Static Members & Methods
* Tracks the total number of active enemies using the static attribute `Enemy::TotalEnemies` and displays the count via the static method `Enemy::ShowEnemy()`.

---

##  How to Compile and Run

### Option 1: Command Line (g++)
Make sure you have a C++ compiler installed (e.g., MinGW on Windows or GCC on Linux/macOS) and run the following commands in your terminal:

```bash
g++ -std=c++17 Main.cpp -o GoblinRPG
./GoblinRPG