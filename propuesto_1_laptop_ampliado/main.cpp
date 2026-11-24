#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Item {
public:
    virtual ~Item() {}
    virtual string GetTipo() const = 0;
    virtual string GetNombre() const = 0;
    virtual float GetPrecio() const = 0;
    virtual void GetConfiguracion() const = 0;
};

class Marca : public Item {
private:
    string nombre;
public:
    explicit Marca(string nom) : nombre(nom) {}
    string GetTipo() const override { return "Marca"; }
    string GetNombre() const override { return nombre; }
    float GetPrecio() const override { return 0.0f; }
    void GetConfiguracion() const override {
        cout << "Fabricante oficial: " << nombre << endl;
    }
};

class ColorChasis : public Item {
private:
    string color;
public:
    explicit ColorChasis(string col) : color(col) {}
    string GetTipo() const override { return "Color de chasis"; }
    string GetNombre() const override { return color; }
    float GetPrecio() const override { return 0.0f; }
    void GetConfiguracion() const override {
        cout << "Acabado exterior: " << color << endl;
    }
};

class CPU : public Item {
private:
    string modelo;
    float precio;
    string detalles;
public:
    CPU(string mod, float prec, string det) : modelo(mod), precio(prec), detalles(det) {}
    string GetTipo() const override { return "Procesador (CPU)"; }
    string GetNombre() const override { return modelo; }
    float GetPrecio() const override { return precio; }
    void GetConfiguracion() const override {
        cout << detalles << endl;
    }
};

class RAM : public Item {
private:
    string capacidad;
    float precio;
    string frecuencia;
public:
    RAM(string cap, float prec, string freq) : capacidad(cap), precio(prec), frecuencia(freq) {}
    string GetTipo() const override { return "Memoria RAM"; }
    string GetNombre() const override { return capacidad; }
    float GetPrecio() const override { return precio; }
    void GetConfiguracion() const override {
        cout << capacidad << " tecnologia DDR5 a " << frecuencia << endl;
    }
};

class Almacenamiento : public Item {
private:
    string tipo;
    string capacidad;
    float precio;
    string velocidad;
public:
    Almacenamiento(string tip, string cap, float prec, string vel)
        : tipo(tip), capacidad(cap), precio(prec), velocidad(vel) {}
    string GetTipo() const override { return "Almacenamiento"; }
    string GetNombre() const override { return tipo + " " + capacidad; }
    float GetPrecio() const override { return precio; }
    void GetConfiguracion() const override {
        cout << tipo << " " << capacidad << ", velocidad: " << velocidad << endl;
    }
};

class Pantalla : public Item {
private:
    string tipoPantalla;
    float precio;
    string resolucion;
public:
    Pantalla(string tipo, float prec, string res)
        : tipoPantalla(tipo), precio(prec), resolucion(res) {}
    string GetTipo() const override { return "Pantalla / Monitor"; }
    string GetNombre() const override { return tipoPantalla; }
    float GetPrecio() const override { return precio; }
    void GetConfiguracion() const override {
        cout << tipoPantalla << ", resolucion: " << resolucion << endl;
    }
};

class Impresora : public Item {
private:
    string modelo;
    float precio;
    string tecnologia;
public:
    Impresora(string mod, float prec, string tec)
        : modelo(mod), precio(prec), tecnologia(tec) {}
    string GetTipo() const override { return "Impresora vinculada"; }
    string GetNombre() const override { return modelo; }
    float GetPrecio() const override { return precio; }
    void GetConfiguracion() const override {
        cout << modelo << ", tipo de impresion: " << tecnologia << endl;
    }
};

class Accesorio : public Item {
private:
    string nombre;
    float precio;
    string descripcion;
public:
    Accesorio(string nom, float prec, string desc)
        : nombre(nom), precio(prec), descripcion(desc) {}
    string GetTipo() const override { return "Accesorio adicional"; }
    string GetNombre() const override { return nombre; }
    float GetPrecio() const override { return precio; }
    void GetConfiguracion() const override {
        cout << nombre << " (" << descripcion << ")" << endl;
    }
};

