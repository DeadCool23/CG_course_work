import csv
import plotly.express as px
import plotly.graph_objects as go

MES_CSV_FILES = ["points_mes.csv", "segment_mes.csv"]
DEPENDENSES = ["точек на кривой", "сегментов"]
FILENAMES = ["points_mes.html", "segment_mes.html"]

def read_data_from_file(file_path):
    with open(file_path, mode='r', encoding='utf-8') as file:
        reader = csv.DictReader(file, delimiter=",")
        return list(reader)

def get_mes_data(mes_data):
    sizes = []
    mes = []

    for data in mes_data:
        sizes.append(data["n"])
        mes.append(float(data["time"]))

    return (sizes, mes)


if __name__ == '__main__':
    for i in range(0, len(MES_CSV_FILES)):
        mes_data = read_data_from_file(MES_CSV_FILES[i])
        sizes, mes = get_mes_data(mes_data)

        fig = go.Figure()

        fig.add_trace(go.Scatter(x=sizes, y=mes, mode='lines+markers'))

        fig.update_layout(go.Layout(
            title=f"Зависимость времени генерации тела вращения от количества {DEPENDENSES[i]}",
            xaxis=dict(
                title=f"Количество {DEPENDENSES[i]}"
            ),
            yaxis=dict(
                title="Время выполнения (нс)"
            )
        ))

        fig.write_html(FILENAMES[i])