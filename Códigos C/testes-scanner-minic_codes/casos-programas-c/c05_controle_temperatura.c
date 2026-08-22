void main() {
    float temperatura = 23.5;
    char unidade = 'C';
    bool alerta = false;

    /* Verifica a faixa segura do equipamento. */
    if (temperatura >= 30.0 || temperatura <= 5.0) {
        alerta = true;
    }

    if (alerta && unidade == 'C') {
        print("temperatura fora da faixa");
    } else {
        print("temperatura normal");
    }
}
