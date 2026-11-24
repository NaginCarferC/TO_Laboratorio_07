#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <numeric>

using namespace std;

struct Fecha {
    int dia;
    int mes;
    int anio;

    Fecha(int d = 1, int m = 1, int a = 2026) : dia(d), mes(m), anio(a) {}

    bool operator<=(const Fecha& f) const {
        if (anio != f.anio) return anio < f.anio;
        if (mes != f.mes) return mes < f.mes;
        return dia <= f.dia;
    }

    friend ostream& operator<<(ostream& os, const Fecha& f) {
        ios_base::fmtflags flags = os.flags();
        char fill = os.fill('0');
        os << right << setw(2) << f.dia << "/"
           << right << setw(2) << f.mes << "/"
           << f.anio;
        os.fill(fill);
        os.flags(flags);
        return os;
    }
};

enum class Moneda {
    SOLES,
    DOLARES
};

string nombreMoneda(Moneda m) {
    return (m == Moneda::SOLES) ? "S/." : "US$";
}

struct Transaccion {
    Fecha fecha;
    string tipo;
    double monto;
    double comisionOInteres;
    string descripcion;

    Transaccion(Fecha f, string t, double m, double c, string d)
        : fecha(f), tipo(t), monto(m), comisionOInteres(c), descripcion(d) {}
};

class Cuenta {
protected:
    string numeroCuenta;
    string tipoCuenta;
    Moneda moneda;
    vector<string> titulares;
    double saldo;
    double tasaInteresAnual;
    double deudaPendiente;
    double lineaCredito;
    vector<Transaccion> historial;

public:
    Cuenta(string tipo = "General", Moneda mon = Moneda::SOLES)
        : numeroCuenta(""), tipoCuenta(tipo), moneda(mon), saldo(0.0),
          tasaInteresAnual(0.0), deudaPendiente(0.0), lineaCredito(0.0) {}

    virtual ~Cuenta() {}

    void SetNumeroCuenta(const string& num) { numeroCuenta = num; }
    void SetMoneda(Moneda mon) { moneda = mon; }
    void SetSaldo(double s) { saldo = s; }
    void SetTasaInteres(double t) { tasaInteresAnual = t; }
    void SetLineaCredito(double l) { lineaCredito = l; }

    void AgregarTitular(const string& nombre) {
        titulares.push_back(nombre);
    }

    string GetNumeroCuenta() const { return numeroCuenta; }
    string GetTipoCuenta() const { return tipoCuenta; }
    Moneda GetMoneda() const { return moneda; }
    double GetSaldo() const { return saldo; }
    double GetDeuda() const { return deudaPendiente; }
    double GetLineaCredito() const { return lineaCredito; }
    bool EsMancomunada() const { return titulares.size() > 1; }
    const vector<string>& GetTitulares() const { return titulares; }

    virtual void Depositar(Fecha f, double monto, const string& desc = "Deposito en ventanilla/app") {
        if (monto <= 0) return;
        saldo += monto;
        historial.emplace_back(f, "DEPOSITO", monto, 0.0, desc);
    }

    virtual bool Retirar(Fecha f, double monto, const string& desc = "Retiro en cajero") {
        if (monto <= 0) return false;
        if (saldo >= monto) {
            saldo -= monto;
            historial.emplace_back(f, "RETIRO", -monto, 0.0, desc);
            return true;
        }
        cout << " [!] Saldo insuficiente en cuenta " << numeroCuenta << endl;
        return false;
    }

    virtual bool Transferir(Fecha f, double monto, double porcentajeComision, const string& destino) {
        if (monto <= 0) return false;
        double comision = monto * (porcentajeComision / 100.0);
        double totalADescontar = monto + comision;
        if (saldo >= totalADescontar) {
            saldo -= totalADescontar;
            string detalle = "Transferencia a " + destino + " (comision " + to_string(porcentajeComision).substr(0, 4) + "%)";
            historial.emplace_back(f, "TRANSFERENCIA", -monto, comision, detalle);
            return true;
        }
        cout << " [!] Saldo insuficiente para transferir S/. " << monto << " + comision" << endl;
        return false;
    }

