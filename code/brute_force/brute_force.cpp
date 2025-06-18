#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "algorithm/sequence_difference.h"
using namespace std;
namespace fs = std::filesystem;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input_folder = "./data/brute_force_input";
    string output_folder = "./data/brute_force_output";

    // Asegura que la carpeta de salida exista
    fs::create_directories(output_folder);

    for (const auto& entry : fs::directory_iterator(input_folder)) {
        string filename = entry.path().filename().string();
        string input_path = entry.path().string();

        // Procesa solo archivos que contienen "entrada_" y terminan en ".txt"
        if (filename.find("entrada_") != string::npos && filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt") {
            // Genera nombre de archivo de salida
            string numero = filename.substr(8, filename.size() - 12); // extrae número de "entrada_X.txt"
            string output_path = output_folder + "/salida_" + numero + ".txt";

            ifstream in(input_path);
            ofstream out(output_path);
            if (!in || !out) {
                cerr << "Error con archivo: " << input_path << " o " << output_path << '\n';
                continue;
            }

            int K;
            in >> K;
            out << K << '\n';

            for (int caso = 0; caso < K; ++caso) {
                int n, m;
                string s, t;
                in >> n >> s;
                in >> m >> t;

                auto [ds, dt] = obtenerDiferencias(s, t);

                out << ds.size() << '\n';
                for (size_t i = 0; i < ds.size(); ++i) {
                    out << ds[i] << ' ' << dt[i] << '\n';
                }
            }
        }
    }
    return 0;
}
