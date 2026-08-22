# Analisador Léxico - MINIC

## Estrutura do Projeto

MiniC/
├── Py/
│   └── Lexichal.py              # Implementação do analisador léxico
├── Testes/
│   └── tests.py                 # Script para executar os testes
├── Codigos C/
│   └── testes-scanner-minic_codes/
│       └── casos-programas-c/
│           ├── c01_fibonacci.c
│           ├── c02_primos.c
│           ├── c03_media_vetor.c
│           ├── c04_menu_interativo.c
│           └── c05_controle_temperatura.c
└── resultados/                   # Pasta gerada com os resultados dos testes

## Funcionamento

O analisador léxico percorre o código fonte caractere por caractere, identificando e classificando os tokens de acordo com as regras da linguagem MINIC. Para cada token encontrado, é gerado um JSON com as seguintes informações:

{
  "type": "tipo_do_token",
  "value": "lexema_encontrado",
  "row": 0,
  "colSpan": [posicao_inicial, posicao_final]
}

## Como Executar

1. Instale a biblioteca regex:
pip install regex

2. Execute o script de testes:
python tests.py

3. Os resultados serao gerados na pasta resultados/ no formato JSON Lines (.jsonl), onde cada linha contem um token.

## Exemplo de Saida

Para o codigo:

int resultado = 42

O analisador gera:

{"type":"function","value":"int","row":0,"colSpan":[0,3]}
{"type":"var","value":"resultado","row":0,"colSpan":[4,13]}
{"type":"operator","value":"=","row":0,"colSpan":[14,15]}
{"type":"int","value":"42","row":0,"colSpan":[16,18]}