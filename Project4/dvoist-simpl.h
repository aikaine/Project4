#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <limits>

namespace Project4 {

    struct DualSimplexResult {
        std::vector<double> x;   // solution vector (length n)
        double objective = 0.0;
        bool optimal = false;
        std::string status;      // "optimal", "infeasible", "unbounded", "basis singular", ...
    };

    // Простейшая реализация двойственного симплекса.
    // A: m x n, b: m, c: n, basis: length m (индексы переменных, которые сейчас базисные)
    // Формат: минимизация c^T x, Ax = b, x >= 0
    inline static std::vector<std::vector<double>> invertMatrix(const std::vector<std::vector<double>>& A) {
        int n = (int)A.size();
        std::vector<std::vector<double>> M(n, std::vector<double>(2 * n, 0.0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) M[i][j] = A[i][j];
            M[i][n + i] = 1.0;
        }
        for (int col = 0; col < n; ++col) {
            int pivot = col;
            double maxv = std::abs(M[pivot][col]);
            for (int i = col + 1; i < n; ++i) {
                double av = std::abs(M[i][col]);
                if (av > maxv) { maxv = av; pivot = i; }
            }
            if (std::abs(M[pivot][col]) < 1e-12) return {}; // singular
            if (pivot != col) std::swap(M[pivot], M[col]);
            double diag = M[col][col];
            for (int j = 0; j < 2 * n; ++j) M[col][j] /= diag;
            for (int i = 0; i < n; ++i) if (i != col) {
                double factor = M[i][col];
                if (std::abs(factor) < 1e-15) continue;
                for (int j = 0; j < 2 * n; ++j) M[i][j] -= factor * M[col][j];
            }
        }
        std::vector<std::vector<double>> Inv(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) Inv[i][j] = M[i][n + j];
        return Inv;
    }

    inline static std::vector<double> matVecMul(const std::vector<std::vector<double>>& M, const std::vector<double>& v) {
        int m = (int)M.size();
        int n = (m ? (int)M[0].size() : 0);
        std::vector<double> r(m, 0.0);
        for (int i = 0; i < m; ++i) {
            double s = 0.0;
            for (int j = 0; j < n; ++j) s += M[i][j] * v[j];
            r[i] = s;
        }
        return r;
    }

    inline static std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>>& M) {
        int m = (int)M.size(), n = (m ? (int)M[0].size() : 0);
        std::vector<std::vector<double>> T(n, std::vector<double>(m, 0.0));
        for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) T[j][i] = M[i][j];
        return T;
    }

    inline static double dot(const std::vector<double>& a, const std::vector<double>& b) {
        double s = 0.0;
        int n = (int)a.size();
        for (int i = 0; i < n; ++i) s += a[i] * b[i];
        return s;
    }

    class DualSimplexSolver {
    public:
        // solve: минимизация c^T x при Ax = b, x >= 0. basis.size() == m
        // Возвращает структуру DualSimplexResult.
        static DualSimplexResult solve(std::vector<std::vector<double>> A,
            std::vector<double> b,
            std::vector<double> c,
            std::vector<int> basis) {
            DualSimplexResult res;
            int m = (int)A.size();
            if (m == 0) { res.status = "empty A"; return res; }
            int n = (int)A[0].size();
            if ((int)basis.size() != m) { res.status = "wrong basis size"; return res; }

            // Build basis matrix B (m x m)
            std::vector<std::vector<double>> B(m, std::vector<double>(m));
            for (int i = 0; i < m; ++i) for (int j = 0; j < m; ++j) B[i][j] = A[i][basis[j]];

            auto Binv = invertMatrix(B);
            if (Binv.empty()) { res.status = "basis singular"; return res; }

            auto xB = matVecMul(Binv, b);
            std::vector<double> cB(m);
            for (int i = 0; i < m; ++i) cB[i] = c[basis[i]];

            auto pi = matVecMul(transpose(Binv), cB); // dual prices (m-length)
            std::vector<double> reduced(n, 0.0);
            for (int j = 0; j < n; ++j) {
                std::vector<double> Aj(m);
                for (int i = 0; i < m; ++i) Aj[i] = A[i][j];
                double cb = dot(pi, Aj);
                reduced[j] = c[j] - cb;
            }

            // dual simplex loop
            for (int iter = 0; iter < 2000; ++iter) {
                // find leaving row: xB[i] < 0
                int leaving = -1;
                double minVal = 1e-12;
                for (int i = 0; i < m; ++i) {
                    if (xB[i] < minVal) { minVal = xB[i]; leaving = i; }
                }
                if (leaving == -1) {
                    // primal feasible -> optimal
                    res.x.assign(n, 0.0);
                    for (int i = 0; i < m; ++i) res.x[basis[i]] = xB[i];
                    res.objective = dot(c, res.x);
                    res.optimal = true;
                    res.status = "optimal";
                    return res;
                }

                // determine entering variable
                double bestRatio = std::numeric_limits<double>::infinity();
                int entering = -1;
                for (int j = 0; j < n; ++j) {
                    // compute a_ell_j = (B^{-1} * A_j)[leaving]
                    std::vector<double> Aj(m);
                    for (int i = 0; i < m; ++i) Aj[i] = A[i][j];
                    auto BinvAj = matVecMul(Binv, Aj);
                    double a_ell_j = BinvAj[leaving];
                    if (a_ell_j < -1e-12) {
                        double ratio = reduced[j] / a_ell_j; // both possibly negative -> ratio positive
                        if (ratio < bestRatio) {
                            bestRatio = ratio;
                            entering = j;
                        }
                    }
                }
                if (entering == -1) {
                    res.status = "dual infeasible (unbounded)";
                    return res;
                }

                // pivot: replace basis[leaving] with entering
                basis[leaving] = entering;
                // recompute B, Binv, xB, cB, pi, reduced
                for (int i = 0; i < m; ++i) for (int j = 0; j < m; ++j) B[i][j] = A[i][basis[j]];
                Binv = invertMatrix(B);
                if (Binv.empty()) { res.status = "basis singular after pivot"; return res; }
                xB = matVecMul(Binv, b);
                for (int i = 0; i < m; ++i) cB[i] = c[basis[i]];
                pi = matVecMul(transpose(Binv), cB);
                for (int j = 0; j < n; ++j) {
                    std::vector<double> Aj(m);
                    for (int i = 0; i < m; ++i) Aj[i] = A[i][j];
                    reduced[j] = c[j] - dot(pi, Aj);
                }
            }

            res.status = "max iterations";
            return res;
        }
    };

}
