import folium
from folium import plugins
import webbrowser
import os
import csv

def ler_dados_aeroportos(caminho_arquivo):
    aeroportos = []
    with open(caminho_arquivo, 'r', encoding='utf-8') as arquivo_csv:
        leitor_csv = csv.reader(arquivo_csv, delimiter=',')
        for linha in leitor_csv:
            if len(linha) == 6:
                codigo, aeroporto, cidade, pais, lat, long = linha
                aeroporto = {"IATA": codigo, "Aeroporto": aeroporto, "Local": cidade, "País": pais, "Lat": float(lat), "Long": float(long)}
                aeroportos.append(aeroporto)
            else:
                print(f"Linha inválida: {linha}")
    return aeroportos

def ler_dados_conexoes(nome_arquivo):
    conexoes = []

    with open(nome_arquivo, 'r', encoding='utf-8') as arquivo_csv:
        leitor_csv = csv.reader(arquivo_csv, delimiter=',')
        for linha in leitor_csv:
            if len(linha) == 3:
                inicio, destino, distancia = linha
                conexao = {"inicio": inicio, "destino": destino, "distancia": float(distancia)}
                conexoes.append(conexao)
            else:
                print(f"Linha inválida: {linha}")
            
    return conexoes

def abrir_html_no_navegador(arquivo_html):
    url = 'file://' + arquivo_html
    webbrowser.open(url)

# Exemplo de uso
aeroportos = ler_dados_aeroportos('./dados/aeroportos.csv')
conexoes = ler_dados_conexoes('./dados/conexoes.csv')

# Criar um mapa
m = folium.Map(location=[0, 0], zoom_start=2)

m = folium.Map(location=(42, 29), zoom_start=5)
minimap = plugins.MiniMap()
m.add_child(minimap)

# Adicionar pontos no mapa
for aeroporto in aeroportos:
    folium.Marker(
        location=[aeroporto["Lat"], aeroporto["Long"]],
        popup=f"{aeroporto['IATA']}: {aeroporto['Aeroporto']}, {aeroporto['Local']}, {aeroporto['País']}"
    ).add_to(m)

# Adicionar conexões no mapa
for conexao in conexoes:
    inicio = next((aeroporto for aeroporto in aeroportos if aeroporto["IATA"] == conexao["inicio"]), None)
    destino = next((aeroporto for aeroporto in aeroportos if aeroporto["IATA"] == conexao["destino"]), None)
    if inicio and destino:
        folium.PolyLine(
            locations=[[inicio["Lat"], inicio["Long"]], [destino["Lat"], destino["Long"]]],
            color='blue',
            weight=1
        ).add_to(m)

# Salvar o mapa como um arquivo HTML
m.save('mapas/mapa.html')

# Exemplo de uso
arquivo_html = os.path.realpath("mapas/mapa.html")
abrir_html_no_navegador(arquivo_html)