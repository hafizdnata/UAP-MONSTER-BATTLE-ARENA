#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

using namespace std;

// Base Monster Class (Abstract)
class Monster {
protected:
    string name;
    int health;
    int attackPower;
    int resourcePoints;
    int maxResourcePoints;
    bool isDefending;
    int dotDamage;
    int dotTurns;
    bool isStunned;

public:
    Monster(string n, int hp, int atk, int rp) 
        : name(n), health(hp), attackPower(atk), resourcePoints(rp), 
          maxResourcePoints(rp * 10), isDefending(false), dotDamage(0), 
          dotTurns(0), isStunned(false) {}
    
    virtual ~Monster() {}
    
    void takeDamage(int damage) {
        if (isDefending) {
            damage /= 2;
            isDefending = false;
        }
        health -= damage;
        if (health < 0) health = 0;
        cout << name << " menerima " << damage << " damage! HP tersisa: " << health << endl;
    }
    
    bool isAlive() const { return health > 0; }
    
    // Pure virtual methods (must be overridden)
    virtual void basicAttack(Monster* target) = 0;
    virtual void defense() = 0;
    virtual void ultimate(Monster* target) = 0;
    
    // Method overloading
    void attack(Monster* target) {
        target->takeDamage(attackPower);
    }
    
    void attack(Monster* target, int bonusDamage) {
        target->takeDamage(attackPower + bonusDamage);
    }
    
    void applyDOT() {
        if (dotTurns > 0) {
            health -= dotDamage;
            if (health < 0) health = 0;
            cout << name << " terbakar! Menerima " << dotDamage << " DOT damage! HP: " << health << endl;
            dotTurns--;
        }
    }
    
    void regenerateRP(int amount) {
        resourcePoints += amount;
        if (resourcePoints > maxResourcePoints) resourcePoints = maxResourcePoints;
    }
    
    void setDOT(int damage, int turns) {
        dotDamage = damage;
        dotTurns = turns;
    }
    
    // Getters
    string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttackPower() const { return attackPower; }
    int getResourcePoints() const { return resourcePoints; }
    bool getIsStunned() const { return isStunned; }
    
    // Setters
    void setStunned(bool stunned) { isStunned = stunned; }
    void removeDOT() { dotDamage = 0; dotTurns = 0; }
};

// FireMonster Class (Inheritance)
class FireMonster : public Monster {
public:
    FireMonster(string n) : Monster(n, 90, 25, 10) {}
    
    void basicAttack(Monster* target) override {
        cout << name << " menggunakan Flame Strike!" << endl;
        target->takeDamage(attackPower);
        regenerateRP(8);
    }
    
    void defense() override {
        cout << name << " mengaktifkan Flame Aura! (Mengurangi 50% damage masuk)" << endl;
        isDefending = true;
        regenerateRP(6);
    }
    
    void ultimate(Monster* target) override {
        if (resourcePoints < 30) {
            throw runtime_error("Resource Points tidak cukup! Butuh 30 RP.");
        }
        cout << name << " melepaskan FIRESTORM!" << endl;
        int damage = attackPower * 5;
        target->takeDamage(damage);
        target->setDOT(10, 3);
        cout << "Target terbakar selama 3 turn!" << endl;
        resourcePoints -= 30;
    }
};

// IceMonster Class (Inheritance)
class IceMonster : public Monster {
public:
    IceMonster(string n) : Monster(n, 130, 12, 8) {}
    
    void basicAttack(Monster* target) override {
        cout << name << " menggunakan Frost Bite!" << endl;
        target->takeDamage(attackPower);
        regenerateRP(7);
    }
    
    void defense() override {
        cout << name << " mengaktifkan Ice Barrier! (Blok damage + regen 15 HP)" << endl;
        isDefending = true;
        health += 15;
        regenerateRP(5);
    }
    
    void ultimate(Monster* target) override {
        if (resourcePoints < 25) {
            throw runtime_error("Resource Points tidak cukup! Butuh 25 RP.");
        }
        cout << name << " melepaskan ABSOLUTE ZERO!" << endl;
        removeDOT();
        health += 30;
        cout << name << " menghilangkan DOT dan regenerasi 30 HP! HP sekarang: " << health << endl;
        resourcePoints -= 25;
    }
};

// EarthMonster Class (Inheritance)
class EarthMonster : public Monster {
public:
    EarthMonster(string n) : Monster(n, 100, 18, 15) {}
    
    void basicAttack(Monster* target) override {
        cout << name << " menggunakan Stone Fist!" << endl;
        target->takeDamage(attackPower);
        regenerateRP(10);
    }
    
