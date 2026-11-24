#include <iostream>
#include <string>
#include <vector>

using namespace std;

class item {
public:
    virtual ~item() {}
    virtual string GetName() { return ""; }
    virtual float GetPrice() { return 0.0f; }
    virtual void GetConfiguration() {}
};

class Processor : public item {
public:
    virtual ~Processor() {}
    virtual string GetName() { return string(""); }
    virtual float GetPrice() { return 0.0f; }
    virtual void GetConfiguration() {}
};

class IntelI5 : public Processor {
public:
    string GetName() override {
        return "Intel i5";
    }
    float GetPrice() override {
        return 3000.00f;
    }
    void GetConfiguration() override {
        cout << "1.50 Ghz, up to 2.0 Ghz with turbo, 2 cores, 4 threads, 8 MB cache, RAM 4 GB - 8 GB" << endl;
    }
};

class IntelI7 : public Processor {
public:
    string GetName() override {
        return "Intel i7";
    }
    float GetPrice() override {
        return 4700.00f;
    }
    void GetConfiguration() override {
        cout << "1.6 Ghz, up to 1.99 Ghz, 9na Gen, 4 cores, 8 threads, 16 MB cache, RAM 16 GB" << endl;
    }
};

class Drive : public item {
public:
    virtual ~Drive() {}
    virtual string GetName() { return string(""); }
    virtual float GetPrice() { return 0.0f; }
    virtual void GetConfiguration() {}
};

class HDD : public Drive {
public:
    string GetName() override {
        return "Hard disk drive: ";
    }
    float GetPrice() override {
        return 150.00f;
    }
    void GetConfiguration() override {
        cout << "capacity: 1 TB, data transfer 1050 mbits/s" << endl;
    }
};

class SDD : public Drive {
public:
    string GetName() override {
        return "Solid state drive: ";
    }
    float GetPrice() override {
        return 270.00f;
    }
    void GetConfiguration() override {
        cout << "capacity: 250 GB, data transfer 1800 mbits/s" << endl;
    }
};

class DisplayType : public item {
public:
    virtual ~DisplayType() {}
    virtual string GetName() { return string(""); }
    virtual float GetPrice() { return 0.0f; }
    virtual void GetConfiguration() {}
};

class Normal : public DisplayType {
public:
    string GetName() override {
        return "Non-touch screen: ";
    }
    float GetPrice() override {
        return 195.00f;
    }
    void GetConfiguration() override {
        cout << "15.6 inch FHD(1920 x 1080), plane, ....etc" << endl;
    }
};

class laptop {
public:
    ~laptop() {
        for (auto itemPtr : mLaptopParts) {
            delete itemPtr;
        }
        mLaptopParts.clear();
    }

    void AddParts(item *itemPtr) {
        mLaptopParts.push_back(itemPtr);
    }

    float GetCost() {
        float cost = 0.0f;
        for (auto itemPtr : mLaptopParts) {
            cost += itemPtr->GetPrice();
        }
        return cost;
    }

    void GetConfiguration() {
        for (size_t i = 0; i < mLaptopParts.size(); i++) {
            cout << mLaptopParts[i]->GetName() << " : ";
            mLaptopParts[i]->GetConfiguration();
        }
    }

private:
    vector<item*> mLaptopParts;
};

class laptopBuilder {
public:
    laptop *opcion1() {
        laptop *lap = new laptop();
        lap->AddParts(new IntelI5());
        lap->AddParts(new Normal());
        lap->AddParts(new HDD());
        return lap;
    }

    laptop *opcion2() {
        laptop *lap = new laptop();
        lap->AddParts(new IntelI7());
        lap->AddParts(new Normal());
        lap->AddParts(new SDD());
        return lap;
    }
};

int main()
{
    laptopBuilder cotizar;

    cout << "--- Cotizacion 1: Laptop Lenovo XY ---" << endl;
    laptop *lenovo = cotizar.opcion1();
    lenovo->GetConfiguration();
    cout << "Precio total: S/. " << lenovo->GetCost() << "\n" << endl;
    delete lenovo;

    cout << "--- Cotizacion 2: Laptop Lenovo Alta Gama ---" << endl;
    laptop *lenovoAvanzada = cotizar.opcion2();
    lenovoAvanzada->GetConfiguration();
    cout << "Precio total: S/. " << lenovoAvanzada->GetCost() << endl;
    delete lenovoAvanzada;

    return 0;
}
