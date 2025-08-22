#include <bits/stdc++.h>
#include <filesystem>
#include <future>
#include <fstream>

using namespace std;

namespace fs = std::filesystem;

// función que procesa un archivo
void procesarArchivo(const fs::path& file) {
    try {
        ifstream in(file, ios::in | ios::binary);
        if (!in) throw runtime_error("No se pudo abrir " + file.string());

        string contenido((istreambuf_iterator<char>(in)),
                               istreambuf_iterator<char>());
        in.close();

        // ejemplo de clasificación simple según contenido
        if (contenido.find("DRIVER") != string::npos)
            cout << "[Drivers] " << file.filename() << "\n";
        else if (contenido.find("PROGRAM") != string::npos)
            cout << "[Programs] " << file.filename() << "\n";
        else if (contenido.find("DLL") != string::npos)
            cout << "[DLL] " << file.filename() << "\n";
        else if (contenido.find("TRACK") != string::npos)
            cout << "[Pistas] " << file.filename() << "\n";
        else
            cout << "[Basura] " << file.filename() << "\n";

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
