#pragma once
#include <vector>
#include <list>

using namespace std;

class Nodo {
public:
	Nodo(float pos_x, float pos_y, bool obstaculo, Nodo *n_padre, bool es_visitado, 
		float cost_g, float cost_h, float cost_f, float cost_c);
	Nodo();
	bool es_obstaculo = false;
	bool visitado = false;
	float costo_h;
	float costo_g;
	float costo_f;
	float costo_c;
	int x;
	int y;
	Nodo *padre;
	vector <Nodo*> vecinos;//Lista de vecinos para mostrar en la parte grafica

};