    void defense() override {
        cout << name << " mengaktifkan Rock Solid! (Mengurangi damage masuk)" << endl;
        isDefending = true;
        regenerateRP(8);
    }
    
    void ultimate(Monster* target) override {
        if (resourcePoints < 35) {
            throw runtime_error("Resource Points tidak cukup! Butuh 35 RP.");
        }
        cout << name << " melepaskan TECTONIC SHIFT!" << endl;
        int damage = attackPower * 3;
        target->takeDamage(damage);
        target->setStunned(true);
        cout << "Target ter-STUN untuk 1 turn!" << endl;
        resourcePoints -= 35;
    }
};

// Function untuk memilih monster (Polymorphism)
Monster* selectMonster(int playerNum) {
    int choice;
    string name;
    
    cout << "\n=== Player " << playerNum << " - Pilih Monster ===" << endl;
    cout << "1. Fire Monster (HP: 90, ATK: 25, RP: 10)" << endl;
    cout << "2. Ice Monster (HP: 130, ATK: 12, RP: 8)" << endl;
    cout << "3. Earth Monster (HP: 100, ATK: 18, RP: 15)" << endl;
    cout << "Pilihan: ";
    cin >> choice;
    
    cout << "Nama monster: ";
    cin >> name;
    
    switch(choice) {
        case 1: return new FireMonster(name);
        case 2: return new IceMonster(name);
        case 3: return new EarthMonster(name);
        default: throw invalid_argument("Pilihan tidak valid!");
    }
}

void displayStatus(Monster* p1, Monster* p2) {
    cout << "\n========== STATUS ==========" << endl;
    cout << p1->getName() << " | HP: " << p1->getHealth() 
         << " | RP: " << p1->getResourcePoints() << endl;
    cout << p2->getName() << " | HP: " << p2->getHealth() 
         << " | RP: " << p2->getResourcePoints() << endl;
    cout << "===========================\n" << endl;
}

// Main Game Loop
int main() {
    Monster* player1 = nullptr;
    Monster* player2 = nullptr;
    
    try {
        cout << "======================================" << endl;
        cout << "   MONSTER BATTLE ARENA - OOP DEMO   " << endl;
        cout << "======================================" << endl;
        
        // Polymorphism - Monster* dapat menunjuk ke subclass apapun
        player1 = selectMonster(1);
        player2 = selectMonster(2);
        
        int turn = 1;
        Monster* attacker = player1;
        Monster* defender = player2;
        
        while (player1->isAlive() && player2->isAlive()) {
            cout << "\n\n========== TURN " << turn << " ==========" << endl;
            cout << "Giliran: " << attacker->getName() << endl;
            
            // Apply DOT damage
            attacker->applyDOT();
            if (!attacker->isAlive()) {
                cout << attacker->getName() << " mati karena DOT!" << endl;
                break;
            }
            
            // Check stun status
            if (attacker->getIsStunned()) {
                cout << attacker->getName() << " ter-STUN! Kehilangan giliran." << endl;
                attacker->setStunned(false);
            } else {
                displayStatus(player1, player2);
                
                cout << "\nPilih aksi:" << endl;
                cout << "1. Basic Attack" << endl;
                cout << "2. Defense" << endl;
                cout << "3. Ultimate" << endl;
                cout << "Pilihan: ";
                
                int action;
                cin >> action;
                
                // Exception handling untuk input tidak valid
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("Input harus berupa angka!");
                }
                
                try {
                    switch(action) {
                        case 1:
                            // Method overriding - setiap subclass punya implementasi berbeda
                            attacker->basicAttack(defender);
                            break;
                        case 2:
                            attacker->defense();
                            break;
                        case 3:
                            attacker->ultimate(defender);
                            break;
                        default:
                            throw invalid_argument("Pilihan tidak valid! Pilih 1-3.");
                    }
                } catch (const runtime_error& e) {
                    cout << "ERROR: " << e.what() << endl;
                    cout << "Giliran dilewati!" << endl;
                }
            }
            
            // Switch turns
            swap(attacker, defender);
            turn++;
        }
        
        // Game over
        cout << "\n\n======================================" << endl;
        cout << "           GAME OVER!" << endl;
        cout << "======================================" << endl;
        if (player1->isAlive()) {
            cout << "PEMENANG: " << player1->getName() << " (Player 1)" << endl;
        } else {
            cout << "PEMENANG: " << player2->getName() << " (Player 2)" << endl;
        }
        cout << "======================================" << endl;
        
    } catch (const exception& e) {
        cout << "\nERROR: " << e.what() << endl;
        delete player1;
        delete player2;
        return 1;
    }
    
    // Cleanup memory
    delete player1;
    delete player2;
    return 0;
}
