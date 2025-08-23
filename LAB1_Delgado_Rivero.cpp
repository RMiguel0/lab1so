#include <bits/stdc++.h>
#include <filesystem>
#include <future>
#include <fstream>

// poner el sudo apt install unzip en el readme


using namespace std;
namespace fs = std::filesystem;

// variables globales de nombre de archivos
int drivers = 0;
int programas = 0;
int link_library = 0;
int pistas = 0;
int basura = 0;

/* se recibe el archivo, se establece la ruta y nombre para
   el archivo organizado y se cambia la ubicacion con rename()

   Mismo funcionamiento para las otras funciones
*/
void exe(const fs::path& file){
    programas++;
    fs::path carpeta = "Output/System/Programs";
    string nuevo_nombre = "programa" + to_string(programas) + ".exe";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
}

void sys(const fs::path& file){
    drivers++;
    fs::path carpeta = "Output/System/Drivers";
    string nuevo_nombre = "drivers" + to_string(drivers) + ".sys";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
}

void dll(const fs::path& file){
    link_library++;
    fs::path carpeta = "Output/System/Dynamic Link-Library";
    string nuevo_nombre = "dynamic link-library" + to_string(link_library) + ".dll";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
}

void pista(const fs::path& file){
    pistas++;
    fs::path carpeta = "Output/Pistas";
    string nuevo_nombre = "pista" + to_string(pistas) + ".txt";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
}

void trash(const fs::path& file){
    basura++;
    fs::path carpeta = "Output/Basura";
    string nuevo_nombre = "basura" + to_string(basura) + ".txt";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
}


/* Se abre el archivo y se guarda en una variable tipo
   string, luego se usa el metodo .find() para que llame
   a la funcion adecuada
*/
void procesarArchivo(const fs::path& file) {
    try {
        ifstream in(file, ios::in | ios::binary);
        if (!in) throw runtime_error("No se pudo abrir :( " + file.string());

        string contenido((istreambuf_iterator<char>(in)),
                               istreambuf_iterator<char>());
        in.close();

        if (contenido.find(".exe") != string::npos)
            exe(file);
        else if (contenido.find(".sys") != string::npos)
            sys(file);
        else if (contenido.find(".dll") != string::npos)
            dll(file);
        else if (contenido.find("tipo: log") != string::npos)
            pista(file);
        else
            trash(file);

    } catch (exception& e) {
        cerr << "Error procesando " << file << ": " << e.what() << "\n";
    }
}

int main() {
    fs::create_directories("Output/System/Drivers");
    fs::create_directories("Output/System/Programs");
    fs::create_directories("Output/System/Dynamic Link-Library");
    fs::create_directories("Output/Pistas");
    fs::create_directories("Output/Basura");

    fs::create_directories("Temp");
    system("unzip -o archivos_de_prueba.zip -d Temp");

    /* Se crea un vector de promesas con los archivos para
       desplegar un procesamiento en paralelo con async, la
       funcion de procesamiento y el archivo para despues ser
       obtenidas con .get() y ser trabajadas como bloque de 
       tamaño MAX_TAREAS
    */

    const int MAX_TAREAS = 4;
    vector<future<void>> futures;

    for (auto& entry : fs::recursive_directory_iterator("Temp")) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {

            futures.push_back(async(launch::async, procesarArchivo, entry.path()));

            if (futures.size() >= MAX_TAREAS) {
                for (auto& f : futures) f.get();
                futures.clear();
            }
        }
    }

    for (auto& f : futures) f.get();

    cout << "Procesamiento completo.\n";
    
    uintmax_t n = fs::remove_all("Temp");

    return 0;
}
