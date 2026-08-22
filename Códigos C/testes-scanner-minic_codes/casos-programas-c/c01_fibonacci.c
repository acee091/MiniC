int fibonacci(int n) {
    int a = 0;
    int b = 1;
    int i = 0;
    while (i < n) {
        int proximo = a + b;
        a = b;
        b = proximo;
        i = i + 1;
    }
    return a;
}

void main() {
    int limite = 10;
    int resultado = fibonacci(limite);
    print("fibonacci calculado");
    print(resultado);
}
