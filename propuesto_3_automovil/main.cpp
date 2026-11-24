#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct PiezaAuto {
    string nombre;
    string color;
    string caracteristica;
    double precio;

    PiezaAuto(string n, string c, string det, double p)
        : nombre(n), color(c), caracteristica(det), precio(p) {}
};

class ListaComponentes {
private:
    vector<PiezaAuto> piezas;

public:
    void Agregar(const PiezaAuto& p) {
        piezas.push_back(p);
    }

    const vector<PiezaAuto>& GetPiezas() const {
        return piezas;
    }

    double CalcularTotal() const {
        double total = 0.0;
        for (const auto& p : piezas) {
            total += p.precio;
        }
        return total;
    }

    void Mostrar() const {
        cout << " Piezas y componentes instalados (con color especifico):" << endl;
        for (const auto& p : piezas) {
            cout << "   * [" << left << setw(12) << p.nombre << "] Color: "
                 << left << setw(18) << p.color
                 << " | Detalle: " << left << setw(34) << p.caracteristica
                 << " | S/. " << right << fixed << setprecision(2) << setw(8) << p.precio << endl;
        }
    }
};

class Automovil {
private:
    string modelo;
    string motorizacion;
    string colorCarroceria;
    string paisOrigen;
    double precioBase;
    ListaComponentes componentes;

public:
    Automovil() : modelo(""), motorizacion(""), colorCarroceria(""),
                  paisOrigen(""), precioBase(0.0) {}

    void SetModelo(const string& mod) { modelo = mod; }
    void SetMotorizacion(const string& mot) { motorizacion = mot; }
    void SetColorCarroceria(const string& col) { colorCarroceria = col; }
    void SetPaisOrigen(const string& pais) { paisOrigen = pais; }
    void SetPrecioBase(double pb) { precioBase = pb; }

    void AgregarPieza(const string& nombre, const string& color, const string& detalle, double precio) {
        componentes.Agregar(PiezaAuto(nombre, color, detalle, precio));
    }

    double CalcularPrecioFinal() const {
        return precioBase + componentes.CalcularTotal();
    }

    void MostrarFicha() const {
        cout << "=========================================================================" << endl;
        cout << "              FICHA DE ENSAMBLAJE DEL AUTOMOVIL                         " << endl;
        cout << "=========================================================================" << endl;
        cout << " Modelo:               " << modelo << endl;
        cout << " Motorizacion:         " << motorizacion << endl;
        cout << " Color carroceria:     " << colorCarroceria << endl;
        cout << " Pais de procedencia:  " << paisOrigen << endl;
        cout << " Precio base chasista: S/. " << fixed << setprecision(2) << precioBase << endl;
        cout << "-------------------------------------------------------------------------" << endl;
        componentes.Mostrar();
        cout << "-------------------------------------------------------------------------" << endl;
        cout << " Subtotal componentes: S/. " << fixed << setprecision(2) << componentes.CalcularTotal() << endl;
        cout << " PRECIO TOTAL FINAL:   S/. " << fixed << setprecision(2) << CalcularPrecioFinal() << endl;
        cout << "=========================================================================\n" << endl;
    }
};

class IAutomovilBuilder {
public:
    virtual ~IAutomovilBuilder() {}
    virtual void Reset() = 0;
    virtual void DefinirModelo(const string& modelo, double precioBase) = 0;
    virtual void DefinirMotorizacion(const string& motor) = 0;
    virtual void DefinirColorCarroceria(const string& color) = 0;
    virtual void DefinirPais(const string& pais) = 0;
    virtual void AgregarPuertas(int cantidad, const string& color, const string& detalle, double precio) = 0;
    virtual void AgregarLlantas(const string& tipo, const string& color, double precio) = 0;
    virtual void AgregarTimon(const string& tipo, const string& color, double precio) = 0;
    virtual void AgregarAsientos(const string& tipo, const string& color, double precio) = 0;
    virtual void AgregarMotor(const string& tipo, const string& color, double precio) = 0;
    virtual void AgregarEspejos(const string& tipo, const string& color, double precio) = 0;
    virtual void AgregarVidrios(const string& tipo, const string& color, double precio) = 0;
    virtual Automovil* ObtenerAutomovil() = 0;
};