class Laptop {
private:
    vector<Item*> componentes;
public:
    ~Laptop() {
        for (auto item : componentes) {
            delete item;
        }
        componentes.clear();
    }

    void AgregarComponente(Item* item) {
        componentes.push_back(item);
    }

    float CalcularCostoTotal() const {
        float total = 0.0f;
        for (const auto item : componentes) {
            total += item->GetPrecio();
        }
        return total;
    }

    void MostrarFichaTecnica() const {
        cout << "================================================================" << endl;
        cout << "                  DETALLE DE LA COMPUTADORA                     " << endl;
        cout << "================================================================" << endl;
        for (const auto item : componentes) {
            cout << " [" << item->GetTipo() << "] " << item->GetNombre();
            if (item->GetPrecio() > 0.0f) {
                cout << " (S/. " << fixed << setprecision(2) << item->GetPrecio() << ")";
            }
            cout << "\n   -> ";
            item->GetConfiguracion();
        }
        cout << "----------------------------------------------------------------" << endl;
        cout << " Costo total del equipo y accesorios: S/. " << fixed << setprecision(2)
             << CalcularCostoTotal() << endl;
        cout << "================================================================\n" << endl;
    }
};

class ILaptopBuilder {
public:
    virtual ~ILaptopBuilder() {}
    virtual void Reset() = 0;
    virtual void DefinirMarca(string marca) = 0;
    virtual void DefinirColor(string color) = 0;
    virtual void AgregarCPU(CPU* cpu) = 0;
    virtual void AgregarRAM(RAM* ram) = 0;
    virtual void AgregarAlmacenamiento(Almacenamiento* disco) = 0;
    virtual void AgregarPantalla(Pantalla* pantalla) = 0;
    virtual void AgregarImpresora(Impresora* impresora) = 0;
    virtual void AgregarAccesorio(Accesorio* accesorio) = 0;
    virtual Laptop* ObtenerLaptop() = 0;
};

class LaptopBuilder : public ILaptopBuilder {
private:
    Laptop* laptopActual;
public:
    LaptopBuilder() {
        this->Reset();
    }

    ~LaptopBuilder() override {
        delete laptopActual;
    }

    void Reset() override {
        laptopActual = new Laptop();
    }

    void DefinirMarca(string marca) override {
        laptopActual->AgregarComponente(new Marca(marca));
    }

    void DefinirColor(string color) override {
        laptopActual->AgregarComponente(new ColorChasis(color));
    }

    void AgregarCPU(CPU* cpu) override {
        laptopActual->AgregarComponente(cpu);
    }

    void AgregarRAM(RAM* ram) override {
        laptopActual->AgregarComponente(ram);
    }

    void AgregarAlmacenamiento(Almacenamiento* disco) override {
        laptopActual->AgregarComponente(disco);
    }

    void AgregarPantalla(Pantalla* pantalla) override {
        laptopActual->AgregarComponente(pantalla);
    }

    void AgregarImpresora(Impresora* impresora) override {
        laptopActual->AgregarComponente(impresora);
    }

    void AgregarAccesorio(Accesorio* accesorio) override {
        laptopActual->AgregarComponente(accesorio);
    }

    Laptop* ObtenerLaptop() override {
        Laptop* resultado = laptopActual;
        this->Reset();
        return resultado;
    }
};

class DirectorLaptop {
private:
    ILaptopBuilder* builder;
public:
    explicit DirectorLaptop(ILaptopBuilder* b) : builder(b) {}

    void CambiarBuilder(ILaptopBuilder* b) {
        builder = b;
    }