    virtual void OtorgarPrestamo(Fecha f, double monto, double tasaPrestamo) {
        if (monto <= 0) return;
        saldo += monto;
        double interesCalculado = monto * (tasaPrestamo / 100.0);
        deudaPendiente += (monto + interesCalculado);
        string detalle = "Prestamo aprobado (tasa: " + to_string(tasaPrestamo).substr(0, 4) + "%)";
        historial.emplace_back(f, "PRESTAMO", monto, interesCalculado, detalle);
    }

    virtual bool RealizarCompraCredito(Fecha f, double monto, double tasaCompra, const string& comercio) {
        (void)f; (void)monto; (void)tasaCompra; (void)comercio;
        cout << " [!] Esta cuenta no es una tarjeta de credito" << endl;
        return false;
    }

    virtual void AdelantarDeuda(Fecha f, double monto) {
        if (monto <= 0 || deudaPendiente <= 0) return;
        double pago = min(monto, deudaPendiente);
        deudaPendiente -= pago;
        if (saldo >= pago) {
            saldo -= pago;
        }
        historial.emplace_back(f, "ADELANTO_DEUDA", -pago, 0.0, "Pago parcial / amortizacion anticipada de deuda");
    }

    virtual void LiquidarDeudaTotal(Fecha f) {
        if (deudaPendiente <= 0) {
            cout << " [i] No existe deuda pendiente en la cuenta " << numeroCuenta << endl;
            return;
        }
        double deudaOriginal = deudaPendiente;
        deudaPendiente = 0.0;
        historial.emplace_back(f, "LIQUIDACION_DEUDA", -deudaOriginal, 0.0, "Cancelacion total de deuda pendiente");
    }

    virtual void GenerarEstadoDeCuenta(Fecha fechaCorte) const {
        vector<Transaccion> filtradas;
        copy_if(historial.begin(), historial.end(), back_inserter(filtradas),
            [&fechaCorte](const Transaccion& t) {
                return t.fecha <= fechaCorte;
            });

        cout << "-------------------------------------------------------------------------" << endl;
        cout << " Cuenta: " << numeroCuenta << " | Tipo: " << tipoCuenta
             << " | Moneda: " << nombreMoneda(moneda) << endl;
        cout << " Titular(es): ";
        for (size_t i = 0; i < titulares.size(); ++i) {
            cout << titulares[i] << (i + 1 < titulares.size() ? ", " : "");
        }
        if (EsMancomunada()) cout << " [MANCOMUNADA]";
        cout << "\n Saldo disponible: " << nombreMoneda(moneda) << " " << fixed << setprecision(2) << saldo;
        if (deudaPendiente > 0) {
            cout << " | Deuda pendiente: " << nombreMoneda(moneda) << " " << deudaPendiente;
        }
        if (lineaCredito > 0) {
            cout << " | Linea de credito: " << nombreMoneda(moneda) << " " << lineaCredito;
        }
        cout << " | Tasa de interes: " << tasaInteresAnual << "%" << endl;
        cout << " Movimientos hasta el corte del " << fechaCorte << " (" << filtradas.size() << " operaciones):" << endl;

        if (filtradas.empty()) {
            cout << "   (Sin movimientos registrados hasta esta fecha)" << endl;
        } else {
            for (const auto& t : filtradas) {
                cout << "   " << t.fecha << " | " << left << setw(18) << t.tipo
                     << " | " << right << setw(10) << fixed << setprecision(2) << t.monto
                     << " | " << left << t.descripcion;
                if (t.comisionOInteres > 0) {
                    cout << " (Cargo/Interes: " << fixed << setprecision(2) << t.comisionOInteres << ")";
                }
                cout << endl;
            }
        }
    }

    friend ostream& operator<<(ostream& os, const Cuenta& c) {
        os << "[" << c.tipoCuenta << " " << c.numeroCuenta << "] Saldo: "
           << nombreMoneda(c.moneda) << " " << fixed << setprecision(2) << c.saldo
           << " (Titulares: " << c.titulares.size() << ")";
        return os;
    }
};

class CuentaAhorro : public Cuenta {
public:
    explicit CuentaAhorro(Moneda mon = Moneda::SOLES) : Cuenta("Ahorro", mon) {
        tasaInteresAnual = 0.85;
    }
};