class AutomovilBuilder : public IAutomovilBuilder {
private:
    Automovil* autoActual;

public:
    AutomovilBuilder() {
        this->Reset();
    }

    ~AutomovilBuilder() override {
        delete autoActual;
    }

    void Reset() override {
        autoActual = new Automovil();
    }

    void DefinirModelo(const string& modelo, double precioBase) override {
        autoActual->SetModelo(modelo);
        autoActual->SetPrecioBase(precioBase);
    }

    void DefinirMotorizacion(const string& motor) override {
        autoActual->SetMotorizacion(motor);
    }

    void DefinirColorCarroceria(const string& color) override {
        autoActual->SetColorCarroceria(color);
    }

    void DefinirPais(const string& pais) override {
        autoActual->SetPaisOrigen(pais);
    }

    void AgregarPuertas(int cantidad, const string& color, const string& detalle, double precio) override {
        string nom = to_string(cantidad) + " Puertas";
        autoActual->AgregarPieza(nom, color, detalle, precio);
    }

    void AgregarLlantas(const string& tipo, const string& color, double precio) override {
        autoActual->AgregarPieza("Llantas", color, tipo, precio);
    }

    void AgregarTimon(const string& tipo, const string& color, double precio) override {
        autoActual->AgregarPieza("Timon", color, tipo, precio);
    }

    void AgregarAsientos(const string& tipo, const string& color, double precio) override {
        autoActual->AgregarPieza("Asientos", color, tipo, precio);
    }

    void AgregarMotor(const string& tipo, const string& color, double precio) override {
        autoActual->AgregarPieza("Motor", color, tipo, precio);
    }

    void AgregarEspejos(const string& tipo, const string& color, double precio) override {
        autoActual->AgregarPieza("Espejos", color, tipo, precio);
    }

    void AgregarVidrios(const string& tipo, const string& color, double precio) override {
        autoActual->AgregarPieza("Vidrios", color, tipo, precio);
    }

    Automovil* ObtenerAutomovil() override {
        Automovil* resultado = autoActual;
        this->Reset();
        return resultado;
    }
};

class DirectorAutomotriz {
public:
    static void ConstruirDeportivoPremium(IAutomovilBuilder* b) {
        b->Reset();
        b->DefinirModelo("Coupe Deportivo GT-R V-Spec", 120000.00);
        b->DefinirMotorizacion("Gasolina 3.8L V6 Twin-Turbo 565 HP");
        b->DefinirColorCarroceria("Rojo Ferrari Tricapa");
        b->DefinirPais("Alemania");
        b->AgregarPuertas(2, "Rojo Ferrari", "Apertura en tijera con fibra carbono", 6500.00);
        b->AgregarLlantas("Aleacion forjada 20 pulgadas ultraligeras", "Negro Satinado", 5800.00);
        b->AgregarTimon("Deportivo volante D-cut en alcantara", "Negro y Rojo", 2200.00);
        b->AgregarAsientos("Butacas Recaro de cuero y fibra de carbono", "Rojo Carmesi", 7500.00);
        b->AgregarMotor("Bloque V6 biturbo con intercooler frontal", "Aluminio Pulido", 28000.00);
        b->AgregarEspejos("Espejos aerodinamicos con sensor de punto ciego", "Fibra de Carbono", 1800.00);
        b->AgregarVidrios("Cristal templado laminado con proteccion UV", "Ahumado Oscuro 30%", 2100.00);
    }

