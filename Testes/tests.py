import json
import sys
from pathlib import Path

sys.path.append(str(Path(__file__).parent.parent / "Py"))
from Lexichal import analisadorLexico

pastas = [
    Path("Códigos C/testes-scanner-minic_codes/casos-programas-c")
]

for pasta in pastas:
    arquivos_teste = list(pasta.glob("*.c")) + list(pasta.glob("*.txt")) + list(pasta.glob("*.minic"))
    
    for arquivo in arquivos_teste:
        try:
            with open(arquivo, 'r', encoding='utf-8') as f:
                codigo = f.read()
            
            tokens = analisadorLexico(codigo)
            
            saida = Path("resultados")
            saida.mkdir(parents=True, exist_ok=True)
            
            arquivo_saida = saida / f"{arquivo.stem}_tokens.jsonl"
            with open(arquivo_saida, 'w', encoding='utf-8') as f:
                for token in tokens:
                    f.write(json.dumps(token, ensure_ascii=False) + '\n')
                    
        except Exception as e:
            continue