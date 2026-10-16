
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <exception>
#include <list>
#include <ctime>
#include <cstdlib>

using namespace std;

//#############################Clase exceptii ##################################

//----> excepția 1: monede insuficiente
class InsufficientCoinsException : public std::exception {
private:
   string mesaj;
public:
   InsufficientCoinsException(string numeItem, int cost, int monedeCurente) {
      mesaj = "[SHOP ERROR]: You dont have enough coins for '" + numeItem +
              "'! Cost: " + to_string(cost) + " | You have: " + to_string(monedeCurente);
   }
   const char* what() const noexcept {
      return mesaj.c_str();
   }
};
//----->exceptia 2: inventar plin;
class InventoryFullException : public std::exception {
public:
   const char* what() const noexcept  {
      return "[Inventory ERROR]: The inventory is full";
   }
};
//####################################################################################

//##############################   Clase pentru iteme ####################################


//---->clasa abstracta Item
enum Rarity {
   Common,
   Rare,
   Epic,
   Legendary
};
class Item
{
   string Name;
   int Price;
   Rarity itemRarity;
public:
   Item(string Name, int Price,Rarity itemRarity=Common);
   string getName() const;
   int getPrice() const;
   virtual int getDamage() const=0;
   virtual int getHealth() const=0;
   virtual int getSpeed() const=0;
   virtual void Details()=0;
   string getRarityString() const;
   virtual ~Item() {}
};

//----->metode clasa item
Item::Item(string Name, int Price,Rarity itemRarity)
{
   this->itemRarity = itemRarity;
   this->Name = Name;
   this->Price = Price;
}
string Item::getName() const{return this->Name;}
int Item::getPrice() const {return this->Price;}
string Item::getRarityString() const {
   switch (this->itemRarity) {
   case Common:    return "Common";
   case Rare:      return "Rare";
   case Epic:      return "Epic";
   case Legendary: return "Legendary";
   default:        return "Unknown";
   }
}
bool operator<(const Item& a, const Item& b) {
   return a.getPrice() < b.getPrice();
}

//------>clasa mostenita Attack_Item
class Attack_Item:public Item
{
   int Damage,Speed;
public:
   Attack_Item(int damage, int speed,int Price,string Name="Invalid",Rarity rarity=Common):Item(Name,Price,rarity),Damage(damage),Speed(speed){}
   int getDamage() const{return Damage;}
   int getSpeed() const{return Speed;}
   int getHealth() const{return 0;}
   void setDamage(int damage){Damage = damage;}
   void setSpeed(int speed){Speed = speed;}
   void Details();
   friend istream& operator>>(istream& in, Attack_Item& item);
};
istream& operator>>(istream& in, Attack_Item& item) {
   string Name1;
   int Damage1, Speed1, Price1;
   Rarity rarity;
   cout << "Weapon's name "; in >> Name1;
   cout << "Damage: "; in >> Damage1;
   cout << "Speed: "; in >> Speed1;
   cout << "Price (coins): "; in >> Price1;
   item = Attack_Item(Damage1, Speed1, Price1, Name1);
   return in;
}
//------>metode clasa Attack_Item
void Attack_Item::Details()
{
   cout<<"Item ["<<this->getRarityString()<<"] '"<<this->getName()<<"' has a value of "<<this->getPrice()<<" coins\n";
   cout<<"Details: ->"<<this->getSpeed()<<" BonusAttackSpeed\n";
   cout<<"         ->"<<this->getDamage()<<" BonusDamage\n\n";
}


//------->clasa mostenita Health_Item
class Health_Item:public Item
{
   int Health;
public:
   Health_Item(int health,int price ,string Name="Invalid",Rarity rarity=Common):Item(Name,price,rarity),Health(health){}
   int getHealth() const{return this->Health;}
   int getDamage() const{return 0;}
   int getSpeed() const{return 0;}
   void setHealth(int health){this->Health = health;}
   void Details();
};

//------->metode clasa Health_Item
void Health_Item::Details()
{
   cout<<"Item ["<<this->getRarityString()<<"] '"<<this->getName()<<"' has a value of "<<this->getPrice()<<" coins\n";
   cout<<"Details: ->"<<this->getHealth()<<" BonusHealth\n"<<"\n";
}


