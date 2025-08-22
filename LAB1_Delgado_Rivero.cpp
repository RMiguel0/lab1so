#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

void exe(auto& entry){

}

void sys(auto& entry){
    
}

void dll(auto& entry){
    
}

void txt(auto& entry){
    
}

void basura(auto& entry){
    
}



int main() {
    fs::create_directories("Temp");

    system("unzip -o archivos_de_prueba.zip -d Temp");

    fs::create_directories("Output");
    fs::create_directories("Output/System");
    fs::create_directories("Output/System/Drivers");
    fs::create_directories("Output/System/Programs");
    fs::create_directories("Output/System/Dynamic Link-Library");
    fs::create_directories("Output/Pistas");
    fs::create_directories("Output/Basura");

    for (auto& entry :fs::recursive_directory_iterator("Temp"))
    {
        if (entry.is_regular_file())
        {
            auto ext = entry.path().extension(); // ".txt", ".exe", ".sys", ".dll", etc.

            if (ext == ".exe") //Drivers
            {
                exe(entry);
            }
            else if (ext == ".sys") //programs
            {
                sys(entry);
            }
            else if (ext == ".dll") //dynamic link-library
            {
                dll(entry);
            }
            else if (ext == ".txt")
            {
                txt(entry);
            }
            else
            {
                basura();
            }
            
            
            
            
        }
        
    }
    

    return 0;
}