    void ConstruirLaptopOficina() {
        builder->Reset();
        builder->DefinirMarca("Lenovo ThinkPad");
        builder->DefinirColor("Plata Titanio");
        builder->AgregarCPU(new CPU("Intel Core i5-1335U", 2800.00f, "10 nucleos (2P + 8E), hasta 4.60 GHz, 12 MB cache"));
        builder->AgregarRAM(new RAM("16 GB", 400.00f, "5200 MHz"));
        builder->AgregarAlmacenamiento(new Almacenamiento("SSD NVMe M.2", "512 GB", 290.00f, "3500 MB/s lectura"));
        builder->AgregarPantalla(new Pantalla("Pantalla Normal FHD IPS 15.6 pulgadas", 350.00f, "1920x1080 antirreflejo"));
        builder->AgregarImpresora(new Impresora("HP Smart Tank 580 Multifuncional", 650.00f, "Inyeccion continua WiFi"));
        builder->AgregarAccesorio(new Accesorio("Mochila ergonomica ejecutiva", 120.00f, "Nylon impermeable acolchado"));
    }

    void ConstruirLaptopGamerPlasma() {
        builder->Reset();
        builder->DefinirMarca("ASUS ROG Strix");
        builder->DefinirColor("Negro Mate con iluminacion RGB");
        builder->AgregarCPU(new CPU("AMD Ryzen 7 7840HS", 3900.00f, "8 nucleos, 16 hilos, hasta 5.10 GHz, 24 MB cache total"));
        builder->AgregarRAM(new RAM("32 GB", 750.00f, "5600 MHz Dual-Channel"));
        builder->AgregarAlmacenamiento(new Almacenamiento("SSD PCIe Gen4 NVMe", "1 TB", 480.00f, "7000 MB/s alta velocidad"));
        builder->AgregarPantalla(new Pantalla("Monitor Plasma Ultra HD 17.3 pulgadas", 1100.00f, "3840x2160, colores de alto contraste"));
        builder->AgregarAccesorio(new Accesorio("Kit Gamer Mouse Optico 12000 DPI y Pad", 180.00f, "Sensor optico de precision"));
    }
};

int main()
{
    LaptopBuilder builder;
    DirectorLaptop director(&builder);

    cout << "=== CASO 1: CONSTRUCCION VIA DIRECTOR (LAPTOP OFICINA + IMPRESORA) ===" << endl;
    director.ConstruirLaptopOficina();
    Laptop* laptopOficina = builder.ObtenerLaptop();
    laptopOficina->MostrarFichaTecnica();
    delete laptopOficina;

    cout << "=== CASO 2: CONSTRUCCION VIA DIRECTOR (LAPTOP GAMER + MONITOR PLASMA) ===" << endl;
    director.ConstruirLaptopGamerPlasma();
    Laptop* laptopGamer = builder.ObtenerLaptop();
    laptopGamer->MostrarFichaTecnica();
    delete laptopGamer;

    cout << "=== CASO 3: CONSTRUCCION PERSONALIZADA LIBRE (SIN DIRECTOR) ===" << endl;
    builder.Reset();
    builder.DefinirMarca("Dell XPS Studio");
    builder.DefinirColor("Azul Medianoche");
    builder.AgregarCPU(new CPU("Intel Core i7-13700H", 4200.00f, "14 nucleos, hasta 5.00 GHz, 24 MB cache"));
    builder.AgregarRAM(new RAM("32 GB", 750.00f, "5600 MHz"));
    builder.AgregarAlmacenamiento(new Almacenamiento("SSD NVMe PCIe", "1 TB", 480.00f, "5000 MB/s"));
    builder.AgregarAlmacenamiento(new Almacenamiento("HDD Externo Respaldo", "1 TB", 160.00f, "5400 RPM USB 3.2"));
    builder.AgregarPantalla(new Pantalla("Monitor Plasma Ultra HD 17.3 pulgadas", 1100.00f, "3840x2160 negros puros"));
    builder.AgregarImpresora(new Impresora("Brother Laser Monocromatica HL-1212W", 820.00f, "Laser toner de alta duracion"));
    builder.AgregarAccesorio(new Accesorio("Mouse Bluetooth silencioso", 95.00f, "Conexion inalambrica dual"));
    Laptop* laptopCustom = builder.ObtenerLaptop();
    laptopCustom->MostrarFichaTecnica();
    delete laptopCustom;

    return 0;
}