class CuentaJoven : public Cuenta {
public:
    explicit CuentaJoven(Moneda mon = Moneda::SOLES) : Cuenta("Joven", mon) {
        tasaInteresAnual = 2.20;
    }
};

class CuentaPlus : public Cuenta {
public:
    explicit CuentaPlus(Moneda mon = Moneda::SOLES) : Cuenta("Plus", mon) {
        tasaInteresAnual = 4.50;
    }
};

class CuentaEmpresarial : public Cuenta {
public:
    explicit CuentaEmpresarial(Moneda mon = Moneda::SOLES) : Cuenta("Empresarial", mon) {
        tasaInteresAnual = 1.75;
    }
};

class TarjetaCredito : public Cuenta {
public:
    explicit TarjetaCredito(Moneda mon = Moneda::SOLES) : Cuenta("Tarjeta de credito", mon) {
        tasaInteresAnual = 18.50;
        lineaCredito = 5000.00;
        saldo = 5000.00;
    }

    bool RealizarCompraCredito(Fecha f, double monto, double tasaCompra, const string& comercio) override {
        if (monto <= 0) return false;
        if (saldo >= monto) {
            saldo -= monto;
            double interes = monto * (tasaCompra / 100.0);
            deudaPendiente += (monto + interes);
            string desc = "Compra en " + comercio + " (tasa mensual " + to_string(tasaCompra).substr(0, 4) + "%)";
            historial.emplace_back(f, "COMPRA_TARJETA", -monto, interes, desc);
            return true;
        }
        cout << " [!] Linea de credito insuficiente para compra en " << comercio << endl;
        return false;
    }
};

class ICuentaBuilder {
public:
    virtual ~ICuentaBuilder() {}
    virtual void Reset() = 0;
    virtual void DefinirNumero(const string& num) = 0;
    virtual void DefinirMoneda(Moneda mon) = 0;
    virtual void AgregarTitular(const string& nom) = 0;
    virtual void DefinirSaldoInicial(double s) = 0;
    virtual void DefinirTasaInteres(double t) = 0;
    virtual void DefinirLineaCredito(double l) = 0;
    virtual Cuenta* ObtenerCuenta() = 0;
};

template <typename TipoCuentaConcreta>
class CuentaBuilder : public ICuentaBuilder {
private:
    TipoCuentaConcreta* cuentaActual;

public:
    CuentaBuilder() {
        this->Reset();
    }

    ~CuentaBuilder() override {
        delete cuentaActual;
    }

    void Reset() override {
        cuentaActual = new TipoCuentaConcreta();
    }

    void DefinirNumero(const string& num) override {
        cuentaActual->SetNumeroCuenta(num);
    }

    void DefinirMoneda(Moneda mon) override {
        cuentaActual->SetMoneda(mon);
    }

    void AgregarTitular(const string& nom) override {
        cuentaActual->AgregarTitular(nom);
    }

    void DefinirSaldoInicial(double s) override {
        cuentaActual->SetSaldo(s);
    }

    void DefinirTasaInteres(double t) override {
        cuentaActual->SetTasaInteres(t);
    }

    void DefinirLineaCredito(double l) override {
        cuentaActual->SetLineaCredito(l);
    }

    Cuenta* ObtenerCuenta() override {
        Cuenta* prod = cuentaActual;
        this->Reset();
        return prod;
    }
};

class DirectorFinanciero {
public:
    static Cuenta* ArmarCuentaAhorroPersonal(ICuentaBuilder* b, string num, string titular, Moneda mon, double saldoIni) {
        b->Reset();
        b->DefinirNumero(num);
        b->DefinirMoneda(mon);
        b->AgregarTitular(titular);
        b->DefinirSaldoInicial(saldoIni);
        b->DefinirTasaInteres(0.95);
        return b->ObtenerCuenta();
    }

    static Cuenta* ArmarCuentaJoven(ICuentaBuilder* b, string num, string titular, double saldoIni) {
        b->Reset();
        b->DefinirNumero(num);
        b->DefinirMoneda(Moneda::SOLES);
        b->AgregarTitular(titular);
        b->DefinirSaldoInicial(saldoIni);
        b->DefinirTasaInteres(2.20);
        return b->ObtenerCuenta();
    }

