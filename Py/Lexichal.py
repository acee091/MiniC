import regex;
import json
from pprint import pprint

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
    "str": r'".*"',
    "var": r'[A-Za-z_][A-Za-z_0-9]*',
    "int": r'[0-9]+',
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

codeTokens = []

codeLines = codigo.split("\n")


for index, line in enumerate(codeLines, start=0):
    if line.strip() == "": 
        continue

    consumed = []

    newLine = line

    for tokenType, definition in tokensDefinition.items():
        
        for found in regex.finditer(definition, newLine):
            
            x1, x2 = found.span()            
            
            for cX, cY in consumed:
                if x<=cX and y >= cY:
                    continue
                break

            consumed.append(found.span())

            foundedstring = line[x1:x2]

            codeTokens.append({
                "type": tokenType,
                "value": foundedstring,
                "row": index,
                "colSpan": found.span(),
                })
            

pprint(codeTokens)
