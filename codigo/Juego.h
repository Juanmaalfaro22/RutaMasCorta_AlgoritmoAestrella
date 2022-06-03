#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include "A-estrella.h"
#include "Nodo.h"
using namespace sf;
using namespace std;


class Juego {

public:
	Juego(int resolucion_x, int resolucion_y, std::string titulo);
	Juego();


private:
	//Variables para la ventana
	int fps = 0;
	int resolucion_x = 0;
	int resolucion_y = 0;

	//Variables de control
	bool iniciado = false;
	bool movimiento = false;
	bool mostrar_datos = false;
	bool camino_encontrado = false;
	int nodo_actual;
	
	//Variables para los graficos
	RenderWindow * ventana;
	RectangleShape  **matrizCuadrados;
	vector <Nodo*> ubicacion_nodos;
	RectangleShape * boton, *boton_2;
	Color *color_boton;
	Vector2i posicion_mouse;
	Vector2f posicion;
	Vector2f medidas_cuadrado = Vector2f(50.f, 50.f);
	Event *evento;
	Texture *piso, *inicio, *fin, *obstaculo, *suelo1, *suelo2;
	Sprite * s_piso, *s_inicio, *s_fin;
	Clock *reloj;
	Time * tiempo1;


	//Ciclo infinito
	void gameLoop();

	//Funciones de inicializacion
	void crearMatriz();
	void inicializarVariables();
	void inicializarTexturas();

	//Funciones de reinicio
	void reiniciarMatriz();
	void reiniciar();
	
	//Funciones para dibujar
	void dibujar();
	void dibujarObsSuelos();
	void dibujarMapa();
	void dibujarInterfaz();
	void dibujarMatriz();
	void caminoExplorado();
	void ejecutarCamino();
	void mostrarDatosNodo(Nodo* n);
	RectangleShape crearCuadrado(Vector2f posicion);

	//Funciones para eventos (mouse y botones)
	void procesarEventos();
	void procesarMouse();
	
	
	
	

};

