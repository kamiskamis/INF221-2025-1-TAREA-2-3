#ifndef SEQUENCE_DIFFERENCE_H
#define SEQUENCE_DIFFERENCE_H

#include <vector>
#include <string>
#include <utility>

using std::vector;
using std::string;
using std::pair;

// Calcula la matriz dp para la Longest Common Subsequence (LCS) mediante programación dinámica.
vector<vector<int>> calcularLCS(const string &s, const string &t);

// Reconstruye la secuencia LCS y obtiene las diferencias entre s y t a partir de la matriz dp.
pair<vector<string>, vector<string>> pd_diff(const string &s, const string &t);

#endif // SEQUENCE_DIFFERENCE_H