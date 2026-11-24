#include <iostream>
#include <vector>
#include <string>

using namespace std;

class IBuilder {
public:
    virtual ~IBuilder() {}
    virtual void ProducirParteA() const = 0;
    virtual void ProducirParteB() const = 0;
    virtual void ProducirParteC() const = 0;
};

class Producto1 {
public:
    vector<string> componentes;

    void ListaComp() const {
        cout << "Componentes: ";
        for (size_t i = 0; i < componentes.size(); i++) {
            if (i + 1 == componentes.size()) {
                cout << componentes[i];
            } else {
                cout << componentes[i] << ", ";
            }
        }
        cout << endl;
    }
};

class BuilderEspecifico : public IBuilder {
private:
    Producto1* producto;

public:
    BuilderEspecifico() {
        this->Reset();
    }

    ~BuilderEspecifico() override {
        delete producto;
    }

    void Reset() {
        this->producto = new Producto1();
    }

    void ProducirParteA() const override {
        this->producto->componentes.push_back("ParteA1");
    }

    void ProducirParteB() const override {
        this->producto->componentes.push_back("ParteB1");
    }

    void ProducirParteC() const override {
        this->producto->componentes.push_back("ParteC1");
    }

    Producto1* GetProducto() {
        Producto1* resultado = this->producto;
        this->Reset();
        return resultado;
    }
};

class Director {
private:
    IBuilder *builder;

public:
    Director() : builder(nullptr) {}

    void set_builder(IBuilder *builderPtr) {
        this->builder = builderPtr;
    }

    void BuildProductoMin() {
        this->builder->ProducirParteA();
    }

    void BuildProductoCompleto() {
        this->builder->ProducirParteA();
        this->builder->ProducirParteB();
        this->builder->ProducirParteC();
    }
};

void ClienteCode(Director& director) {
    BuilderEspecifico *builder = new BuilderEspecifico();
    director.set_builder(builder);

    cout << "Producto Basico (por Director):" << endl;
    director.BuildProductoMin();
    Producto1* p = builder->GetProducto();
    p->ListaComp();
    delete p;

    cout << "\nProducto Completo (por Director):" << endl;
    director.BuildProductoCompleto();
    p = builder->GetProducto();
    p->ListaComp();
    delete p;

    cout << "\nProducto Personalizado (sin Director, partes A y C):" << endl;
    builder->ProducirParteA();
    builder->ProducirParteC();
    p = builder->GetProducto();
    p->ListaComp();
    delete p;

    delete builder;
}

int main()
{
    Director *director = new Director();
    ClienteCode(*director);
    delete director;
    return 0;
}
