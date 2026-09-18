bool ehPrimo(int numero) {
    int divisor = 2;
    if (numero < 2) {
        return false;
    }
    while (divisor * divisor <= numero) {
        if (numero % divisor == 0) {
            return false;
        }
        divisor = divisor + 1;
    }
    return true;
}

void main() {
    int valor = 2;
    while (valor <= 20) {
        if (ehPrimo(valor)) {
            print(valor);
        }
        valor = valor + 1;
    }
}