//########################################################################################


//###################################  Clasa INVENTAR  ###################################


//----->clasa Inventar
class Inventory
{
   vector<Item*> Items;
   int Capacity,NrItems;
public:
   Inventory(vector<Item*> Item={}):Capacity(20),NrItems(0){}
   void addItem(Item* item);
   void removeItem(Item* item=nullptr);
   void removeAllItems();
   bool Full();
   bool Empty();
   void ShowInventory();
   int getCapacity() const{return Capacity;};
   int getNrItems() const{return NrItems;};
   vector<Item*> getItems() const{return Items;};
};

//---->metode Inventory
bool Inventory::Full()
{
   return this->Capacity==this->NrItems;
}
bool Inventory::Empty()
{
   return this->NrItems==0;
}
void Inventory::addItem(Item* item)
{
   if (!this->Full())
   {
      cout<<item->getName()<<" has been added\n";
      this->Items.push_back(item);
      NrItems++;
   }
   else throw InventoryFullException();
}
void Inventory::removeItem(Item* item)
{
   if (!this->Empty())
   {
      if (item!=nullptr)
      {
         auto hasItem=find(Items.begin(),Items.end(),item);
         if (hasItem!=Items.end())
         {
            Items.erase(hasItem);
            NrItems--;
         }
         else cout<<"You don't have this item in your inventory\n";
      }
      else
      {  cout<<"You Lost: \n";
         Items[0]->Details();
         Items.erase(Items.begin());
         NrItems--;
      }
   }
   else cout<<"Your inventory is empty\n";
}
void Inventory::ShowInventory()
{
   if (this->Empty())
      cout<<"Your inventory is empty\n";
   else
   {
      cout<<"------------------Inventory---------------\n";
      for (Item*item :this->Items)
      {  cout<<"[]\n";
         item->Details();
      }
      cout<<"-------------------------------------------\n";
   }
}
void Inventory::removeAllItems()
{
   this->Items.clear();
   NrItems=0;
   cout<<"Your inventory is empty\n";
}

//#######################################################################


//####################################Clasa Player ###############################


//--->>Clasa Player
class Player
{
   string Name;
   int Level,Health,Damage,Experience,ExpThreshold,MissingHealth,coins;
   Inventory inventory;
public:
   Player(string Name,int Level=1,Inventory inventory={});
   Player(const Player& Player2);
   ~Player(){};
   int getMissingHealth() const {return MissingHealth;}
   int getCurrentHealth() const { return Health - MissingHealth; }
   int getDamage() const { return Damage; }
   int getLevel() const {return Level;}
   int getHealth() const {return Health;}
   int getCoins() const {return coins;}
   Inventory getInventory() const {return inventory;}
   void Coins(int c){coins-=c;}
   void PlayerHurt(int Damage);
   void PlayerHealed(int Heal);
   void LevelUp();
   void BuyItem(Item* Item);
   void DisplayPlayer(std::ostream& out);
   friend std::ostream& operator<<(std::ostream&,Player&);
   Player& operator=(const Player& Player2);
   Player& operator+=(int EXP);
};

