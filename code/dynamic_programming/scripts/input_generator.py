import os
import random
import string

NUM_ARCHIVOS = 3
CASOS_POR_ARCHIVO = 10
MAX_LEN = 15
CHARS = string.ascii_uppercase    

output_dir = "code/brute_force/data/brute_force_input"
os.makedirs(output_dir, exist_ok=True)

def generar_cadena(largo):
    return ''.join(random.choices(CHARS, k=largo))

def generar_caso(tipo):
    if tipo == "vacio":
        s = "" if random.random() < 0.5 else generar_cadena(random.randint(1, MAX_LEN))
        t = "" if random.random() < 0.5 else generar_cadena(random.randint(1, MAX_LEN))
    elif tipo == "igual":
        n = random.randint(1, MAX_LEN)
        s = generar_cadena(n)
        t = s  # iguales
    elif tipo == "diferente":
        n = random.randint(1, MAX_LEN)
        m = random.randint(1, MAX_LEN)
        s = generar_cadena(n)
        t = generar_cadena(m)
        if s == t:
            t = s[::-1] if len(s) > 1 else s + "A"
    else:
        s = t = ""
    return f"{len(s)} {s}\n{len(t)} {t}\n"

tipos = ["vacio", "igual", "diferente"]

for archivo in range(NUM_ARCHIVOS):
    nombre = f"{output_dir}/entrada_{archivo + 1}.txt"
    with open(nombre, "w") as f:
        f.write(f"{CASOS_POR_ARCHIVO}\n")
        for _ in range(CASOS_POR_ARCHIVO):
            tipo = random.choices(tipos, weights=[1, 2, 7])[0]  # más casos diferentes
            caso = generar_caso(tipo)
            f.write(caso)

print(f"casos de prueba generados :3")
