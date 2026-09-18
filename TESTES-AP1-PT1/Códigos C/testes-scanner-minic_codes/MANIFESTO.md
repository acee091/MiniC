# Manifesto dos casos

## Casos válidos

| Caso | Cobertura |
|---|---|
| `v01_declaracoes.minic` | palavras reservadas, identificadores, inteiros, atribuição e delimitadores |
| `v02_expressao_precedencia.minic` | operadores aritméticos, relacionais e parênteses |
| `v03_operadores_compostos.minic` | maximal munch: `==`, `!=`, `<=`, `>=`, `&&`, `||` |
| `v04_comentarios_e_posicoes.minic` | comentários de linha/bloco, espaços e linhas/colunas |
| `v05_funcoes_e_vetores.minic` | funções, parâmetros, vetores, chamadas e blocos |
| `v06_reservadas_vs_identificadores.minic` | palavras reservadas e identificadores semelhantes |
| `v07_literais_opcionais.minic` | literais de caractere e cadeia, conforme convenção opcional do README |

## Casos inválidos

| Caso | Cobertura |
|---|---|
| `i01_simbolo_desconhecido.minic` | símbolo `@` não reconhecido |
| `i02_comentario_nao_terminado.minic` | comentário de bloco sem `*/` |
| `i03_caractere_nao_terminado.minic` | literal de caractere sem fechamento |
| `i04_cadeia_nao_terminada.minic` | literal de cadeia sem fechamento |
| `i05_numero_real_malformado.minic` | ponto sem dígitos após a parte inteira |
| `i06_identificador_iniciado_por_digito.minic` | lexema inválido `123abc` |
| `i07_operador_logico_incompleto.minic` | `&` e `|` isolados |

## Programas completos em C

| Caso | Cobertura |
|---|---|
| `c01_fibonacci.c` | função iterativa, laço `while`, variáveis e chamadas |
| `c02_primos.c` | booleanos, condicionais aninhadas, módulo e operadores relacionais |
| `c03_media_vetor.c` | vetor, indexação, reais, função com parâmetro vetor e divisão |
| `c04_menu_interativo.c` | funções `void`, chamadas com argumentos, `if/else` e controle de estado |
| `c05_controle_temperatura.c` | comentários de bloco, reais, caracteres, booleanos, `&&` e `||` |

Cada programa possui um arquivo `.expected.jsonl` com todos os tokens esperados, incluindo `EOF`.
