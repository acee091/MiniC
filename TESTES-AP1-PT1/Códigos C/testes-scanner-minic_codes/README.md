# Testes do analisador léxico da MINIC

Conjunto de entradas e saídas esperadas para validar o scanner da linguagem MINIC.

## Formato

Cada caso possui:

- um arquivo `.minic` com a entrada do scanner;
- um arquivo `.expected.jsonl` com um token JSON por linha, na ordem esperada;
- nos casos inválidos, um arquivo `.errors.jsonl` com os diagnósticos mínimos esperados.

Formato de token:

```json
{"token":"INT","lexeme":"int","attribute":null,"line":1,"column":1}
```

`EOF` aparece sempre como último token. Espaços, quebras de linha e comentários não geram tokens, mas alteram linha e coluna.

## Convenções adotadas

- Palavras reservadas: `int`, `float`, `bool`, `char`, `void`, `if`, `else`, `while`, `for`, `return`, `break`, `continue`, `true`, `false`, `print`, `read`.
- Identificadores: `[A-Za-z_][A-Za-z0-9_]*`.
- Inteiros: `[0-9]+`, token `INT_LIT`, atributo numérico.
- Reais: `[0-9]+\\.[0-9]+`, token `FLOAT_LIT`, atributo numérico.
- Operadores compostos têm prioridade sobre os simples: `==`, `!=`, `<=`, `>=`, `&&`, `||`.
- Delimitadores: `(` `)` `[` `]` `{` `}` `,` `;`.
- Comentários `//` e `/* ... */` são ignorados.
- `char` e `string` são testados como identificadores de tipo reservado apenas quando aparecem na especificação; literais de caractere/string ficam em casos de fronteira, pois a apostila não fixa seus nomes de token. Neste pacote, a convenção opcional é `CHAR_LIT` e `STRING_LIT`.
- A coluna começa em 1.

## Execução sugerida

Para cada entrada, execute o scanner e compare a sequência produzida com o arquivo `.expected.jsonl`. Nos casos inválidos, compare também classe, linha e coluna do diagnóstico. O scanner pode continuar após um erro, desde que preserve os tokens posteriores; isso é indicado no caso correspondente.

## Programas completos em C

A pasta `casos-programas-c` contém cinco programas completos, escritos no subconjunto de C utilizado pela MINIC. Cada programa possui um arquivo `.expected.jsonl` correspondente, com a sequência integral de tokens, posições e atributos esperados. Esses casos exercitam declarações, funções, chamadas, laços, condicionais, vetores, operadores compostos, comentários e literais.

Os arquivos usam a extensão `.c` apenas para facilitar a leitura como código-fonte C; a convenção léxica e os nomes dos tokens são os definidos neste pacote.
