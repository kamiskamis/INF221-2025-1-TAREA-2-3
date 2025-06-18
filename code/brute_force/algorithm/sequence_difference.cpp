#include "sequence_difference.h"

std::pair<std::vector<std::string>, std::vector<std::string>>
obtenerDiferencias(const std::string& s, const std::string& t) {
    int n = (int)s.size(), m = (int)t.size();
    int inicio = 0;
    while (inicio < n && inicio < m && s[inicio] == t[inicio]) inicio++;
    int fin = 0;
    while (n - 1 - fin >= inicio && m - 1 - fin >= inicio && s[n - 1 - fin] == t[m - 1 - fin]) fin++;

    std::vector<std::string> partes_s, partes_t;
    if (inicio + fin <= n || inicio + fin <= m) {
        std::string sub_s = s.substr(inicio, n - inicio - fin);
        std::string sub_t = t.substr(inicio, m - inicio - fin);

        int i = 0, j = 0;
        while (i < (int)sub_s.size() || j < (int)sub_t.size()) {
            int ini_i = i, ini_j = j;
            while (i < (int)sub_s.size() && j < (int)sub_t.size() && sub_s[i] == sub_t[j]) {
                i++; j++;
            }
            if (ini_i < (int)sub_s.size() || ini_j < (int)sub_t.size()) {
                int sig_i = ini_i, sig_j = ini_j;
                while (sig_i < (int)sub_s.size() && (sig_j >= (int)sub_t.size() || sub_s[sig_i] != sub_t[sig_j]))
                    sig_i++;
                while (sig_j < (int)sub_t.size() && (sig_i >= (int)sub_s.size() || sub_s[sig_i] != sub_t[sig_j]))
                    sig_j++;
                partes_s.push_back(sub_s.substr(ini_i, sig_i - ini_i));
                partes_t.push_back(sub_t.substr(ini_j, sig_j - ini_j));
                i = sig_i;
                j = sig_j;
            }
        }
    }
    return {partes_s, partes_t};
}
