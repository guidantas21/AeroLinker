import folium
from folium import plugins

# Dados dos aeroportos
aeroportos = [
    {"IATA": "GRU", "Local": "Guarulhos", "Aeroporto": "São Paulo", "País": "Brasil", "Lat": -23.4346, "Long": -46.4732},
    {"IATA": "JFK", "Local": "John F. Kennedy", "Aeroporto": "New York", "País": "Estados Unidos", "Lat": 40.6413, "Long": -73.7781},
    {"IATA": "EZE", "Local": "Ministro Pistarini", "Aeroporto": "Buenos Aires", "País": "Argentina", "Lat": -34.8129, "Long": -58.5398},
    {"IATA": "DUB", "Local": "Terminal 2", "Aeroporto": "Dublin", "País": "Irlanda", "Lat": 53.4264, "Long": -6.2499},
    {"IATA": "SYD", "Local": "Sydney", "Aeroporto": "Sydney", "País": "Austrália", "Lat": -33.9462, "Long": 151.1772},
    {"IATA": "LAX", "Local": "Los Angeles", "Aeroporto": "Los Angeles", "País": "Estados Unidos", "Lat": 33.9416, "Long": -118.4085},
    {"IATA": "DXB", "Local": "Dubai", "Aeroporto": "Dubai", "País": "Emirados Árabes Unidos", "Lat": 25.252, "Long": 55.364},
    {"IATA": "LHR", "Local": "Heathrow", "Aeroporto": "London", "País": "Reino Unido", "Lat": 51.4700, "Long": -0.4543},
    {"IATA": "MAD", "Local": "Barajas", "Aeroporto": "Madrid", "País": "Espanha", "Lat": 40.4719, "Long": -3.5626},
    {"IATA": "CDG", "Local": "Charles de Gaulle", "Aeroporto": "Paris", "País": "França", "Lat": 49.0097, "Long": 2.5479},
    {"IATA": "HND", "Local": "Haneda", "Aeroporto": "Tokyo", "País": "Japão", "Lat": 35.5523, "Long": 139.7798},
    {"IATA": "AKL", "Local": "Auckland", "Aeroporto": "Auckland", "País": "Nova Zelândia", "Lat": -37.0081, "Long": 174.785},
    {"IATA": "SIN", "Local": "Changi", "Aeroporto": "Singapore", "País": "Cingapura", "Lat": 1.3644, "Long": 103.9915},
    {"IATA": "MEL", "Local": "Tullamarine", "Aeroporto": "Melbourne", "País": "Austrália", "Lat": -37.6690, "Long": 144.8410}
]

# Dados das conexões e distâncias
conexoes = [
    {"inicio": "GRU", "destino": "JFK", "distancia": 8055},
    {"inicio": "GRU", "destino": "EZE", "distancia": 2010},
    {"inicio": "GRU", "destino": "DUB", "distancia": 5933},
    {"inicio": "GRU", "destino": "SYD", "distancia": 15219},
    {"inicio": "JFK", "destino": "LAX", "distancia": 3985},
    {"inicio": "JFK", "destino": "DXB", "distancia": 11147},
    {"inicio": "JFK", "destino": "LHR", "distancia": 5574},
    {"inicio": "JFK", "destino": "MAD", "distancia": 8706},
    {"inicio": "EZE", "destino": "LAX", "distancia": 9070},
    {"inicio": "EZE", "destino": "MAD", "distancia": 10518},
    {"inicio": "EZE", "destino": "CDG", "distancia": 10915},
    {"inicio": "EZE", "destino": "HND", "distancia": 18585},
    {"inicio": "DUB", "destino": "JFK", "distancia": 5106},
    {"inicio": "DUB", "destino": "DXB", "distancia": 5548},
    {"inicio": "DUB", "destino": "LHR", "distancia": 464},
    {"inicio": "DUB", "destino": "CDG", "distancia": 832},
    {"inicio": "SYD", "destino": "LAX", "distancia": 12010},
    {"inicio": "SYD", "destino": "HND", "distancia": 7829},
    {"inicio": "SYD", "destino": "AKL", "distancia": 2151},
    {"inicio": "SYD", "destino": "SIN", "distancia": 6306},
    {"inicio": "LAX", "destino": "HND", "distancia": 8836},
    {"inicio": "LAX", "destino": "AKL", "distancia": 10394},
    {"inicio": "LAX", "destino": "SIN", "distancia": 14579},
    {"inicio": "DXB", "destino": "DUB", "distancia": 5548},
    {"inicio": "DXB", "destino": "LHR", "distancia": 5506},
    {"inicio": "DXB", "destino": "SIN", "distancia": 5292},
    {"inicio": "MAD", "destino": "JFK", "distancia": 8706},
    {"inicio": "MAD", "destino": "EZE", "distancia": 10518},
    {"inicio": "MAD", "destino": "LHR", "distancia": 1254},
    {"inicio": "MAD", "destino": "CDG", "distancia": 1055},
    {"inicio": "MAD", "destino": "HND", "distancia": 19286},
    {"inicio": "CDG", "destino": "EZE", "distancia": 10915},
    {"inicio": "CDG", "destino": "DUB", "distancia": 832},
    {"inicio": "CDG", "destino": "MAD", "distancia": 1055},
    {"inicio": "CDG", "destino": "HND", "distancia": 18769},
    {"inicio": "HND", "destino": "SYD", "distancia": 7829},
    {"inicio": "HND", "destino": "LAX", "distancia": 8836},
    {"inicio": "HND", "destino": "MAD", "distancia": 19286},
    {"inicio": "HND", "destino": "CDG", "distancia": 18769},
    {"inicio": "HND", "destino": "SIN", "distancia": 6386},
    {"inicio": "HND", "destino": "MEL", "distancia": 15545},
    {"inicio": "AKL", "destino": "SYD", "distancia": 2151},
    {"inicio": "AKL", "destino": "LAX", "distancia": 10394},
    {"inicio": "AKL", "destino": "DUB", "distancia": 19847},
    {"inicio": "AKL", "destino": "DXB", "distancia": 14484},
    {"inicio": "AKL", "destino": "SIN", "distancia": 8404},
    {"inicio": "AKL", "destino": "CDG", "distancia": 21150},
    {"inicio": "AKL", "destino": "HND", "distancia": 15063},
    {"inicio": "SIN", "destino": "SYD", "distancia": 6306},
    {"inicio": "SIN", "destino": "LAX", "distancia": 14579},
    {"inicio": "SIN", "destino": "DXB", "distancia": 5292},
    {"inicio": "SIN", "destino": "AKL", "distancia": 8404},
    {"inicio": "SIN", "destino": "HND", "distancia": 6386},
    {"inicio": "SIN", "destino": "MEL", "distancia": 6329},
    {"inicio": "MEL", "destino": "HND", "distancia": 15545},
    {"inicio": "MEL", "destino": "SIN", "distancia": 6329}
]

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
            weight=2
        ).add_to(m)
        
        # Calcular o ponto médio da conexão
        mid_lat = (inicio["Lat"] + destino["Lat"]) / 2
        mid_long = (inicio["Long"] + destino["Long"]) / 2
        
        # Adicionar a legenda da distância
        folium.Marker(
            location=[mid_lat, mid_long],
            icon=folium.Icon(color='white', icon_color='blue', icon='info-sign'),
            popup=f"{conexao['distancia']} km",
        ).add_to(m)

# Salvar o mapa como um arquivo HTML
m.save('mapa.html')