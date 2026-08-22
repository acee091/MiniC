# MiniC - Analisador Léxico

Implementação de um analisador léxico (scanner) para a linguagem MiniC, desenvolvido em Python. O scanner processa programas em linguagem C simplificada e gera a sequência de tokens estruturada em formato JSON.

---

## Estrutura do Projeto

```text
MiniC/
├── Py/
│   └── Lexichal.py                                  # Implementação do analisador léxico
├── Testes/
│   └── tests.py                                     # Script para executar os testes
├── Codigos C/
│   └── testes-scanner-minic_codes/
│       └── casos-programas-c/
│           ├── c01_fibonacci.c
│           ├── c02_primos.c
│           ├── c03_media_vetor.c
│           ├── c04_menu_interativo.c
│           └── c05_controle_temperatura.c
└── resultados/                                      # Pasta gerada com os resultados dos testes
```

---

## Funcionamento

O analisador léxico percorre o código fonte caractere por caractere, identificando e classificando os tokens de acordo com as regras da linguagem MINIC. Para cada token encontrado, é gerado um JSON com as seguintes informações:

```json
{
  "type": "tipo_do_token",
  "value": "lexema_encontrado",
  "row": 0,
  "colSpan": [posicao_inicial, posicao_final]
}
```

---

## Como Executar

### 1. Pré-requisitos
Certifique-se de ter o Python instalado e instale a biblioteca regex:

```bash
pip install regex
```

### 2. Execução dos Testes
Execute o script de testes:

```bash
python tests.py
```

Os resultados serão gerados na pasta `resultados/` no formato JSON Lines (`.jsonl`), onde cada linha contém um token.

---

## Exemplo de Saída

Para o código:

```c
int resultado = 42
```

O analisador gera:

```json
{"type":"function","value":"int","row":0,"colSpan":[0,3]}
{"type":"var","value":"resultado","row":0,"colSpan":[4,13]}
{"type":"operator","value":"=","row":0,"colSpan":[14,15]}
{"type":"int","value":"42","row":0,"colSpan":[16,18]}
```
