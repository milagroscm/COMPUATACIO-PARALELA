#include <iostream>
#include <vector>
#include <algorithm>
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

MATRIZ multiplicacion6(const MATRIZ &a, const MATRIZ &b, int tamanio_bloque) {
    int n = a.size();
    int p = b.size();   // número de columnas de a, número de filas de b
    int m = b[0].size();    // columnas

    // creamos la matriz resultante e inicializamos todas las posiciones a cero
    MATRIZ resultado(n, vector<int> (m, 0));

    for (int bloque_fila = 0; bloque_fila < a.size(); bloque_fila += tamanio_bloque) {
        for (int bloque_columna = 0; bloque_columna < b[0].size(); bloque_columna += tamanio_bloque) {
            for (int bloque_k = 0; bloque_k < a[0].size(); bloque_k += tamanio_bloque) {
                for (int fila = bloque_fila; fila < min(bloque_fila + tamanio_bloque, (int) a.size()); fila++) {
                    for (int columna = bloque_columna; columna < min(bloque_columna + tamanio_bloque, (int) b[0].size()); columna++) {
                        for (int k = bloque_k; k < min(bloque_k + tamanio_bloque, (int) a[0].size()); k++) {
                            resultado[fila][columna] += a[fila][k] * b[k][columna];
                        }
                    }
                }
            }
        }
    }
    return resultado;
}


void mostrar(MATRIZ r) {
    int n = r.size();
    int m = r[0].size();

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

    auto start = chrono::system_clock::now();
    MATRIZ resultado = multiplicacion6(a, b, 64);

    auto end = chrono::high_resolution_clock::now();

    //mostrar(resultado);

    cout <<"Time matriz cuadrada de " << n <<": " <<chrono::duration_cast<chrono::duration<float>>(end - start).count() << endl;
    return 0;
}
