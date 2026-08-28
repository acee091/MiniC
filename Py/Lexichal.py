from pprint import pprint
import regex;
import json

tokensDefinition = {
    "literal": r'\\',
    "str": r'".*"',
    "function": "func|str|int|float|return|if|else|while|for",
    "operator": "[==|=|+|-|/|*]",
    "braces": "[[|]|{|}|(|)]",
    "int": r'[0-9]+',
    "var": r'[A-Za-z_][A-Za-z_0-9]*'
};

tokenFound = {
    "type": "void | int | var | func",
    "col": int,
    "row": int,
    "value": any
}

# Return
'''
Return: {error: "error_name", error_location: (0, 1)}
'''
def cacadorDeErros(tokens):
    
    parDeOperadores = False

    # TODO: melhorar!
    braces = [token for token in tokens if token["type"] == "braces"]
    braces_t = ["[]", "()", "{}"]

    for i in braces_t:

        braces_values = [brace["value"] for brace in braces]

        brace_left_ammt  = braces_values.count(list(i)[0])
        brace_right_ammt = braces_values.count(list(i)[1])


        if brace_left_ammt != brace_right_ammt:
            last_instance = (''.join(braces_values).rindex(list(i)[0]))                
            return {"error": "Brace without pair!", 
                "error_location": braces[last_instance]["colSpan"]}
    
    return None

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

    error = cacadorDeErros(codeTokens)  
    return error if error else codeTokens

codigo = '''
str Test = "Codigo"
int numero = 36 
{[}
'''

pprint(analisadorLexico(codigo))