    static void ConstruirSUVFamiliarAventura(IAutomovilBuilder* b) {
        b->Reset();
        b->DefinirModelo("SUV Familiar Terra 4x4", 95000.00);
        b->DefinirMotorizacion("Hibrido Auto-recargable 2.5L e-AWD 245 HP");
        b->DefinirColorCarroceria("Azul Medianoche Metalizado");
        b->DefinirPais("Japon");
        b->AgregarPuertas(4, "Azul Medianoche", "Estructura reforzada con barras de impacto", 4800.00);
        b->AgregarLlantas("Todoterreno All-Terrain 18 pulgadas con rines reforzados", "Gris Titanio", 4200.00);
        b->AgregarTimon("Multifuncion forrado en cuero con mandos al volante", "Gris Grafito", 1600.00);
        b->AgregarAsientos("3 filas de asientos en cuero sintetico lavable", "Beige Arena", 5400.00);
        b->AgregarMotor("Motor hibrido dual termico-electrico ecologico", "Negro Mate", 19500.00);
        b->AgregarEspejos("Espejos electricos calefactables con camaras 360", "Azul Medianoche", 1500.00);
        b->AgregarVidrios("Vidrios de privacidad termicos con aislante acustico", "Tinte Solar 50%", 1800.00);
    }
};

void MostrarMenu() {
    cout << "=========================================================================" << endl;
    cout << "             MENU DE FABRICACION Y PERSONALIZACION DE VEHICULOS          " << endl;
    cout << "=========================================================================" << endl;
    cout << " [1] Ensamblar Deportivo Premium (Configurado por Director)" << endl;
    cout << " [2] Ensamblar SUV Familiar Aventura (Configurado por Director)" << endl;
    cout << " [3] Ensamblar Automovil Personalizado (Seleccion a medida del Cliente)" << endl;
    cout << "=========================================================================\n" << endl;
}

int main()
{
    AutomovilBuilder builder;

    MostrarMenu();

    cout << ">>> EJECUTANDO OPCION 1: DEPORTIVO PREMIUM (DIRECTOR) <<<\n" << endl;
    DirectorAutomotriz::ConstruirDeportivoPremium(&builder);
    Automovil* autoDeportivo = builder.ObtenerAutomovil();
    autoDeportivo->MostrarFicha();
    delete autoDeportivo;

    cout << ">>> EJECUTANDO OPCION 2: SUV FAMILIAR AVENTURA (DIRECTOR) <<<\n" << endl;
    DirectorAutomotriz::ConstruirSUVFamiliarAventura(&builder);
    Automovil* autoSUV = builder.ObtenerAutomovil();
    autoSUV->MostrarFicha();
    delete autoSUV;

    cout << ">>> EJECUTANDO OPCION 3: AUTOMOVIL PERSONALIZADO POR EL CLIENTE <<<\n" << endl;
    builder.Reset();
    builder.DefinirModelo("Sedan Urbano Electrificado Custom", 70000.00);
    builder.DefinirMotorizacion("100% Electrico Bateria Litio-Ferrofosfato 280 HP");
    builder.DefinirColorCarroceria("Blanco Perla");
    builder.DefinirPais("Peru");
    builder.AgregarPuertas(4, "Negro Brillante", "Puertas con manijas retractiles aerodinamicas", 5200.00);
    builder.AgregarLlantas("Llantas de perfil bajo 19 pulgadas aerodinamicas", "Plata Diamantada", 4600.00);
    builder.AgregarTimon("Timon forrado en cuero microperforado", "Negro Onix", 1850.00);
    builder.AgregarAsientos("Asientos de microfibra deportiva transpirable", "Negro con Azul", 4900.00);
    builder.AgregarMotor("Motor electrico de imanes permanentes dual", "Azul Electrico", 22000.00);
    builder.AgregarEspejos("Espejos con direccionales LED dinámicas", "Negro Brillante", 1400.00);
    builder.AgregarVidrios("Vidrios blindaje nivel II ligero con filtro polarizado", "Verde Ahumado", 3500.00);

    Automovil* autoPersonalizado = builder.ObtenerAutomovil();
    autoPersonalizado->MostrarFicha();
    delete autoPersonalizado;

    return 0;
}
