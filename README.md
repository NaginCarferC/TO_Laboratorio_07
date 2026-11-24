# Práctica de Laboratorio 07: Patrón Builder en C++

**Asignatura:** Tecnología de Objetos  
**Institución:** Universidad Nacional de San Agustín (UNSA)  
**Estudiante:** Nagin Jazmin Carfer Carpio  
**Docente:** Mg. Ing. Rene Alonso Nieto Valencia  

---

## Contenido del Repositorio

Este repositorio contiene la solución completa de la guía de laboratorio sobre el patrón de diseño creacional Builder en C++:

1. `ejercicio_resuelto_a_laptop/`: Implementación corregida de la cotización y ensamblado de laptops por componentes.
2. `ejercicio_resuelto_b_builder_generico/`: Implementación corregida de la estructura clásica del patrón Builder con interfaz `IBuilder`, `BuilderEspecifico`, `Producto1` y `Director`.
3. `propuesto_1_laptop_ampliado/`: Ampliación del sistema de laptops incorporando marcas, colores, CPUs modernas, memoria RAM, monitor plasma, impresoras y accesorios mediante un Director y configuración libre.
4. `propuesto_2_cuentas_bancarias/`: Sistema bancario con cuentas de Ahorro, Joven, Plus, Empresarial y Tarjetas de crédito. Aplica Builder genérico con templates, lambdas para cortes de fecha, herencia, polimorfismo, cuentas mancomunadas y amortización de deudas.
5. `propuesto_3_automovil/`: Sistema de ensamblaje de automóviles donde se configuran piezas con color personalizado (puertas, llantas, timón, asientos, motor, espejos, vidrios), modelo, motorización y país de procedencia con menú de opciones.

---

## Requisitos de Compilación

- Compilador de C++ con soporte para C++17 (por ejemplo, GCC / MinGW `g++`).
- Terminal de comandos (PowerShell, CMD o Bash en Linux/macOS).

---

## Instrucciones de Compilación y Ejecución

Para compilar y ejecutar cada ejercicio, abre una terminal en la carpeta correspondiente y ejecuta los comandos indicados:

### 1. Ejercicio Resuelto A (Laptops)
```bash
cd ejercicio_resuelto_a_laptop
g++ -std=c++17 -Wall main.cpp -o main.exe
./main.exe
```

### 2. Ejercicio Resuelto B (Builder Genérico)
```bash
cd ejercicio_resuelto_b_builder_generico
g++ -std=c++17 -Wall main.cpp -o main.exe
./main.exe
```

### 3. Ejercicio Propuesto 1 (Laptop Ampliado)
```bash
cd propuesto_1_laptop_ampliado
g++ -std=c++17 -Wall main.cpp -o main.exe
./main.exe
```

### 4. Ejercicio Propuesto 2 (Cuentas Bancarias)
```bash
cd propuesto_2_cuentas_bancarias
g++ -std=c++17 -Wall main.cpp -o main.exe
./main.exe
```

### 5. Ejercicio Propuesto 3 (Automóvil)
```bash
cd propuesto_3_automovil
g++ -std=c++17 -Wall main.cpp -o main.exe
./main.exe
```
