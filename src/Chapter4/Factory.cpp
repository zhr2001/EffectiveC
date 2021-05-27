#include <iostream>
#include <memory>
using namespace std;

enum ProductionType {
    TypeA,TypeB,TypeC
};

class production {
public:
    virtual ~production() {};
    virtual void show() = 0;
};

class PA: public production {
public:
    ~PA() { cout << "Destructing A" <<endl; };
    PA() { cout << "Constructing A" << endl; };
    void show() { cout << "Show A" << endl; }
};

class PB: public production {
public:
    ~PB() { cout << "Destructing B" <<endl; };
    PB() { cout << "Constructing B" << endl; };
    void show() { cout << "Show B" << endl; }
};

class PC: public production {
public:
    ~PC() { cout << "Destructing C" <<endl; };
    PC() { cout << "Constructing C" << endl; };
    void show() { cout << "Show C" << endl; }
};

class Factory {
public:
    static auto_ptr<production> createProduction(ProductionType type) {
        switch (type) {
            case TypeA:
                return auto_ptr<production>(new PA());
            case TypeB:
                return auto_ptr<production>(new PB());
            case TypeC:
                return auto_ptr<production>(new PC());
        }
    }
};

