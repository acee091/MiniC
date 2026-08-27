import regex;
import json
    
# codigo = '''
# str Test = "Codigo"
# int numero = 36 
# '''

tokensDefinition = {
    "literal": r'\\',
    "str": r'".*"',
    "function": "func|str|int|float|return|if|else|while|for",
    "operator": "[==|=|+|-|/|*|[|]|{|}]",
    "int": r'[0-9]+',
    "var": r'[A-Za-z_][A-Za-z_0-9]*'
};

tokenFound = {
    "type": "void | int | var | func",
    "col": int,
    "row": int,
    "value": any
}


def analisadorLexico(codigo):
    codeTokens = [] # (tokenColStart, tokenColEnd)

    codeLines = codigo.split("\n")

    for index, line in enumerate(codeLines, start=0):
        if line.strip() == "": 
            continue
    
        consumed = []
    
        newLine = line
    
        for tokenType, definition in tokensDefinition.items():
             
            for found in regex.finditer(definition, newLine):
                
                foundStart, foundEnd = found.span()            
                
                # I don't know any other way of doing this
                intervalConsumed = False
                
                for cX, cY in consumed:
                    if foundStart>=cX and foundEnd <= cY:
                        intervalConsumed = True
    
                if intervalConsumed:
                    continue
    
                consumed.append(found.span())
    
                foundedstring = line[foundStart:foundEnd]
    
                codeTokens.append({
                    "type": tokenType,
                    "value": foundedstring,
                    "row": index,
                    "colSpan": found.span(),
                    })
                
    
    pprint(codeTokens)
    return codeTokens
