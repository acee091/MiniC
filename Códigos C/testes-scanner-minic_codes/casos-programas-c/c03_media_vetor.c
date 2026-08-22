float media(int valores[5]) {
    int indice = 0;
    float soma = 0.0;
    while (indice < 5) {
        soma = soma + valores[indice];
        indice = indice + 1;
    }
    return soma / 5.0;
}

void main() {
    int notas[5];
    notas[0] = 7;
    notas[1] = 8;
    notas[2] = 9;
    notas[3] = 6;
    notas[4] = 10;
    float resultado = media(notas);
    print("media final");
    print(resultado);
}
