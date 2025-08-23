#include <bits/stdc++.h>
#include <filesystem>
#include <future>
#include <fstream>

// poner el sudo apt install unzip en el readme


using namespace std;

namespace fs = std::filesystem;

int drivers = 0;
int programas = 0;
int link_library = 0;
int pistas = 0;

void exe(const fs::path& file){
    programas++;
    fs::path carpeta = "Output/System/Programs";
    string nuevo_nombre = "programa" + to_string(programas) + ".exe";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
    return;
}

void sys(const fs::path& file){
    drivers++;
    fs::path carpeta = "Output/System/Drivers";
    string nuevo_nombre = "drivers" + to_string(drivers) + ".sys";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
    return;
}

void dll(const fs::path& file){
    std::cout << "[Dynamic link-library]" << file.filename() << "\n";
}

void pista(const fs::path& file){
    pistas++;
    fs::path carpeta = "Output/Pistas";
    string nuevo_nombre = "pista" + to_string(pistas) + ".txt";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
    return;
}

void trash(const fs::path& file){
    fs::path carpeta = "Output/Basura";
    string nuevo_nombre = "basura.txt";
    fs::path destino = carpeta / nuevo_nombre;
    rename(file,destino);
    return;
}


// función que procesa un archivo
void procesarArchivo(const fs::path& file) {
    try {
        ifstream in(file, ios::in | ios::binary);
        if (!in) throw runtime_error("No se pudo abrir " + file.string());

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
    return 0;
}