//---->metode player
Player::Player(string Name, int Level,Inventory inventory)
{  this->coins=100;
   this->inventory=inventory;
   this->Name=Name;
   this->Level=Level;
   this->Health=100+50*(Level-1);
   this->Damage=50+25*(Level-1);
   this->Experience=0;
   this->MissingHealth=0;
   this->ExpThreshold=100+50*(Level-1);

}
Player::Player(const Player& Player2)
{
   this->Name=Player2.Name;
   this->Level=Player2.Level;
   this->Health=Player2.Health;
   this->Damage=Player2.Damage;
   this->Experience=Player2.Experience;
   this->MissingHealth=Player2.MissingHealth;
   this->ExpThreshold=Player2.ExpThreshold;
   this->coins=Player2.coins;
}
Player& Player::operator=(const Player& Player2)
{
   if (this!=&Player2)
   {
      this->Name=Player2.Name;
      this->Level=Player2.Level;
      this->Health=Player2.Health;
      this->Damage=Player2.Damage;
      this->Experience=Player2.Experience;
      this->MissingHealth=Player2.MissingHealth;
      this->ExpThreshold=Player2.ExpThreshold;
      this->coins=Player2.coins;
   }
   return *this;
}
void Player::PlayerHurt(int Damage)
{
   this->MissingHealth+=Damage;
   if (Health-MissingHealth<=0)
   {
      if (Level>1)
      {
         cout<<"You died , your level has been decreased \n";
         this->Level--;
         this->ExpThreshold-=50;
         this->Experience=0;
         this->Damage-=25;
         this->Health-=50;
      }
      else
      {
         cout<<"You died, but your level is too low so you will lose an item instead\n";
         Level=1,Health=100,this->Damage=50,Experience=0,ExpThreshold=100;
         inventory.removeItem();
      }
      this->MissingHealth=0;
   }
}
void Player::PlayerHealed(int Heal)
{
   this->MissingHealth-=Heal;
   if (MissingHealth<=0)
      MissingHealth=0;
}
void Player::LevelUp()
{  cout<<"You leveld up!!\n";
   this->Level++;
   this->Health+=50;
   this->Damage+=25;
   this->ExpThreshold+=50;
   this->MissingHealth=0;
}
Player& Player::operator+=(int EXP)
{
   this->Experience+=EXP;
   while (this->Experience>this->ExpThreshold)
   {
      this->Experience-=this->ExpThreshold;
      this->LevelUp();
   }
   return *this;
}
void Player::BuyItem(Item* Item)
{
   int value=Item->getPrice();
   if (value>this->coins)
      throw InsufficientCoinsException(Item->getName(), value, this->coins);
   else
   {  this->Health+=Item->getHealth();
      this->Damage+=Item->getDamage();
      this->inventory.addItem(Item);
      cout<<"Congrats! You bought "<<Item->getName()<<'\n';
      this->coins-=value;
   }
}
void Player::DisplayPlayer(ostream& out) {
   out << "====================================\n";
   out << "Player: " << this->Name << " | Level: " << this->Level << "\n";
   out << "Health: " << this->getCurrentHealth() << "/" << this->Health << "\n";
   out << "Damage: " << this->Damage << " | Coins: " << this->coins << "\n";
   out << "EXP: " << this->Experience << "/" << this->ExpThreshold << "\n";
   out << "====================================\n";
   this->inventory.ShowInventory();
}
ostream& operator<<(ostream& out, Player& player) {
   player.DisplayPlayer(out);
   return out;
}

//###################################################################################


//#######################################Clasa Enemy ##############################

//---->>Clasa Enemy
class Enemy
{
   string Name;
   int Health,Damage;
   static int TotalEnemies;
public:
   Enemy(string name, int health ,int damage) : Name(name), Health(health), Damage(damage){TotalEnemies++;}
   Enemy(const Enemy& Enemy1):Name(Enemy1.Name),Health(Enemy1.Health),Damage(Enemy1.Damage){TotalEnemies++;}
   Enemy& operator=(const Enemy& Enemy1);
   string getName() const { return this->Name; }
   int getHealth() const { return this->Health; }
   int getDamage() const { return this->Damage; }
   void takeDamage(int damageTaken) { this->Health -= damageTaken; }
   bool isAlive() const { return this->Health > 0; }
   virtual void Attack() const{cout<<"The enemy  attacked\n";}
   static void ShowEnemy();
   virtual ~Enemy(){TotalEnemies--;};
};
Enemy& Enemy::operator=(const Enemy& Enemy1)
{
   if (this!=&Enemy1)
   {
      this->Name=Enemy1.Name;
      this->Health=Enemy1.Health;
      this->Damage=Enemy1.Damage;
   }
   return *this;
}
int Enemy::TotalEnemies=0;
void Enemy::ShowEnemy()
{
   if (Enemy::TotalEnemies==0)
      cout<<"There are no enemies alive\n";
   else cout<<"There are "<<Enemy::TotalEnemies<<" alive , you better be carefull\n";
}
//#############################################################################