    static Cuenta* ArmarCuentaPlusMancomunada(ICuentaBuilder* b, string num, const vector<string>& titulares, double saldoIni) {
        b->Reset();
        b->DefinirNumero(num);
        b->DefinirMoneda(Moneda::SOLES);
        for (const auto& t : titulares) {
            b->AgregarTitular(t);
        }
        b->DefinirSaldoInicial(saldoIni);
        b->DefinirTasaInteres(4.50);
        return b->ObtenerCuenta();
    }

    static Cuenta* ArmarTarjetaCreditoOro(ICuentaBuilder* b, string num, string titular, double linea) {
        b->Reset();
        b->DefinirNumero(num);
        b->DefinirMoneda(Moneda::SOLES);
        b->AgregarTitular(titular);
        b->DefinirLineaCredito(linea);
        b->DefinirSaldoInicial(linea);
        b->DefinirTasaInteres(19.90);
        return b->ObtenerCuenta();
    }
};

class Cliente {
private:
    string dni;
    string nombreCompleto;
    vector<Cuenta*> cuentas;

public:
    Cliente(string id, string nom) : dni(id), nombreCompleto(nom) {}

    ~Cliente() {
        for (auto c : cuentas) {
            delete c;
        }
        cuentas.clear();
    }

    void AsociarCuenta(Cuenta* c) {
        cuentas.push_back(c);
    }

    string GetNombre() const { return nombreCompleto; }
    string GetDni() const { return dni; }
    const vector<Cuenta*>& GetCuentas() const { return cuentas; }

    void ImprimirEstadoCuentaGlobal(Fecha fechaCorte) const {
        cout << "=========================================================================" << endl;
        cout << " ESTADO DE CUENTA GLOBAL - CLIENTE: " << nombreCompleto << " (DNI: " << dni << ")" << endl;
        cout << " Fecha de corte: " << fechaCorte << endl;
        cout << "=========================================================================" << endl;

        for (const auto c : cuentas) {
            c->GenerarEstadoDeCuenta(fechaCorte);
        }

        double totalSoles = accumulate(cuentas.begin(), cuentas.end(), 0.0,
            [](double acum, Cuenta* c) {
                return acum + (c->GetMoneda() == Moneda::SOLES ? c->GetSaldo() : 0.0);
            });

        double deudaSoles = accumulate(cuentas.begin(), cuentas.end(), 0.0,
            [](double acum, Cuenta* c) {
                return acum + (c->GetMoneda() == Moneda::SOLES ? c->GetDeuda() : 0.0);
            });

        cout << "-------------------------------------------------------------------------" << endl;
        cout << " RESUMEN PATRIMONIAL DEL CLIENTE EN SOLES:" << endl;
        cout << " Saldo acumulado: S/. " << fixed << setprecision(2) << totalSoles << endl;
        cout << " Deuda total pendiente: S/. " << fixed << setprecision(2) << deudaSoles << endl;
        cout << " Saldo neto consolidado: S/. " << fixed << setprecision(2) << (totalSoles - deudaSoles) << endl;
        cout << "=========================================================================\n" << endl;
    }

    void ImprimirEstadoCuentaPorTipo(Fecha fechaCorte, const string& tipoDeseado) const {
        cout << "=========================================================================" << endl;
        cout << " ESTADO DE CUENTA POR TIPO [" << tipoDeseado << "] - " << nombreCompleto << endl;
        cout << " Corte al: " << fechaCorte << endl;
        cout << "=========================================================================" << endl;

        int encontradas = 0;
        for (const auto c : cuentas) {
            if (c->GetTipoCuenta() == tipoDeseado) {
                c->GenerarEstadoDeCuenta(fechaCorte);
                encontradas++;
            }
        }

        if (encontradas == 0) {
            cout << " El cliente no registra cuentas del tipo especificado." << endl;
        }
        cout << "=========================================================================\n" << endl;
    }
};

template <typename T>
void ImprimirSeparadorTitulo(const T& mensaje) {
    cout << "\n>>> " << mensaje << " <<<\n" << endl;
}

