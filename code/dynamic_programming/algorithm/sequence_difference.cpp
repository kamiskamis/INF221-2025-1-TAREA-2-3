#include "sequence_difference.h"
#include <algorithm> // Para std::reverse
#include <string>

// Programación dinámica para calcular la matriz LCS.
vector<vector<int>> calcularLCS(const string &s, const string &t) {
    int n = static_cast<int>(s.size());
    int m = static_cast<int>(t.size());
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            if (s[i - 1] == t[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
    return dp;
}

// Reconstrucción de la secuencia LCS y obtención de diferencias a partir de la matriz dp.
pair<vector<string>, vector<string>> pd_diff(const string &s, const string &t) {
    vector<string> dif_s, dif_t;
    auto dp = calcularLCS(s, t);

    vector<int> pos_s, pos_t;
    int i = static_cast<int>(s.size());
    int j = static_cast<int>(t.size());

    // Reconstrucción de los índices pertenecientes a la LCS.
    while (i > 0 && j > 0) {
        if (s[i - 1] == t[j - 1]) {
            pos_s.push_back(i - 1);
            pos_t.push_back(j - 1);
            i--; j--;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    std::reverse(pos_s.begin(), pos_s.end());
    std::reverse(pos_t.begin(), pos_t.end());

    // Insertar extremos para procesar las diferencias.
    pos_s.insert(pos_s.begin(), -1);
    pos_s.push_back(static_cast<int>(s.size()));
    pos_t.insert(pos_t.begin(), -1);
    pos_t.push_back(static_cast<int>(t.size()));

    for (size_t k = 0; k + 1 < pos_s.size(); ++k) {
        int start_s = pos_s[k] + 1;
        int end_s   = pos_s[k + 1];
        int start_t = pos_t[k] + 1;
        int end_t   = pos_t[k + 1];

        string sub_s = (start_s < end_s) ? s.substr(start_s, end_s - start_s) : "";
        string sub_t = (start_t < end_t) ? t.substr(start_t, end_t - start_t) : "";

        if (!sub_s.empty() || !sub_t.empty()) {
            dif_s.push_back(sub_s);
            dif_t.push_back(sub_t);
        }
    }

    return {dif_s, dif_t};
}