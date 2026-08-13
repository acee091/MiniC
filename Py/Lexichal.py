import regex as joao;

tokensRegex = {
    "var": r'[A-Za-z_][A-Za-z_0-9]*',
    "int": r'[0-9]+',
    "str": r'".*"',
    "literal": r'\',
    "operators": "[==|=|+|-|\/|*|[|]|{|}]"
    "functions": [
        "int", "func", "str", "float",
        "return", "if", "else", "while", "for"
    ]
};

tokenFound = {
    "type": "void | int | var | func",
    "col": int,
    "row": int,
    "value": any
}

tokensFound = [];

for i in tokens:
    foundTokens = joao.findall(i)
    foundTokens
    