int main()
{
    CuentaBuilder<CuentaAhorro> builderAhorro;
    CuentaBuilder<CuentaJoven> builderJoven;
    CuentaBuilder<CuentaPlus> builderPlus;
    CuentaBuilder<TarjetaCredito> builderTarjeta;

    Cliente cliente1("72839401", "Nagin Jazmin Carfer Carpio");

    ImprimirSeparadorTitulo("1. ARMADO DE CUENTAS MEDIANTE EL PATRON BUILDER");

    Cuenta* cAhorro = DirectorFinanciero::ArmarCuentaAhorroPersonal(
        &builderAhorro, "191-456789-0-12", cliente1.GetNombre(), Moneda::SOLES, 2500.00);

    Cuenta* cJoven = DirectorFinanciero::ArmarCuentaJoven(
        &builderJoven, "191-889911-0-44", cliente1.GetNombre(), 800.00);

    vector<string> titularesMancomunados = { cliente1.GetNombre(), "Carlos Andres Carfer" };
    Cuenta* cPlusMancomunada = DirectorFinanciero::ArmarCuentaPlusMancomunada(
        &builderPlus, "191-332211-1-90", titularesMancomunados, 15000.00);

    Cuenta* cTarjeta = DirectorFinanciero::ArmarTarjetaCreditoOro(
        &builderTarjeta, "4557-8812-9901-3344", cliente1.GetNombre(), 6000.00);

    cliente1.AsociarCuenta(cAhorro);
    cliente1.AsociarCuenta(cJoven);
    cliente1.AsociarCuenta(cPlusMancomunada);
    cliente1.AsociarCuenta(cTarjeta);

    for (const auto c : cliente1.GetCuentas()) {
        cout << " * Registrada exitosamente: " << *c << endl;
    }

    ImprimirSeparadorTitulo("2. OPERACIONES BANCARIAS SEGUN REGLAS DE NEGOCIO");

    cAhorro->Depositar(Fecha(1, 11, 2026), 1200.00, "Abono de honorarios profesionales");
    cAhorro->Transferir(Fecha(3, 11, 2026), 500.00, 1.5, "Cuenta BCP terceros (Soles, comision 1.5%)");
    cAhorro->OtorgarPrestamo(Fecha(5, 11, 2026), 4000.00, 9.75); // Prestamo personal 9.75% (entre 8.55% y 11.25%)

    cJoven->Depositar(Fecha(2, 11, 2026), 350.00, "Premio de concurso academico");
    cJoven->Retirar(Fecha(4, 11, 2026), 150.00, "Retiro cajero automatico");

    cPlusMancomunada->Depositar(Fecha(6, 11, 2026), 3000.00, "Aporte fondo mancomunado mensual");
    cPlusMancomunada->Transferir(Fecha(8, 11, 2026), 1000.00, 2.0, "Pago cuota hipotecaria mancomunada");

    cTarjeta->RealizarCompraCredito(Fecha(7, 11, 2026), 850.00, 18.0, "Tiendas por departamento (Compra a credito)");
    cTarjeta->RealizarCompraCredito(Fecha(10, 11, 2026), 1200.00, 21.5, "Pasajes aereos (Compra a credito)");

    ImprimirSeparadorTitulo("3. ESTADO DE CUENTA CON CORTE AL 06/11/2026");
    cliente1.ImprimirEstadoCuentaGlobal(Fecha(6, 11, 2026));

    ImprimirSeparadorTitulo("4. GESTION DE DEUDAS: AMORTIZACION ANTICIPADA Y LIQUIDACION");
    cout << " >> Amortizando S/. 1500.00 del prestamo en la cuenta de ahorro..." << endl;
    cAhorro->AdelantarDeuda(Fecha(12, 11, 2026), 1500.00);

    cout << " >> Liquidando la deuda total acumulada en la tarjeta de credito..." << endl;
    cTarjeta->LiquidarDeudaTotal(Fecha(14, 11, 2026));

    ImprimirSeparadorTitulo("5. ESTADO DE CUENTA CON CORTE POSTERIOR AL 15/11/2026");
    cliente1.ImprimirEstadoCuentaGlobal(Fecha(15, 11, 2026));

    ImprimirSeparadorTitulo("6. CONSULTA ESPECIFICA POR TIPO (SOLO CUENTAS PLUS)");
    cliente1.ImprimirEstadoCuentaPorTipo(Fecha(15, 11, 2026), "Plus");

    return 0;
}