//###############################Clasa Mostenita Goblin#########################

//-->>clasa Goblin
class Goblin:public Enemy
{
   int sneakChance;
public:
   Goblin(string name,int Damage,int Health, int sneakChance):Enemy(name,Health,Damage),sneakChance(sneakChance){}
   Goblin(const Goblin& Goblin1):Enemy(Goblin1),sneakChance(Goblin1.sneakChance){}
   int getSneakChance() const { return this->sneakChance; }
   void Sneak(int v){sneakChance-=v;}
   Goblin& operator=(const Goblin& other);
   void ShowGoblin();
   void Attack();
};

//------->>metoda Goblin
Goblin& Goblin:: operator=(const Goblin& other) {
   if (this != &other) {
      Enemy::operator=(other); //
      sneakChance = other.sneakChance;
   }
   return *this;
}
void Goblin::Attack()
{
   cout<<"The Goblin ("<<this->getName()<<") has attacked\n"<<this->getDamage()<<" infllicted\n";
}
void Goblin::ShowGoblin()
{
   cout << "====================================\n";
   cout << "Goblin: " << this->getName()<<"\n";
   cout << "Health: " << this->getHealth() <<"\n";
   cout << "Damage: " << this->getDamage() <<"\n";
   cout << "SneakChance: "<< this->sneakChance <<"\n";
   cout << "====================================\n";
}
//###################################################################



//#################Clasa Combat######################


class Combat
{
   Player* player;
   Goblin* g;
public:
   Combat(Player* player, Goblin* g) : player(player), g(g){}
   string Battle(int scale);
};

string Combat::Battle(int scale)
{
   int turn=rand()%2;
                   string log;
                    cout << "You encoutered an enemy: " << g->getName() << "!\n";
                    if (turn == 0)
                    {   cout<<"The goblin will attack first\n";
                       int battle=0;
                       while (battle==0)
                       {
                          g->Attack();
                          cout << "1. Attack the enemy\n";
                          cout << "2. Try to flee\n";
                          cout << "Choice: ";
                          int actiune; cin >> actiune;
                          if (player->getMissingHealth()+g->getDamage()>=player->getHealth())
                          {
                             player->PlayerHurt(g->getDamage());
                             log="The player has died to "+g->getName()+"!";
                             break;
                          }
                          player->PlayerHurt(g->getDamage());
                          if (actiune == 1) {
                             int dodge=rand()%10*5;
                             if (g->getSneakChance()>dodge)
                             {
                                cout<<"The goblin has dodged your attack\n";
                                g->Sneak(10);
                             }
                             else
                             {
                                cout<<"You attacked the Goblin\n";
                                g->takeDamage(player->getDamage());
                             }
                             if (g->isAlive()==0)
                             {
                                cout<<"Congrats you have won!!!!!!!!!!!!!!!!\n";
                                log="The player has defeated the goblin"+g->getName()+"!";
                                *player+=(60+20*scale);
                                player->Coins(-(50+10*scale));
                                break;
                             }
                          }
                          else {
                             cout << "You ran away but you took 10 damage\n";
                             player->PlayerHurt(10);
                             log="You ran away from "+g->getName()+"!";
                             break;
                          }
                          cout<<*player<<"\n\n";
                          g->ShowGoblin();
                          cout<<"\n\n";
                       }
                    }
                   else
                   {  cout<<"You will attack first\n";
                      int battle=0;
                      while (battle==0)
                      {
                         cout << "1. Attack the enemy\n";
                         cout << "2. Try to flee\n";
                         cout << "Choice: ";
                         int actiune; cin >> actiune;
                         if (actiune == 1)
                         {
                            int dodge=rand()%10*5;
                            if (g->getSneakChance()>dodge)
                            {
                               cout<<"The goblin has dodged your attack\n";
                               g->Sneak(10);
                            }
                            else
                            {
                               cout<<"You attacked the Goblin\n";
                               g->takeDamage(player->getDamage());
                            }
                            if (g->isAlive()==0)
                            {
                               cout<<"Congrats you have won!!!!!!!!!!!!!!!!\n";
                               log="The player has defeated the goblin"+g->getName()+"!";
                               *player+=(60+20*scale);
                               player->Coins(-(50+10*scale));
                               break;
                            }
                         }
                         else
                         {
                            cout << "You ran away but you took 10 damage\n";
                            player->PlayerHurt(10);
                            log="You ran away from "+g->getName()+"!";
                            break;
                         }
                         g->Attack();
                         if (player->getMissingHealth()+g->getDamage()>=player->getHealth())
                         {
                            player->PlayerHurt(g->getDamage());
                            log="The player has died to "+g->getName()+"!";
                            break;
                         }
                            player->PlayerHurt(g->getDamage());
                         cout<<*player<<"\n\n";
                         g->ShowGoblin();
                         cout<<"\n\n";
                      }
                   }
   return log;
}
//################################Clasa Singleton GameManager####################
class GameManager
{
   bool GameRunning;
   int Difficulty;
   static GameManager* instance;
   static list<string> actionLog;
   GameManager()
   {
      GameRunning=true;
      Difficulty=1;
   }
public:
   GameManager(const GameManager&) = delete;
   void operator=(const GameManager&) = delete;
   static GameManager* getInstance();
   static void pushLog(string log);
   bool isGameRunning() const {return GameRunning; }
   void stopGame() {
      GameRunning=false;
      cout<<"\n[GameManager]: The game stopped! Thank you for playing\n";
   }
   int getDifficulty() const {return Difficulty;}
   void increaseDifficulty() {Difficulty++;}
   void showGameStatus() const;
   void showLog() const;
};

