# 50 testes do analisador sintático MINIC

Conjunto com 50 programas completos em C para avaliação de parser: 25 entradas válidas e 25 entradas sintaticamente inválidas. Todos os casos usam apenas caracteres e construções que devem ser reconhecidos pelo scanner do subconjunto MINIC.

## Organização

Cada diretório em `casos/` contém:

- `codigo.c`: programa de entrada;
- `ast.esperada.txt`: AST compacta para apresentação no terminal;
- `resultado.esperado.txt`: aceitação/rejeição e orientação do diagnóstico.

Os casos 01–25 devem ser aceitos e produzir a AST indicada. Os casos 26–50 devem ser rejeitados sem AST.

## Representação da AST

A notação é uma S-expressão legível: `Program`, `Function`, `Block`, `VarDecl`, `If`, `While`, `Return`, `ExprStmt`, `Assign`, `Binary`, `Unary`, `Call`, `Index`, `Id` e `Lit`. A ordem dos filhos é a ordem sintática/semântica esperada. `NULL` representa ausência de ramo ou valor.

## Execução

A partir do diretório que contém o seu parser:

```bash
python parser.py caminho/para/codigo.c
./parser caminho/para/codigo.c
```

Exemplo:

```bash
python parser.py testes-parser-50/casos/01_declaracao_inteira_simples/codigo.c
./parser testes-parser-50/casos/01_declaracao_inteira_simples/codigo.c
```

Compare a AST textual produzida nos casos válidos com `ast.esperada.txt`. Nos casos inválidos, confirme código de saída diferente de zero e mensagem de erro sintático. A mensagem exata pode variar entre implementações; as pistas indicam o ponto de falha esperado.

## Observação de integração

O parser Python/C do projeto do aluno precisa ser o executável que recebe código-fonte C diretamente. O conversor didático atualmente distribuído em `minic_ast_parser` recebe tokens JSONL, portanto não é usado como oráculo automático destes testes.
