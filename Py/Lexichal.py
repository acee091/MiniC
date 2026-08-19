import regex;

'''
    Todo:
    - Pegar row e col
    - Regex mais robustas (o joão é ótimo com palavras por que ele lê bastante livros (um homem culto) (e solteiro!! prestem atencao em gatinhasss miauuu xDDDD)))
    - Trocar a ordem de execução dos regex
    - "Consumir" os tokens já lidos pelo regex
'''

codigo = '''
str Test = "Codigo"
int numero = 36 
'''

tokensDefinition = {
    "var": r'[A-Za-z_][A-Za-z_0-9]*',
    "int": r'[0-9]+',
    "str": r'".*"',
    "literal": r'\\',
    "operators": "[==|=|+|-|/|*|[|]|{|}]",
    # "functions": [
    #     "int", "func", "str", "float",
    #     "return", "if", "else", "while", "for"
    # ]
};

tokenFound = {
    "type": "void | int | var | func",
    "col": int,
    "row": int,
    "value": any
}

tokensFound = []

linhasCodigo = codigo.split("\n")


for index, linha in enumerate(linhasCodigo, start=0):
    if linha.strip() == "": 
        continue

    for type, definition in tokensDefinition.items():
        print({
            "type": type,
            "value": regex.findall(definition, linha)
            })

    print("--------")