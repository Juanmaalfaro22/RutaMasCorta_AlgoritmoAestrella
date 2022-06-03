#pragma once
#include <string.h>
#include <math.h>
#include <vector>
#include <list>
#include <stdlib.h>
#include <time.h>
#include "Nodo.h"

using namespace std;

class A_estrella{

public:
	A_estrella();
	vector <int> getCamino();//regresa el camino encontrado
	vector <Nodo*> getCaminoTextura();//regresa el camino encontrado (se usa con texturas)
	vector <Nodo*> getNodosVisitados();//regresa la lista cerrada
	vector <int> getInicio();//regresa las coordenadas del inicio
	vector <int> getFinal();//regresa las coordenadas del final
	vector <int> getObstaculos();//regresa la coordenadas de los obstaculos
	vector <int> getSuelos();//regresa las coordenadas de los suelos y su valor
	bool ejecutar_a_estrella();//para la ejecucion
	bool reiniciar();//reinicia el algoritmo
	~A_estrella();

private:
	Nodo **nodos;//matriz para el mapa
	int ancho_mapa = 16;
	int alto_mapa = 12;

	list<Nodo*> nodos_no_visitados;//lista abierta
	list<Nodo*> nodos_visitados;//lista cerrada

	Nodo *nodo_inicio = nullptr;
	Nodo *nodo_fin = nullptr;

protected:
	bool crearMapaNodos();
	bool encontrarCamino();
	float calcularH(Nodo* a);
	bool esValido(Nodo* a);
	void calculos(Nodo* a, Nodo* p);
};