//----->metode GameManager
GameManager* GameManager::instance=NULL;
list<string> GameManager::actionLog;
GameManager* GameManager::getInstance()
{
   if (!instance)
      instance=new GameManager();
   return instance;
}
void GameManager::pushLog(string log)
{
   actionLog.push_back(log);
}
void GameManager::showGameStatus() const
{
   cout<<"\n===============================\n";
   cout<<"       GAME MANAGER STATE     \n";
   cout<<" WORLD DIFFICULTY: " << Difficulty << "\n";
   cout<<" GAME STATE " << (GameRunning ? "Playing" : "Stopped") << "\n";
   cout<<"===============================\n";
}
void GameManager::showLog() const
{
   cout<<"\n====== [ LOG ] ======\n";
   for (const string& actiune : actionLog) {
      cout<<" * "<<actiune<<"\n";
   }
   cout<<"=================================\n";
}
//################################################################################


//###################FUNCTIE TEMPLATE############################################
template <typename T>
void showAffordableItems(vector<T*>& items, int playerCoins) {
   cout<<"\n==== [ SHOP FILTER ] ====\n";
   cout<<"Your Budget"<<playerCoins<<" coins\n";
   bool found = false;
   for (T* item : items) {
      if (item != nullptr && item->getPrice() <= playerCoins) {
         item->Details();
         found = true;
      }
   }
   if (!found) {
      cout<<"You can't afford anything\n";
   }
   cout<<"=========================\n";
}


void uiMeniu() {
    cout<<"\n=========================================================\n";
    cout<<"                 GAME MENU                     \n";
    cout<<"=========================================================\n";
    cout<<" 1. Start an adventure (Random events/ Scaling enemies)\n";
    cout<<" 2. Visit the shop\n";
    cout<<" 3. Player Profile\n";
    cout<<" 4. Inspect a shop item\n";
    cout<< " 5. Supply the shop with a number of items\n";
    cout<<" 6. History LOG\n";
    cout<<" 7. Increase the game Difficulty\n";
    cout<<" 0. EXIT GAME\n";
    cout<<"---------------------------------------------------------\n";
    cout<<"INPUT THE CHOICE: ";
}

