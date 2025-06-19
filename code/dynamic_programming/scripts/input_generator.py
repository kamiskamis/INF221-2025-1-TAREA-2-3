import os
import random
import string

NUM_ARCHIVOS = 3
CASOS_POR_ARCHIVO = 5
MAX_LEN = 20
CHARS = string.ascii_uppercase + " "  # Ahora incluye espacios

# Directorios de entrada para DP y BF
output_dir_dp = "code/dynamic_programming/data/dynamic_programming_input"
output_dir_bf = "code/brute_force/data/brute_force_input"
os.makedirs(output_dir_dp, exist_ok=True)
os.makedirs(output_dir_bf, exist_ok=True)

def generar_cadena(largo):
    return ''.join(random.choices(CHARS, k=largo))

def generar_caso(tipo):
    if tipo == "vacio":
        s = "" if random.random() < 0.5 else generar_cadena(random.randint(1, MAX_LEN))
        t = "" if random.random() < 0.5 else generar_cadena(random.randint(1, MAX_LEN))
    elif tipo == "igual":
        n = random.randint(1, MAX_LEN)
        s = generar_cadena(n)
        t = s  # Ambas son iguales.
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
    contenido = f"{CASOS_POR_ARCHIVO}\n"
    for _ in range(CASOS_POR_ARCHIVO):
        tipo = random.choices(tipos, weights=[1, 2, 7])[0]  # Mayor probabilidad de "diferente"
        caso = generar_caso(tipo)
        contenido += caso

    # Guardar el mismo contenido en el directorio de DP
    nombre_dp = f"{output_dir_dp}/entrada_{archivo + 1}.txt"
    with open(nombre_dp, "w") as f:
        f.write(contenido)

    # Guardar el mismo contenido en el directorio de BF
    nombre_bf = f"{output_dir_bf}/entrada_{archivo + 1}.txt"
    with open(nombre_bf, "w") as f:
        f.write(contenido)

print("Casos de prueba generados para DP y BF con espacios incluidos :3")