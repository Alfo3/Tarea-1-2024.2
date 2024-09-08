#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

class Programa {
private:
    int largo_operaciones;
    char* puntero_operaciones;
    int* puntero_salida;
    char* operaciones;
    char caracteres[72] = { ' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
                           'p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E',
                           'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U',
                           'V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9','.',':','+','-','<','>','[',']','!' };

public:
    Programa(int largo_operaciones) {
        this->largo_operaciones = largo_operaciones;
        operaciones = new char[largo_operaciones];
        puntero_operaciones = operaciones;
        puntero_salida = new int[largo_operaciones]();
    }
    
    void cargar_operaciones(const std::string& linea){
    int len = std::min(static_cast<int>(linea.size()), largo_operaciones);
            for (int i = 0; i < len; i++){
            operaciones[i] = linea[i];
        }
        puntero_operaciones = operaciones;
    }

    void ejecutar_operador() {
        char operador = *puntero_operaciones;

        if (operador == '>') {
            mover('>');
        }
        else if (operador == '<') {
            mover('<');
        }
        else if (operador == '+') {
            ++(*puntero_salida);
        }
        else if (operador == '-') {
            --(*puntero_salida);
        }
        else if (operador == '.') {
            std::cout << *puntero_salida << std::endl;
        }
        else if (operador == ':') {
            std::cout << obtener() << std::endl;
        }
        else if (operador == '[') {
            if (*puntero_salida == 0) {
                int temp = 1;
                while (temp > 0) {
                    ++puntero_operaciones;
                    if (*puntero_operaciones == '['){
                        temp++;
                    }
                    if (*puntero_operaciones == ']'){
                        temp--;
                    }
                }
            }
        }
        else if (operador == ']') {
            if (*puntero_salida != 0) {
                int temp = 1;
                while (temp > 0) {
                    --puntero_operaciones;
                    if (*puntero_operaciones == '['){
                        temp--;
                    }
                    if (*puntero_operaciones == ']') {
                        temp++;
                    }
                }
            }
        }
        else if (operador == '!') {
            terminar_programa();
        }
    }

    void mover(char dir) {
        int pos = 0;
        if (dir == '>') {
            if (pos < largo_operaciones - 1){
                ++puntero_salida;
                if( *puntero_salida == ' '){
                    *puntero_salida = 0;
                }
            }
            

        }
        else if (dir == '<') {
            if (pos > 0){
                --puntero_salida;
                if (*puntero_salida == ' '){
                    *puntero_salida = 0;
                }
            }
        }
    }

    char obtener() {
        return caracteres[(*puntero_salida) % 72];
    }

    void terminar_programa() {
        delete[] operaciones;
        delete[] puntero_salida;
    }

    void ejecutar() {
        while (*puntero_operaciones != '!') {
            ejecutar_operador();
            ++puntero_operaciones;
        }
    }
    ~Programa() {
        delete[] operaciones;
        delete[] puntero_salida;
    }
};

class Interprete{
private:
    int cant_programas;
    int largo_salida;
    int cargado;
    Programa** programas;  // Cambiado a puntero de punteros para manejar el array dinámico
    int* salida;

public:
    Interprete(int cant_programas, int largo_salidas){ // Constructor de la clase, en la que se almacena el arreglo del programa
        this->cant_programas = cant_programas;
        this->largo_salida = largo_salidas;
        this->cargado = -1;
        programas = new Programa*[cant_programas];  // Crear un arreglo de punteros a programas
        for(int i = 0; i < cant_programas; i++){
            programas[i] = new Programa(largo_salida);
        }
        salida = new int[largo_salida]();
    } 
    
    void cargar_programa(int n){ 
        if(n < cant_programas && n >= 0){
            cargado = n;
            std::ifstream archivo("Programas.txt");
            if (!archivo.is_open()) {
                std::cerr << "Error al abrir el archivo" << std::endl;
                return;
            }
            std::string linea;
            int linea_actual = 0;
            while (std::getline(archivo,linea)){
                if (linea_actual == n){
                    programas[cargado]->cargar_operaciones(linea);
                    break;
                }
                linea_actual++;
            }
            archivo.close();
        }
    }

    void ejecutar_programa(){
        if(cargado >= 0 && cargado < cant_programas){
            programas[cargado]->ejecutar_operador();
        }
    }

    void mostrar_programa_cargado(){   // Muestra el programa cargado
        if(cargado >= 0){
            programas[cargado]->ejecutar();
        }
    }

    void terminar_ejecucion(){
        // Evitamos liberar aquí, ya se liberará en el destructor
    }

    ~Interprete(){
        for (int i = 0; i < cant_programas; i++){
            delete programas[i];  // Liberar cada programa individualmente
        }
        delete[] programas;  // Liberar el arreglo de punteros
        delete[] salida;
    }
};

int main(){ 
    std::ifstream archivo;
    archivo.open("Programas.txt");

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo" << std::endl;
        return 1;
    }

    int max_salida, max_programas;
    char comando;

    archivo >> max_salida;
    archivo >> max_programas;
    archivo.ignore();

    Interprete interprete(max_programas, max_salida);

    while(true){
        std::cout << ">" << std::endl;
        std::cin >> comando;

        if(comando == 'c'){
            int n;
            std::cin >> n;
            interprete.cargar_programa(n);
        }else if(comando == 'e'){
            interprete.ejecutar_programa();
        }else if(comando == 'm'){
            interprete.mostrar_programa_cargado();
        }else if(comando == 's'){
            break;  // Salir del ciclo sin llamar a terminar_ejecucion
        }
        else{
            std::cout << "Comando no valido" << std::endl;
        }
    }

    archivo.close(); 
    return 0;
}