void uiShop()
{
   cout<<"-------What do you want to do here?------\n";
   cout<<"1. To view the first n items available in the shop\n";
   cout<<"2. To view what items you can afford\n";
   cout<<"3. To buy an item\n";
   cout<<"4. To leave\n";
}
void clearScreen() {
#ifdef _WIN32
   system("cls");
#else
   system("clear");
#endif
}
int main() {

    srand(static_cast<unsigned int>(time(0)));//pentru generearea random de numere
    GameManager* gm = GameManager::getInstance();
    cout<<"Welcome to GOBLIN RPG\n";
    string playerName;
    cout<<"What is your name(dont use space) ";
    cin>>playerName;
    Player player(playerName);
    vector<Item*> shopItems;
    vector<Item*> ItemBin;
    shopItems.push_back(new Attack_Item(15, 10, 30, "Iron Sword", Common));
    shopItems.push_back(new Attack_Item(70, 20, 190, "Void Sword", Legendary));
    shopItems.push_back(new Health_Item(40, 25, "Health Potion", Rare));
   shopItems.push_back(new Health_Item(100, 60, "Big Health Potion", Epic));
    gm->pushLog("The game has begun. The charcacter " + playerName + " has been initialized.");
    while (gm->isGameRunning()) {
        uiMeniu();
        int opt;
        cin >> opt;
        switch (opt) {
            case 1: {
                cout<<"\n[ADVENTURE]\n";
                int encounter = rand() % 3;

                if (encounter == 0) {
                   int scale=rand()%10;
                   Goblin g("Wild Goblin", 15+15*player.getLevel()+gm->getDifficulty()*2*scale, 30+35*player.getLevel()+gm->getDifficulty()*2*scale, 5*scale);
                   Combat C(&player,&g);
                   gm->pushLog(C.Battle(scale));
                }
                else if (encounter == 1) {
                   int rng=rand()%3;
                   switch (rng)
                   {
                      case 0:
                         {
                            cout<<"You found an old burnt martial arts book\n";
                            gm->pushLog("Player has found an old martial arts book");
                            player+=(20+10*player.getLevel());
                            break;
                         }
                   case 1:
                         {
                            cout<<"You found a magic library\n";
                            gm->pushLog("Player has found a magic library");
                            player+=(80+15*player.getLevel());
                            break;
                         }
                   case 2:
                         {
                            cout<<"You found an Ancient Temple\n";
                            gm->pushLog("Player has found an Acient Temple");
                            player+=(150+20*player.getLevel());
                            break;
                         }
                   }
                }
                else {
                    cout << "You found a safe place to rest\n";
                    cout << "You healed! "<< 15*(player.getLevel()+1) << " healthpoints\n";
                    player.PlayerHealed(15);
                    gm->pushLog(" You rested in an safe place");
                }
                break;
            }
            case 2: {
                cout << "\n[SHOP]\n";
                uiShop();
                  int action,l=1;
                  cin>>action;
                  while (l==1)
                  {
                     switch (action)
                     {
                        case 1:
                           {
                              cout<<"Please input a number from 0 to "<<shopItems.size()-1<<":\n";
                              int nr; cin>>nr;
                              if (nr >= shopItems.size())
                                 nr = shopItems.size() - 1;
                              for (int i=0; i<=nr; i++)
                                 shopItems[i]->Details();
                              uiShop();
                              cin>>action;
                              break;
                           }
                     case 2:
                           {
                              showAffordableItems(shopItems,player.getCoins());
                              uiShop();
                              cin>>action;
                              break;
                           }
                     case 3:
                           {
                              for (int i=0; i<shopItems.size(); i++)
                              {
                                 cout<<i<<". ";

                                 shopItems[i]->Details();
                              }
                              cout<<"\nPlease input the index of the item you want to buy, you can type -1 to stop\n";
                              int index; cin>>index;
                              if (index >= 0 && index < shopItems.size()) {
                                 try {
                                    player.BuyItem(shopItems[index]);
                                    gm->pushLog("The Player has bought'" + shopItems[index]->getName());
                                    ItemBin.push_back(shopItems[index]);
                                    shopItems.erase(shopItems.begin() + index);
                                 }
                                 catch (const InsufficientCoinsException& e) {
                                    cout << "\n[Transaction error]\n";
                                    cout << e.what() << "\n";
                                    gm->pushLog("Lack of coins to buy an item . The player is too poor");
                                 }
                                 catch (const InventoryFullException& e) {
                                    cout << "\n[Inventory eror]\n";
                                    cout << e.what() << "\n";
                                    gm->pushLog("The player has the inventory full");
                                 }
                              } else if (index != -1) {
                                 cout << "Invalid index.\n";
                              }
                              uiShop();
                              cin>>action;
                              break;
                           }
                     case 4:
                           {
                              l=2;
                              cout<<"You have left the shop\n";
                              break;
                           }
                     }
                  }
                  break;
            }
            case 3: {
                cout << "\n[Player Profile]\n";
                cout << player;
                gm->pushLog("The player has verified his stats");
                break;
            }
            case 4: {
                cout << "\n[OBJECT DETAILS]\n";

                if (shopItems.empty()) {
                    cout << "There are no items in the shop.\n";
                    break;
                }
                cout<<"Input the index of the item you want to inspect:0-"<<shopItems.size()-1<<"\n";
                  int index;cin>>index;
                cout << "Inspecting the"<<index+1<<" item\n";
                Item* targetItem = shopItems[index];
                if (Attack_Item* arma = dynamic_cast<Attack_Item*>(targetItem)) {
                    cout << "Object '" << arma->getName() << "' is classified as WEAPON.\n";
                    arma->Details();
                    gm->pushLog("The player has seen the details of an weapon in the shop");
                }
                else if (Health_Item* potiune = dynamic_cast<Health_Item*>(targetItem)) {
                    cout << "Object '" << potiune->getName() << "' is classified as Potion.\n";
                    potiune->Details();
                    gm->pushLog("The player has seen the details of an potion in the shop");
                }
                break;
            }
            case 5: {
                cout << "\n[Supplying the shop]\n";
                cout << "How many items do you want to add? Input  (N): ";
                int n; cin >> n;
                for (int i = 0; i < n; i++) {
                    cout << "\nItem " << i+1 << " - Select the type:\n";
                    cout << "1. Weapon (Attack Item)\n2. Potion (Health Item)\nChoice: ";
                    int tip; cin >> tip;
                    if (tip == 1) {
                        Attack_Item temp(0,0,0);
                        cin >> temp;
                        shopItems.push_back(new Attack_Item(temp));
                        cout << "The weapon has been added to the shop.\n";
                    } else if (tip == 2) {
                        string nume; int hp, pret;
                        cout << "Potion Name: ";
                        cin >> nume;
                        cout << "Health points ";
                        cin >> hp;
                        cout << "Price : ";
                        cin >> pret;
                        shopItems.push_back(new Health_Item(hp, pret, nume, Rare));
                        cout << "The potion has been added to the shop\n";
                    } else {
                        cout << "Invalid type.\n";
                    }
                }
                gm->pushLog("The player has supplied the shop with " + to_string(n) + " items");
                break;
            }
            case 6: {
                gm->showLog();
                break;
            }
            case 7:
               {
                  gm->showGameStatus();
                  cout<<"Will you increase the difficulty? 1/0\n";
                  int n;
                  cin>>n;
                  if (n == 1)
                  {
                     gm->increaseDifficulty();
                     gm->pushLog("Player has increased the difficulty");
                     cout<<"The difficulty have increased . Good luck!!!!!\n";
                  }
                  else cout<<"You made a cood choice\n";
                  break;
               }
        case 8:
               { int pass;
                  cin>>pass;
                  if (pass!=112233)
                     cout<<"You dont have acces here!\n";
                  else
                  {
                     int xp;
                     cout<<"             [ADMIN]         \n";
                     cout<<"How much xp you would like to add?\n";
                     cin>>xp;
                     player+=xp;
                  }
                 break;
               }
            case 0: {
                gm->stopGame();
                break;
            }
            default:
                cout << "Invalid option,try again\n";
        }
    }
    for(Item* it : shopItems) {
        delete it;
    }
   for (Item* it : ItemBin)
   {
      delete it;
   }
    ItemBin.clear();
    shopItems.clear();
    return 0;
}