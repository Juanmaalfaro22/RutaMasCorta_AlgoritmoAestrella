#include "Nodo.h"

Nodo::Nodo(float pos_x, float pos_y, bool obstaculo, Nodo *n_padre, bool es_visitado, float cost_g, float cost_h, float cost_f, float cost_c) {
	x = pos_x;
	y = pos_y;
	es_obstaculo = obstaculo;
	padre = n_padre;
	visitado = es_visitado;
	costo_g = cost_g;
	costo_h = cost_h;
	costo_f = cost_f;
	costo_c = cost_c;
};

Nodo::Nodo(){

}