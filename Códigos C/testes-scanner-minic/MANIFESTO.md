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
