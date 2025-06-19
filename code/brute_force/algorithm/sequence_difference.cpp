#include "sequence_difference.h"

string LCS_fuerza_bruta(const string& s, const string& t, atomic<bool>& cancel) {
    int n = s.size(), m = t.size();
    int max_len = 0;
    string mejor = "";

    for (int mask_s = 0; mask_s < (1 << n); ++mask_s) {
        if (cancel.load()) return "";  // Cancelación cooperativa
        string sub_s = "";
        for (int i = 0; i < n; ++i) {
            if (mask_s & (1 << i))
                sub_s += s[i];
        }
        for (int mask_t = 0; mask_t < (1 << m); ++mask_t) {
            if (cancel.load()) return "";
            string sub_t = "";
            for (int j = 0; j < m; ++j) {
                if (mask_t & (1 << j))
                    sub_t += t[j];
            }
            if (sub_s == sub_t && static_cast<int>(sub_s.size()) > max_len) {
                max_len = sub_s.size();
                mejor = sub_s;
            }
        }
    }
    return mejor;
}

pair<vector<string>, vector<string>> fb_diff(const string &s, const string &t, atomic<bool>& cancel) {
    vector<string> dif_s, dif_t;
    string lcs = LCS_fuerza_bruta(s, t, cancel);
    size_t i = 0, j = 0, k = 0;
    while (k < lcs.size()) {
        if (cancel.load()) return {dif_s, dif_t};  // Abortamos en cuanto se detecta cancelación
        string seg_s = "", seg_t = "";
        while (i < s.size() && s[i] != lcs[k]) {
            if (cancel.load()) return {dif_s, dif_t};
            seg_s += s[i++];
        }
        while (j < t.size() && t[j] != lcs[k]) {
            if (cancel.load()) return {dif_s, dif_t};
            seg_t += t[j++];
        }
        if (!seg_s.empty() || !seg_t.empty()) {
            dif_s.push_back(seg_s);
            dif_t.push_back(seg_t);
        }
        i++;
        j++;
        k++;
    }
    if (i < s.size() || j < t.size()) {
        if (cancel.load()) return {dif_s, dif_t};
        dif_s.push_back(s.substr(i));
        dif_t.push_back(t.substr(j));
    }
    return {dif_s, dif_t};
}