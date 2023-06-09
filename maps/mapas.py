import folium
from folium import plugins
import webbrowser
import os
import csv
from random import randint
import sys

class Mapa:
    def __init__(self, dados) -> None:
        self.dados = dados
        self.mapa = folium.Map(location=[0, 0], zoom_start=2)
        self.mini_map = plugins.MiniMap()
        self.mapa.add_child(self.mini_map)
        self.aeroportos = self.ler_dados_aeroportos()
        self.conexoes = self.ler_dados_conexoes()
        self.voos = self.ler_dados_voos()

    def ler_dados_aeroportos(self):
        aeroportos = []
        with open(self.dados["aeroportos"], 'r', encoding='utf-8') as arquivo_csv:
            leitor_csv = csv.reader(arquivo_csv, delimiter=',')
            for linha in leitor_csv:
                if len(linha) == 6:
                    codigo, aeroporto, cidade, pais, lat, long = linha
                    aeroporto = {"IATA": codigo, "Aeroporto": aeroporto, "Local": cidade, "País": pais, "Lat": float(lat), "Long": float(long)}
                    aeroportos.append(aeroporto)
                else:
                    print(f"Linha inválida: {linha}")
        
        return aeroportos


    def ler_dados_conexoes(self):
        conexoes = []
        with open(self.dados["conexoes"], 'r', encoding='utf-8') as arquivo_csv:
            leitor_csv = csv.reader(arquivo_csv, delimiter=',')
            for linha in leitor_csv:
                if len(linha) == 3:
                    inicio, destino, distancia = linha
                    conexao = {"inicio": inicio, "destino": destino, "distancia": float(distancia)}
                    conexoes.append(conexao)
                else:
                    print(f"Linha inválida: {linha}")

        return conexoes


    def ler_dados_voos(self):
        voos = []
        with open(dados["voos"], 'r', encoding='utf-8') as arquivo_csv:
            leitor_csv = csv.reader(arquivo_csv, delimiter=',')
            for linha in leitor_csv:
                if len(linha) == 3:
                    conexoes = []
                    distancia, trajeto = linha[:2]
                    trajeto = trajeto.split("-")
                    for i in range(len(trajeto)-1):
                        inicio = trajeto[i]
                        destino = trajeto[i+1]
                        conexao = {"inicio": inicio, "destino": destino}
                        conexoes.append(conexao)
                    voo = {"distancia": distancia, "conexoes": conexoes}
                    voos.append(voo)
                else:
                    print(f"Linha inválida: {linha}")
        return voos


    def marcar_aeroportos(self):
        for aeroporto in self.aeroportos:
            folium.Marker(
                location=[aeroporto["Lat"], aeroporto["Long"]],
                popup=f"{aeroporto['IATA']}: {aeroporto['Aeroporto']}, {aeroporto['Local']}, {aeroporto['País']}"
            ).add_to(self.mapa)

    
    def marcar_conexoes(self, cor):
        for conexao in self.conexoes:
            inicio = next((aeroporto for aeroporto in self.aeroportos if aeroporto["IATA"] == conexao["inicio"]), None)
            destino = next((aeroporto for aeroporto in self.aeroportos if aeroporto["IATA"] == conexao["destino"]), None)
            if inicio and destino:
                folium.PolyLine(
                    locations=[[inicio["Lat"], inicio["Long"]], [destino["Lat"], destino["Long"]]],
                    color=cor,
                    weight=2
                ).add_to(self.mapa)


    def marcar_voo(self, pk, cor_linha):
        for conexao in self.voos[pk]["conexoes"]:
            inicio = next((aeroporto for aeroporto in self.aeroportos if aeroporto["IATA"] == conexao["inicio"]), None)
            destino = next((aeroporto for aeroporto in self.aeroportos if aeroporto["IATA"] == conexao["destino"]), None)
            if inicio and destino:
                folium.PolyLine(
                    locations=[[inicio["Lat"], inicio["Long"]], [destino["Lat"], destino["Long"]]],
                    color=cor_linha,
                    weight=2
                ).add_to(self.mapa)
    
    @staticmethod
    def rgbParaHex(rgb):
        return '#%02x%02x%02x' % rgb


    def marcar_voos(self):
        for i in range(len(self.voos)):
            rgb = (randint(0,255), randint(0,255), 0)
            cor_linha = self.rgbParaHex(rgb)
            self.marcar_voo(i, cor_linha)

    @staticmethod
    def abrir_html_no_navegador(arquivo_html):
        url = 'file://' + os.path.realpath(arquivo_html)
        webbrowser.open(url)


    def gerar_mapa_rede_aerea(self):
        caminho = "maps/mapas/rede_aerea.html"

        self.marcar_aeroportos()
        self.marcar_conexoes("blue")

        self.mapa.save(caminho)
        self.abrir_html_no_navegador(caminho)


    def gerar_mapa_voos(self):
        caminho = f"maps/mapas/voos.html"

        self.marcar_aeroportos()
        self.marcar_conexoes(self.rgbParaHex((180,180,255)))
        self.marcar_voos()

        self.mapa.save(caminho)
        self.abrir_html_no_navegador(caminho)


    def gerar_mapa_voo(self, pk, cor):
        caminho = f"maps/mapas/voo_{pk}.html"

        self.marcar_aeroportos()
        self.marcar_conexoes(self.rgbParaHex((180,180,255)))
        self.marcar_voo(pk, cor)

        self.mapa.save(caminho)
        self.abrir_html_no_navegador(caminho)


AEROPORTOS_FILE = './dados/aeroportos.csv'
CONEXOES_FILE = './dados/conexoes.csv'
VOOS_FILE = './dados/voos.csv'

dados = {
    "aeroportos": AEROPORTOS_FILE, 
    "conexoes": CONEXOES_FILE,
    "voos": VOOS_FILE
}

protocolo = sys.argv[1]
print(protocolo)

mapa = Mapa(dados)

if protocolo == "rede":
    mapa.gerar_mapa_rede_aerea()
elif protocolo == "voos":
    mapa.gerar_mapa_voos()
elif protocolo.isnumeric:
    protocolo = int(protocolo)
    if (protocolo >= 0 and protocolo < len(mapa.voos)):
        pk = int(protocolo)
        mapa.gerar_mapa_voo(pk, 'green')
    else:
        print("Id do voo inválido!")
else:
    print("Protocolo de visualização de mapa inválido!")
