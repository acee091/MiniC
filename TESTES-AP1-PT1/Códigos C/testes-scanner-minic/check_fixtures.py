import json
from pathlib import Path

root = Path(__file__).parent
required = {"token", "lexeme", "attribute", "line", "column"}
count = 0
for expected in sorted(root.glob("casos-*/**/*.expected.jsonl")):
    for number, line in enumerate(expected.read_text(encoding="utf-8").splitlines(), 1):
        item = json.loads(line)
        missing = required - item.keys()
        if missing:
            raise SystemExit(f"{expected}:{number}: campos ausentes: {sorted(missing)}")
        count += 1
        if not isinstance(item["line"], int) or not isinstance(item["column"], int):
            raise SystemExit(f"{expected}:{number}: linha/coluna devem ser inteiros")
    if not any(json.loads(line).get("token") == "EOF" for line in expected.read_text(encoding="utf-8").splitlines()):
        raise SystemExit(f"{expected}: não possui EOF")
for errors in sorted(root.glob("casos-invalidos/**/*.errors.jsonl")):
    for number, line in enumerate(errors.read_text(encoding="utf-8").splitlines(), 1):
        item = json.loads(line)
        if not {"error", "lexeme", "line", "column"} <= item.keys():
            raise SystemExit(f"{errors}:{number}: diagnóstico incompleto")
print(f"Fixtures válidas: {count} tokens esperados verificados.")
