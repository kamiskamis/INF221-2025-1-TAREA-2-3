#ifndef SEQUENCE_DIFFERENCE_H
#define SEQUENCE_DIFFERENCE_H

#include <string>
#include <vector>
#include <utility>
#include <atomic>

using std::string;
using std::vector;
using std::pair;
using std::atomic;

// Función que implementa la fuerza bruta para calcular la LCS, checando la bandera cancel.
string LCS_fuerza_bruta(const string &s, const string &t, atomic<bool>& cancel);

// Función que devuelve las diferencias basado en la LCS, chequeando la bandera cancel.
pair<vector<string>, vector<string>> fb_diff(const string &s, const string &t, atomic<bool>& cancel);

#endif // SEQUENCE_DIFFERENCE_H