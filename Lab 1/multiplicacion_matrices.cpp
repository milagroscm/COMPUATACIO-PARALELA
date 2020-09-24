#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;

typedef vector<vector<int> > MATRIZ;

// generar un número aleatorio entre 1 y n
int generar_numero_aleatorio(int n) {
    return (rand() % n) + 1;
}

MATRIZ generar_matriz(int filas, int columnas) {
    MATRIZ r(filas, vector<int> (columnas));

    for (int i = 0; i < filas; i++)
        for (int j = 0; j < columnas; j++)
            r[i][j] = generar_numero_aleatorio(100);
    return r;
}

MATRIZ multiplicacion3(const MATRIZ &a, const MATRIZ &b) {
    int n = a.size();
    int p = b.size();   // número de columnas de a, número de filas de b
    int m = b[0].size();

    // creamos la matriz resultante e inicializamos todas las posiciones a cero
    MATRIZ r(n, vector<int> (m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < p; k++) {
                r[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return r;
}

void mostrar(MATRIZ r) {
    int n = r.size(); // n -> numero de filas
    int m = r[0].size(); // m -> munero de columnas

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << r[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    MATRIZ a, b;
    int n = 3000;
    a = generar_matriz(n, n);
    b = generar_matriz(n, n);

    auto inicio = chrono::system_clock::now();

    MATRIZ resultado = multiplicacion3(a, b);

    auto fin = chrono::system_clock::now();


    //mostrar(multiplicacion3(a, b));

    cout <<"Time matriz cuadrada de " << n <<": " <<chrono::duration_cast<chrono::duration<float>>(fin - inicio).count() << endl;

    return 0;
}
