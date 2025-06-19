import os
import glob
import matplotlib.pyplot as plt

def main():
    # Ruta a la carpeta de mediciones de DP
    measurements_folder = os.path.join("code", "dynamic_programming", "data", "measurements")
    if not os.path.exists(measurements_folder):
        print("La carpeta de mediciones no existe:", measurements_folder)
        return

    # Buscar todos los archivos .txt en esa carpeta
    files = glob.glob(os.path.join(measurements_folder, "*.txt"))
    if not files:
        print("No se encontraron archivos de medición en:", measurements_folder)
        return

    # Listas para acumular datos:
    # data_points: casos con tiempo válido, en forma (dimensión, tiempo)
    # error_points: dimensiones donde ocurrió error (TIMEOUT, TOO_LONG, etc.)
    # table_data: para armar la tabla (cada registro: [dimensión, tiempo])
    data_points = []
    error_points = []
    table_data = []

    # Recorrer cada archivo y acumular los datos
    for filepath in files:
        with open(filepath, "r") as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                # Se espera el formato: <size de s> <size de t> <tiempo o mensaje>
                parts = line.split()
                if len(parts) < 3:
                    continue
                try:
                    size_s = int(parts[0])
                    size_t = int(parts[1])
                except ValueError:
                    continue
                # Definir la dimensión como la suma de ambos tamaños
                dimension = size_s + size_t
                try:
                    exec_time = float(parts[2])
                    data_points.append((dimension, exec_time))
                    table_data.append([str(dimension), f"{exec_time:.6f}"])
                except ValueError:
                    error_points.append(dimension)
                    table_data.append([str(dimension), "ERROR"])

    if not data_points and not error_points:
        print("No se hallaron datos válidos en los archivos de medición.")
        return

    # Crear carpeta para guardar los gráficos si no existe
    plot_folder = os.path.join("report", "images")
    os.makedirs(plot_folder, exist_ok=True)

    # ----------------------------------------------------------------------------------
    # Primera imagen: Gráfico de dimensión vs. tiempo
    # ----------------------------------------------------------------------------------
    # Ordenar los puntos válidos por dimensión (ascendente)
    if data_points:
        data_points.sort(key=lambda x: x[0])
        x_success, y_success = zip(*data_points)
    else:
        x_success, y_success = [], []

    plt.figure(figsize=(10, 6))
    if x_success and y_success:
        # Conecta cada punto (ordenado) con el siguiente
        plt.plot(x_success, y_success, marker='o', linestyle='-', color='blue', label="Ejecución exitosa")
    if error_points:
        # Graficar los casos de error en y=0
        plt.scatter(error_points, [0] * len(error_points), color="red", marker='x', label="TIMEOUT / TOO_LONG")
    
    plt.xlabel("Dimensión (suma de tamaños de las cadenas)")
    plt.ylabel("Tiempo de ejecución (s)")
    plt.title("Relación entre dimensión y tiempo de ejecución (DP)")
    plt.legend()
    plt.grid(True)
    graph_path = os.path.join(plot_folder, "dtdp.png")
    plt.savefig(graph_path, dpi=300, bbox_inches="tight")
    print("Gráfico guardado en:", graph_path)
    plt.close()

    # ----------------------------------------------------------------------------------
    # Segunda imagen: Tabla con los datos
    # ----------------------------------------------------------------------------------
    # Ordenar la tabla por dimensión (ascendente)
    table_data.sort(key=lambda row: int(row[0]))
    # Limitar el número de filas a mostrar, por ejemplo a 20
    max_rows = 20
    if len(table_data) > max_rows:
        table_data = table_data[:max_rows]

    fig, ax = plt.subplots(figsize=(4, 0.5 * len(table_data) + 1))
    ax.axis('tight')
    ax.axis('off')
    table = ax.table(cellText=table_data, colLabels=["Dimensión", "Tiempo (s)"], loc='center')
    table.auto_set_font_size(False)
    table.set_fontsize(10)
    table.scale(1.2, 1.2)

    table_path = os.path.join(plot_folder, "dtdpt.png")
    plt.savefig(table_path, dpi=300, bbox_inches="tight")
    print("Tabla guardada en:", table_path)
    plt.close()

if __name__ == "__main__":
    main()