import folium
from folium.plugins import FloatImage

# Dados dos aeroportos
aeroportos = [
    {"IATA": "GRU", "Local": "Guarulhos", "Aeroporto": "São Paulo", "País": "Brasil", "Lat": -23.4346, "Long": -46.4732},
    {"IATA": "JFK", "Local": "John F. Kennedy", "Aeroporto": "New York", "País": "Estados Unidos", "Lat": 40.6413, "Long": -73.7781},
    {"IATA": "EZE", "Local": "Ministro Pistarini", "Aeroporto": "Buenos Aires", "País": "Argentina", "Lat": -34.8129, "Long": -58.5398},
    # Restante dos aeroportos...
]

# Dados das conexões
conexoes = [
    {"inicio": "GRU", "destino": "JFK", "distancia": 8055},
    {"inicio": "GRU", "destino": "EZE", "distancia": 2010},
    {"inicio": "GRU", "destino": "DUB", "distancia": 5933},
    # Restante das conexões...
]

# Criar um mapa
m = folium.Map(location=[0, 0], zoom_start=2)

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
            weight=2
        ).add_to(m)

        # Calcular o ponto médio da conexão
        mid_lat = (inicio["Lat"] + destino["Lat"]) / 2
        mid_long = (inicio["Long"] + destino["Long"]) / 2

        # Adicionar a legenda da distância
        folium.plugins.FloatText(
            text=f"{conexao['distancia']} km",
            location=[mid_lat, mid_long],
            angle=-45,
            size=10,
            show=True,
            icon=False,
            opacity=0.8
        ).add_to(m)

# Exibir o mapa
m.save('mapa1.html')
