#include "A-estrella.h"

A_estrella::A_estrella() {

}

//-------------------------------------------------- INICIALIZACION DEL MAPA DE NODOS --------------------------------------------------

bool A_estrella::crearMapaNodos() {
	//Se crea el arreglo bidimensional para los nodos
	nodos = new Nodo *[ancho_mapa];//Se asigna el ancho

	for (int i = 0; i < ancho_mapa; i++) {
		nodos[i] = new Nodo[alto_mapa];//Se asigna el largo
	}

	//Se inicializan los nodos con valores default
	for (int y = 0; y < alto_mapa; y++) {
		for (int x = 0; x < ancho_mapa; x++) {
			Nodo *nodo_n = new Nodo(x, y, false, nullptr, false, 1, FLT_MAX, FLT_MAX, 0);
			nodos[x][y] = *nodo_n;
		}
	}
	
	
	//Genracion de obstaculos aleatorios
	srand(time(NULL));
	for (int i = 0; i < alto_mapa; i++) {
		for (int j = 0; j < ancho_mapa; j++) {
			int c = rand() % 7; //0 y 6 
			if (c % 2 == 0) {//Si el aleatorio es par
				//Si esta dentro de los limites del mundo
				if (!(i < 0 && j < 0) && !(i >= alto_mapa && i >= ancho_mapa)) {
					nodos[j][i].es_obstaculo = true;//Se asgina como obstaculo
					//Si el aleatorio no es par y esta dentro de los limites
			        if (c % 3 == 0 && !(i < 0 && j < 0) && !(i >= alto_mapa && i >= ancho_mapa)) { 
						//Si es obstaculo se regresa a default
						nodos[j][i].es_obstaculo = false;
						//y se le asigna un costo del 0 al 3, que son los tipos de suelo
						nodos[j][i].costo_c = rand() % 4;//tipo de suelo
				}
			}
		}
	}
	/*/Pruebas
	for (int i = 0; i < alto_mapa; i++) {
		nodos[7][i].es_obstaculo = true;
	}
	for (int i = 0; i < 7; i++) {
		nodos[i][4].es_obstaculo = true;
		nodos[i][6].es_obstaculo = true;
	}
	nodo_inicio = &nodos[0][5];
	nodo_fin = &nodos[10][5];*/


	//Inicializacion de inicio y final
	
	while (nodo_inicio == nullptr) {//mientras el nodo inicio no este asignado
		int x = rand() % ancho_mapa;//Se crea una posicion aleatoria para el nodo
		int y = rand() % alto_mapa;
		if (!nodos[x][y].es_obstaculo) {//Si no es un obstaculo
			nodo_inicio = &nodos[x][y];//Se asgina como nodo inicio
			nodos[x][y].costo_c = 0;//Se elimina su costo c si es que tenia
		}
	}
	while (nodo_fin == nullptr) {//Mientras el nodo fin no este asignado
		int x = rand() % ancho_mapa;//Se crea una posicion aleatorio
		int y = rand() % alto_mapa;
		//Si la posicion no es un obstaculo y tampoco es el nodo inicio
		if (!nodos[x][y].es_obstaculo && nodo_inicio->x != x && nodo_inicio->y != y) {
			nodo_fin = &nodos[x][y];//Se asigna como destino
			nodos[x][y].costo_c = 0;//Se elimina su coste c
		}
	}
	
	return true;
}

//------------------------------------------------------------ ALGORTIMO A ESTRELLA ---------------------------------------------------------

bool A_estrella::encontrarCamino() {

	Nodo* nodo_actual;//Nodo para guardar la posicion actual
	nodo_actual = nodo_inicio;
	nodo_actual->costo_f = 0;//Se establecen su costos
	nodo_actual->costo_h = 0;
	nodo_actual->costo_g = 0;
	nodos_no_visitados.push_back(nodo_actual);//Se pone en la lista abierta


	while (!nodos_no_visitados.empty() && !nodo_fin->visitado) {//Mientras no se haya visitado el nodo final y la lista abierta no este vacia
		//Se ordenan los nodos de acuerdo a su costo total
		nodos_no_visitados.sort([](const Nodo* lhs, const Nodo* rhs) { return lhs->costo_f < rhs->costo_f; });

		nodo_actual = nodos_no_visitados.front();//Se toma el nodo con menor f de la lista abierta
		nodo_actual->visitado = true;//Se indica que ya se visito el nodo actual
		nodos_no_visitados.pop_front();//Se elimina de la lista abierta
		nodos_visitados.push_front(nodo_actual);//Se inserta en la lista cerrada


		//Se evaluan los vecinos del nodo
		if (esValido(nodo_actual)) {
			if (nodo_actual->y > 0) {//Se toma el nodo vecino de arriba
				//Si no ha sido visitado y es valido
				if (!nodos[nodo_actual->x][nodo_actual->y - 1].visitado && esValido(&nodos[nodo_actual->x][nodo_actual->y - 1])) {
					calculos(&nodos[nodo_actual->x][nodo_actual->y - 1], nodo_actual);//Se calculan sus costos
					nodo_actual->vecinos.push_back(&nodos[nodo_actual->x][nodo_actual->y - 1]);//Se inserta en su lista de vecinos
				}
			}
			if (nodo_actual->y < alto_mapa - 1) {//Se toma el nodo vecino de abajo
				//Si no ha sido visitado y es valido
				if (!nodos[nodo_actual->x][nodo_actual->y + 1].visitado && esValido(&nodos[nodo_actual->x][nodo_actual->y + 1])) {
					calculos(&nodos[nodo_actual->x][nodo_actual->y + 1], nodo_actual);//Se calculan sus costos
					nodo_actual->vecinos.push_back(&nodos[nodo_actual->x][nodo_actual->y + 1]);//Se inserta en su lista de vecinos
				}
			}
			if (nodo_actual->x > 0) {//Se toma el nodo vecino de la izquierda
				//Si no ha sido visitado y es valido
				if (!nodos[nodo_actual->x - 1][nodo_actual->y].visitado && esValido(&nodos[nodo_actual->x - 1][nodo_actual->y])) {
					calculos(&nodos[nodo_actual->x - 1][nodo_actual->y], nodo_actual);//Se calculan sus costos
					nodo_actual->vecinos.push_back(&nodos[nodo_actual->x - 1][nodo_actual->y]);//Se inserta en su lista de vecinos
				}
			}
			if (nodo_actual->x < ancho_mapa - 1) {//Se toma el nodo vecino de la derecha
				//Si no ha sido visitado y es valido
				if (!nodos[nodo_actual->x + 1][nodo_actual->y].visitado && esValido(&nodos[nodo_actual->x + 1][nodo_actual->y])) {
					calculos(&nodos[nodo_actual->x + 1][nodo_actual->y], nodo_actual);//Se calculan sus costos
					nodo_actual->vecinos.push_back(&nodos[nodo_actual->x + 1][nodo_actual->y]);//Se inserta en su lista de vecinos
				}
			}
		}

	}
	if (nodo_fin->visitado) {//Si el nodo final fue visitado
		return true;//exito
	}
	return false;//Si no ha sido visitado: error
}

float A_estrella::calcularH(Nodo* a) {//Se calcula la heursitca del nodo a con la distancia Manhattan
	float h = sqrt((a->x - nodo_fin->x)*(a->x - nodo_fin->x) + (a->y - nodo_fin->y)*(a->y - nodo_fin->y));
	return h;
}

bool A_estrella::esValido(Nodo* a) {//Se revisa si no es obstaculo y esta dentro de los limites
	if (a->es_obstaculo || a->x < 0 || a->y < 0 || a->x >= ancho_mapa || a->y >= alto_mapa) {
		return false;
	}
	return true;
}

void A_estrella::calculos(Nodo* a, Nodo* p) {//Se calculan los nuevos costos
	float nueva_h = calcularH(a);
	float nueva_f = nueva_h + a->costo_g + a->costo_c;

	if (a->costo_f == FLT_MAX || a->costo_f > nueva_f) {//Si es un nuevo nodo (tiene valor por default) o si su f es mayor a la nueva f
		a->costo_f = nueva_f;//Se acutalizan sus costos
		a->costo_h = nueva_h;
		a->padre = p;//Se le asigna su padre
		nodos_no_visitados.push_back(a);//Se inserta en la lista abierta
	}
}

//--------------------------------------------------- FUNCIONES PARA LA PARTE GRAFICA ----------------------------------------------

vector <int> A_estrella::getCamino() {
	if (nodo_fin != nullptr)//Si se encontro un camino
	{
		Nodo *p = nodo_fin;//Se toma el nodo final
		vector <int> camino;//Vector para guardar las coordenadas
		while (p->padre != nullptr)//Mientras el nodo actual (p) tenga padre
		{
			camino.push_back(p->x);//Se toman las coordenadas del nodo actual (inicio de la linea)
			camino.push_back(p->y);
			camino.push_back(p->padre->x);//y las de su padre (fin de la linea)
			camino.push_back(p->padre->y);

			p = p->padre;//Se avanza hacia el padre del nodo actual
		}
		return camino;
	}
}

vector <Nodo*> A_estrella::getCaminoTextura() {
	if (nodo_fin != nullptr)//Si se encontro un camino
	{
		Nodo *p = nodo_fin;//Se toma el nodo final
		vector <Nodo*> camino;//Vector para guardar las coordenadas
		while (p->padre != nullptr)//Mientras el nodo actual (p) tenga padre
		{
			camino.push_back(p);

			p = p->padre;//Se avanza hacia el padre del nodo actual
		}
		if (p->padre == nullptr) {
			camino.push_back(p);
		}
		return camino;
	}
}

vector <Nodo*> A_estrella::getNodosVisitados() {//Regresa la lista cerrada
	vector <Nodo*> visitados;
	for (auto it = begin(nodos_visitados); it != end(nodos_visitados); ++it) {
		visitados.push_back(*it);
	}
	return visitados;
}

bool A_estrella::ejecutar_a_estrella() {//ejecucion del algoritmo
	crearMapaNodos();
	return encontrarCamino();
}

vector <int> A_estrella::getInicio() {//Regresa las coordenadas del nodo inicio
	vector <int> inicio;
	inicio.push_back(nodo_inicio->x);
	inicio.push_back(nodo_inicio->y);

	return inicio;
}

vector <int> A_estrella::getFinal() {//Regresa las coordenadas del nodo final
	vector <int> fin;
	fin.push_back(nodo_fin->x);
	fin.push_back(nodo_fin->y);

	return fin;
}

vector <int> A_estrella::getObstaculos() {//Regresa las coordenadas de los nodos obstaculos
	vector <int> obstaculos;
	for (int y = 0; y < alto_mapa; y++) {
		for (int x = 0; x < ancho_mapa; x++) {
			if (nodos[x][y].es_obstaculo) {
				obstaculos.push_back(nodos[x][y].x);
				obstaculos.push_back(nodos[x][y].y);
			}
		}
	}
	return obstaculos;
}

vector <int> A_estrella::getSuelos() {//Regresa las coordenadas de los nodos de suelo diferente
	vector <int> suelos;
	for (int y = 0; y < alto_mapa; y++) {
		for (int x = 0; x < ancho_mapa; x++) {
			if (nodos[x][y].costo_c != 0) {
				suelos.push_back(nodos[x][y].costo_c);
				suelos.push_back(nodos[x][y].x);
				suelos.push_back(nodos[x][y].y);
			}
		}
	}
	return suelos;
}

bool A_estrella::reiniciar() {//Reinicia el algoritmo
	nodo_inicio = nullptr;
	nodo_fin = nullptr;
	nodos = nullptr;
	nodos_no_visitados.clear();
	nodos_visitados.clear();
	return ejecutar_a_estrella();
}

A_estrella::~A_estrella() {
	
}