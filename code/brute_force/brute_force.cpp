#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <chrono>
#include <future>

namespace fs = std::filesystem;
using namespace std;

// Fuerza bruta para LCS
string LCS_fuerza_bruta(const string& s, const string& t) {
    int n = s.size(), m = t.size();
    int max_len = 0;
    string mejor = "";

    for (int mask_s = 0; mask_s < (1 << n); ++mask_s) {
        string sub_s = "";
        for (int i = 0; i < n; ++i)
            if (mask_s & (1 << i)) 
                sub_s += s[i];

        for (int mask_t = 0; mask_t < (1 << m); ++mask_t) {
            string sub_t = "";
            for (int j = 0; j < m; ++j)
                if (mask_t & (1 << j))
                    sub_t += t[j];

            if (sub_s == sub_t && (int)sub_s.size() > max_len) {
                max_len = sub_s.size();
                mejor = sub_s;
            }
        }
    }
    return mejor;
}

pair<vector<string>, vector<string>> fb_diff(const string& s, const string& t) {
    vector<string> dif_s, dif_t;
    string lcs = LCS_fuerza_bruta(s, t);
    size_t i = 0, j = 0, k = 0;
    while (k < lcs.size()) {
        string seg_s = "", seg_t = "";
        while (i < s.size() && s[i] != lcs[k])
            seg_s += s[i++];
        while (j < t.size() && t[j] != lcs[k])
            seg_t += t[j++];

        if (!seg_s.empty() || !seg_t.empty()) {
            dif_s.push_back(seg_s);
            dif_t.push_back(seg_t);
        }
        i++;
        j++;
        k++;
    }

    if (i < s.size() || j < t.size()) {
        dif_s.push_back(s.substr(i));
        dif_t.push_back(t.substr(j));
    }

    return {dif_s, dif_t};
}

int main() {
    // Ruta a la carpeta de entrada (ajústala según tu estructura)
    string folderPath = "data/brute_force_input/";

    if (!fs::exists(folderPath)) {
        cerr << "La carpeta no existe: " << folderPath << endl;
        return 1;
    }

   

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            ifstream file(entry.path());
            if (!file) {
                cerr << "No se pudo abrir archivo: " << entry.path() << endl;
                continue;
            }

            string inputFilename = entry.path().filename().string();
            string outputPath = "data/brute_force_output/" + inputFilename;
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

            for (int i = 0; i < K; i++) {
                string line1;
                getline(file, line1);
                if (line1.empty())
                    getline(file, line1);
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

                // Comprobar límite de longitud arbitrario para brute_force
                if (s.size() > 30 || t.size() > 30) {
                    outputFile << "ERROR: cadenas demasiado largas para fuerza bruta (>20 caracteres)\n";
                    measurementFile << s.size() << " " << t.size() << " TOO_LONG\n";
                    cerr << "Saltando par muy largo (" << s.size() << ", " << t.size() << ")\n";
                    continue;
                }

                auto inicio = chrono::high_resolution_clock::now();
                future<pair<vector<string>, vector<string>>> resultado =
                    async(launch::async, fb_diff, s, t);

                if (resultado.wait_for(chrono::minutes(5)) == future_status::ready) {
                    auto [dif_s, dif_t] = resultado.get();
                    auto fin = chrono::high_resolution_clock::now();
                    chrono::duration<double> duracion = fin - inicio;
                    cerr << "Tiempo de ejecución: " << duracion.count() << " segundos\n";
                    
                    // Guardar dimensiones y tiempo en el archivo de mediciones
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