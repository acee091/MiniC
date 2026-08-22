void mostrarMenu() {
    print("1 - somar");
    print("2 - sair");
}

int somar(int primeiro, int segundo) {
    return primeiro + segundo;
}

void main() {
    int opcao = 1;
    int ativo = 1;
    while (ativo == 1) {
        mostrarMenu();
        if (opcao == 1) {
            int total = somar(4, 6);
            print(total);
            opcao = 2;
        } else {
            ativo = 0;
        }
    }
}
