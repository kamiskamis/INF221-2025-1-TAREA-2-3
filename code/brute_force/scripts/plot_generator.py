import os
import glob
import matplotlib.pyplot as plt

def main():
    # Ruta a la carpeta de mediciones de brute_force
    measurements_folder = os.path.join("code", "brute_force", "data", "measurements")
    if not os.path.exists(measurements_folder):
        print("La carpeta de mediciones no existe:", measurements_folder)
        return

    # Buscar todos los archivos .txt en la carpeta de mediciones
    files = glob.glob(os.path.join(measurements_folder, "*.txt"))
    if not files:
        print("No se encontraron archivos de medición en:", measurements_folder)
        return

    # Listas para acumular datos:
    # data_points: para casos válidos (dimensión, tiempo)
    # error_points: para casos donde el tiempo no es convertible (p.ej., TIMEOUT)
    # table_data: para la tabla (cada registro es [dimensión, tiempo])
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
                # Se espera formato de la forma: <size de s> <size de t> <tiempo o mensaje>
                parts = line.split()
                if len(parts) < 3:
                    continue
                try:
                    size_s = int(parts[0])
                    size_t = int(parts[1])
                except ValueError:
                    continue
                # Definir la dimensión (aquí, la suma de ambos tamaños)
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

    # Crear carpeta para guardar los gráficos (si no existe)
    plot_folder = os.path.join("code", "brute_force", "data", "plots")
    os.makedirs(plot_folder, exist_ok=True)

    # ----------------------------------------------------------------------------------
    # Primera imagen: Gráfico de dimensión vs. tiempo
    # ----------------------------------------------------------------------------------
    # Ordenar los puntos válidos según la dimensión (en orden ascendente)
    if data_points:
        data_points.sort(key=lambda x: x[0])
        x_success, y_success = zip(*data_points)
    else:
        x_success, y_success = [], []

    # Crear el gráfico
    plt.figure(figsize=(10, 6))
    if x_success and y_success:
        # Conectar cada punto (ordenados) con el siguiente con una línea
        plt.plot(x_success, y_success, marker='o', linestyle='-', color='blue', label="Ejecución exitosa")
    if error_points:
        # Graficar los errores, ubicándolos en y=0 (sin conexión)
        plt.scatter(error_points, [0] * len(error_points), color="red", marker='x', label="TIMEOUT / TOO_LONG")

    plt.xlabel("Dimensión (suma de tamaños de las cadenas)")
    plt.ylabel("Tiempo de ejecución (s)")
    plt.title("Relación entre dimensión y tiempo de ejecución (Brute Force)")
    plt.legend()
    plt.grid(True)
    graph_path = os.path.join(plot_folder, "dim_vs_time_graph.png")
    plt.savefig(graph_path, dpi=300, bbox_inches="tight")
    print("Gráfico guardado en:", graph_path)
    plt.close()  # cerrar la figura para liberar memoria

    # ----------------------------------------------------------------------------------
    # Segunda imagen: Tabla con los datos
    # ----------------------------------------------------------------------------------
    # Ordenar los datos de la tabla por dimensión (ascendente)
    table_data.sort(key=lambda row: int(row[0]))
    # Opcional: limitar el número de filas, por ejemplo a 20
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

    table_path = os.path.join(plot_folder, "dim_vs_time_table.png")
    plt.savefig(table_path, dpi=300, bbox_inches="tight")
    print("Tabla guardada en:", table_path)
    plt.close()

if __name__ == "__main__":
    main()