#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <chrono>
#include <future>
#include "algorithm/sequence_difference.h"  // Incluye el header con pd_diff

using namespace std;
namespace fs = std::filesystem;

int main() {
    // Carpeta de casos de prueba
    string folderPath = "data/dynamic_programming_input";
    if (!fs::exists(folderPath)) {
        cerr << "La carpeta no existe: " << folderPath << endl;
        return 1;
    }


    // Iterar sobre cada archivo de entrada
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            ifstream file(entry.path());
            if (!file) {
                cerr << "No se pudo abrir archivo: " << entry.path() << endl;
                continue;
            }

            string inputFilename = entry.path().filename().string();
            string outputPath = "data/dynamic_programming_output/" + inputFilename;
            ofstream outputFile(outputPath);
            if (!outputFile) {
                cerr << "No se pudo crear archivo de salida: " << outputPath << endl;
                continue;
            }

            string measurementPath = "data/measurements/" + inputFilename;
            ofstream measurementFile(measurementPath);
            if (!measurementFile) {
                cerr << "No se pudo crear archivo de medición: " << measurementPath << endl;
                continue;
            }

            cerr << "Procesando archivo: " << inputFilename << endl;

            int K;
            file >> K;
            file.ignore();
            outputFile << K << "\n";

            // Por cada instancia de caso de prueba
            for (int i = 0; i < K; i++) {
                string line1;
                getline(file, line1);
                if (line1.empty()) getline(file, line1);
                istringstream iss1(line1);
                int n;
                iss1 >> n;
                auto pos = iss1.tellg();
                if (pos == -1) {
                    size_t spacePos = line1.find(' ');
                    pos = spacePos != string::npos ? (streampos)(spacePos + 1) : (streampos)0;
                }
                string s = line1.substr((size_t)pos, n);

                string line2;
                getline(file, line2);
                istringstream iss2(line2);
                int m;
                iss2 >> m;
                auto pos2 = iss2.tellg();
                if (pos2 == -1) {
                    size_t spacePos2 = line2.find(' ');
                    pos2 = spacePos2 != string::npos ? (streampos)(spacePos2 + 1) : (streampos)0;
                }
                string t = line2.substr((size_t)pos2, m);

                // Límite arbitrario para que el método de PD no trabaje con cadenas excesivamente largas
                if (s.size() > 1000 || t.size() > 1000) {
                    outputFile << "ERROR: cadenas demasiado largas para PD (>1000 caracteres)\n";
                    measurementFile << s.size() << " " << t.size() << " TOO_LONG\n";
                    cerr << "Saltando par muy largo (" << s.size() << ", " << t.size() << ")\n";
                    continue;
                }

                // Medir el tiempo de ejecución
                auto inicio = chrono::high_resolution_clock::now();
                future<pair<vector<string>, vector<string>>> resultado =
                    async(launch::async, pd_diff, s, t);

                // Control de timeout (5 minutos)
                if (resultado.wait_for(chrono::minutes(5)) == future_status::ready) {
                    auto [dif_s, dif_t] = resultado.get();
                    auto fin = chrono::high_resolution_clock::now();
                    chrono::duration<double> duracion = fin - inicio;
                    cerr << "Tiempo de ejecución: " << duracion.count() << " segundos\n";
                    
                    // Guardar dimensiones y tiempo: <tamaño de s> <tamaño de t> <tiempo>
                    measurementFile << s.size() << " " << t.size() << " " << duracion.count() << "\n";

                    outputFile << dif_s.size() << "\n";
                    for (size_t k = 0; k < dif_s.size(); ++k) {
                        outputFile << dif_s[k] << " " << dif_t[k] << "\n";
                    }
                } else {
                    cerr << "⛔ Tiempo excedido (más de 5 minutos). Instancia abortada.\n";
                    outputFile << "ERROR: tiempo excedido (más de 5 minutos)\n";
                    measurementFile << s.size() << " " << t.size() << " TIMEOUT\n";
                }
            }

            file.close();
            outputFile.close();
            measurementFile.close();
        }
    }
    cout << "\n✅ Revisión completada: todos los archivos han sido procesados.\n" << endl;
    return 0;
}