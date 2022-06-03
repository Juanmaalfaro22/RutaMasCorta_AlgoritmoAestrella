#include "Juego.h"
#include "resource.h"

A_estrella algoritmo;

Juego::Juego(int resolucionX, int resolucionY, std::string titulo) {
	//Constructor para la ventana e inicio del juego
	fps = 30;//Cuadros por segundo
	resolucion_x = resolucionX;
	resolucion_y = resolucionY;
	ventana = new RenderWindow(VideoMode(resolucionX, resolucionY), titulo);
	ventana->setFramerateLimit(fps);
	inicializarVariables();
	inicializarTexturas();

	gameLoop();
}

Juego::Juego() {

}

//------------------------------------------------------- FUNCIONES DE INICIALIZACION ------------------------------------------------

//Funcion para la inicializacion de variables
void Juego::inicializarVariables() {
	evento = new Event;
	reloj = new Clock;
	tiempo1 = new Time;
	piso = new Texture;
	inicio = new Texture;
	fin = new Texture;
	obstaculo = new Texture;
	suelo1 = new Texture;
	suelo2 = new Texture;
	s_piso = new Sprite;
	s_fin = new Sprite;
	s_inicio = new Sprite;
	boton = new RectangleShape;
	boton_2 = new RectangleShape;
	color_boton = new Color();
	*color_boton = Color::Blue;
	posicion = Vector2f(0, 0);
	crearMatriz();
}

//Funcion para la asignacion de texturas
void Juego::inicializarTexturas() {
	piso->loadFromFile("RECURSOS/img/MAPA.jpg");
	inicio->loadFromFile("RECURSOS/img/PERSONAJE4.png");
	fin->loadFromFile("RECURSOS/img/TESORO.png");
	obstaculo->loadFromFile("RECURSOS/img/PARED.png");
	suelo1->loadFromFile("RECURSOS/img/POPO.png");
	suelo2->loadFromFile("RECURSOS/img/HIELO.png");
	s_piso->setTexture(*piso);
	s_inicio->setTexture(*inicio);
	s_fin->setTexture(*fin);
}

//Funcion para la inicializacion de la matriz
void Juego::crearMatriz() {
	int numero_cuadrados_x = (resolucion_x - 400) / (int)medidas_cuadrado.x;//Se calcula el numero de cuadrados (nodos) en x
	int numero_cuadrados_y = resolucion_y / (int)medidas_cuadrado.x;//y en y
	matrizCuadrados = new RectangleShape *[numero_cuadrados_x];//Se crea la primera fila de la matriz

	for (int i = 0; i < numero_cuadrados_x; i++) {
		matrizCuadrados[i] = new RectangleShape[numero_cuadrados_y];//En cada posicion de la primera fila, se inserta una columna
	}

	for (int y = 0; y < numero_cuadrados_y; y++) {
		for (int x = 0; x < numero_cuadrados_x; x++) {
			matrizCuadrados[x][y] = crearCuadrado(posicion);//En cada posicion de la matriz se crea un cuadrado
			posicion.x += medidas_cuadrado.x;//Se aumenta la posicion x para el siguiente cuadrado
		}
		posicion.x = 0;//Se reinicia la posicion x cuando ya se termino la primera fila
		posicion.y += medidas_cuadrado.y;//Se avanza a la siguiente fila
	}
}

//Funcion para el ciclo infinito
void Juego::gameLoop() {
	while (ventana->isOpen()) {
		procesarMouse();
		procesarEventos();
		dibujar();
	}
}

//------------------------------------------------------- FUNCIONES PARA DIBUJAR CON TEXTURA ------------------------------------------------
//Funcion principal de dibujo
void Juego::dibujar() {
	ventana->clear();

	if (iniciado) {
		dibujarMatriz();
		dibujarObsSuelos();
		dibujarMapa();

		*tiempo1 = reloj->getElapsedTime();//El camino se ejecuta cada 200ms y solo si se encontró
		if (tiempo1->asMilliseconds() > 200 && camino_encontrado) {
			ejecutarCamino();
			reloj->restart();
		}
		//Si no se encontró un camino, se muetran todos los nodos que fueron explorados
		if (!camino_encontrado && mostrar_datos) {
			caminoExplorado();
		}
	}
	dibujarInterfaz();

	ventana->display();
}

//Funcion para dibujar la interfaz (botones, avisos, informacion)
void Juego::dibujarInterfaz() {
	Font fuente;//Fuente para usar en la interfaz
	fuente.loadFromFile("RECURSOS/fuentes/OpenSerif-Book.ttf");
	//Boton Inicio/Reinicio
	Text texto;
	boton->setSize(Vector2f(200, 50));
	boton->setFillColor(*color_boton);
	boton->setPosition(Vector2f(900, 455));
	texto.setFont(fuente);
	if (!iniciado) {//Si aun no es iniciado, se muestra el texto iniciar
		texto.setPosition(955, 460);
		texto.setString("Iniciar");
	}
	else {//Una vez iniciado, se cambia el texto a iniciar
		texto.setString("Reiniciar");
		texto.setPosition(Vector2f(940, 460));
	}
	
	//Boton Mostrar datos
	Text texto_2;
	boton_2->setSize(Vector2f(200, 50));
	boton_2->setFillColor(Color::Red);
	if (mostrar_datos) {//Cuando el boton es presionado, se hace mas opaco
		boton_2->setFillColor(Color(255, 0, 0, 150));
	}
	boton_2->setPosition(Vector2f(900, 400));
	texto_2.setCharacterSize(27);
	texto_2.setString("Mostrar datos");
	texto_2.setFont(fuente);
	texto_2.setPosition(Vector2f(910, 405));

	//Nodo de informacion
	RectangleShape ejemplo(medidas_cuadrado);
	ejemplo.setPosition(Vector2f(975, 50));
	ejemplo.setFillColor(Color::White);
	Text texto_3;
	texto_3.setFont(fuente);
	texto_3.setCharacterSize(22);
	texto_3.setString("G = costo para avanzar.\nC = costo por tipo de suelo.\nH = heurística.\nF = costo total.");
	texto_3.setPosition(Vector2f(875, 150));

	//Posicionamiento de las letras que representan los costos
	Text info[4];
	for (int i = 0; i < 4; i++) {
		info[i].setFont(fuente);
		info[i].setCharacterSize(18);
		info[i].setFillColor(Color::Black);
	}

	info[0].setString("G");
	info[1].setString("C");
	info[2].setString("H");
	info[3].setString("F");
	info[0].setPosition(ejemplo.getPosition().x + 5, ejemplo.getPosition().y);//Izquierda superior
	info[1].setPosition(ejemplo.getPosition().x + 5, (ejemplo.getPosition().y) + 25);//Izquierda inferior
	info[2].setPosition((ejemplo.getPosition().x) + 30, ejemplo.getPosition().y);//Derecha superior
	info[3].setPosition((ejemplo.getPosition().x) + 30, (ejemplo.getPosition().y) + 25);//Derecha inferior

	//Texto de aviso
	Text resultado;
	resultado.setString("No hay camino");
	resultado.setPosition(860, 300);
	resultado.setFont(fuente);
	resultado.setFillColor(Color::Red);
	resultado.setCharacterSize(40);

	//Instrucciones para dibujar la interfaz
	if (!camino_encontrado && iniciado) {//Si el camino no fue encontrado una vez iniciado el programa
		ventana->draw(resultado);//Se muestra el mensaje
	}

	ventana->draw(*boton);
	ventana->draw(*boton_2);
	ventana->draw(texto);
	ventana->draw(texto_2);
	ventana->draw(ejemplo);
	ventana->draw(texto_3);
	for (int i = 0; i < 4; i++) {
		ventana->draw(info[i]);
	}

}

//Funcion para dibujar el mapa
void Juego::dibujarMapa() {
	vector <int> inicio = algoritmo.getInicio();//Se toma la posicion del nodo inicio 
	vector <int> fin = algoritmo.getFinal();//Se toma la posicion del nodo final

	//Las coordenadas obtenidas mediante el objeto "algoritmo" deben ser multiplicadas por 50
	//para que se ajusten al tamaño en pixeles que se maneja en la parte gráfica.

	if (!movimiento) {//Antes de iniciar el movimiento
		s_inicio->setPosition(inicio[0] * 50, inicio[1] * 50);//Se establece la posicion inicial del agente
		s_fin->setPosition(fin[0] * 50, fin[1] * 50);//Y la posicion del objetivo
		ventana->draw(*s_inicio);
		ventana->draw(*s_fin);
	}

	if (!mostrar_datos) {//Si no se estan mostrando los datos, se dibuja el mapa con texturas
		ventana->draw(*s_piso);

		vector <int> obstaculos = algoritmo.getObstaculos();//Se toman las posiciones de los obstaculos generados
		
		Sprite s_obs(*obstaculo);
		//Se avanzan dos posiciones en cada ciclo ya que las coordenadas (x,y) de cada
		//obtaculo estan colocados de forma sucesiva
		for (int i = 0; i < obstaculos.size(); i += 2) {//Se dibuja la textura en las posiciones obtenidas
			s_obs.setPosition(obstaculos[i] * 50, obstaculos[i + 1] * 50);
			ventana->draw(s_obs);
		}

		vector <int> suelos = algoritmo.getSuelos();//Se toman las posiciones de los suelos generados
		Sprite s_suelo;
		//Se avazan tres posiciones ya que la informacion de los suelos (tipo,x,y) esta colocados de forma sucesiva
		for (int i = 0; i < suelos.size(); i += 3) {
			switch (suelos[i])
			{
			case 1: {
				s_suelo.setTexture(*suelo1);//Si el suelo es de tipo 1 (excremento) se toma la textura suelo1
				s_suelo.setPosition(suelos[i + 1] * 50, suelos[i + 2] * 50);
				break;
			}

			case 2: {
				s_suelo.setTexture(*suelo2);//Si el suelo es de tipo 2 (hielo) se toma la textura suelo2
				s_suelo.setPosition(suelos[i + 1] * 50, suelos[i + 2] * 50);
				break;
			}
			case 3: {
				s_suelo.setTexture(*suelo2);//El suelo dos abarca 2 casos para que aparezcan mas suelos de este tipo
				s_suelo.setPosition(suelos[i + 1] * 50, suelos[i + 2] * 50);
				break;
			}
			}
			ventana->draw(s_suelo);
		}
	}

	ventana->draw(*s_fin);
	ventana->draw(*s_inicio);

}

//Funcion para ejecutar el camino
void Juego::ejecutarCamino() {
	if (!movimiento) {//antes de iniciar el movimiento
		nodo_actual = ubicacion_nodos.size() - 1;//Se toma la cantidad de nodos que se van a recorrer
	}

	if (!ubicacion_nodos.empty() && nodo_actual >= 0) {//Si hay nodos para recorrer y el nodo actual es valido (>=0)
		movimiento = true;//Se inicia el movimiento
		int x = 0, y = 0;
		//Si hay diferencia entre la posicion x del cazador de tesoros y el nodo actual
		if (ubicacion_nodos[nodo_actual]->x * 50 - s_inicio->getPosition().x != 0) {
			if (ubicacion_nodos[nodo_actual]->x * 50 - s_inicio->getPosition().x < 0) {//Si la diferencia es negativa
				x = -10;//Se mueve hacia la izquierda
			}
			else {//Si no
				x = 10;//Se mueve hacia la derecha
			}
		}
		//Si hay diferencia entre la posicion y del cazador de tesoros y el nodo actual
		if (ubicacion_nodos[nodo_actual]->y * 50 - s_inicio->getPosition().y != 0) {
			if (ubicacion_nodos[nodo_actual]->y * 50 - s_inicio->getPosition().y < 0) {//Si la diferencia es negativa
				y = -10;//Se mueve hacia arriba
			}
			else {//Si no
				y = 10; //Se mueve hacia abajo
			}
		}

		s_inicio->move(x, y);//El cazador se mueve en la direccion indicada por x y y

		//Se debe decrementar la posicion del nodo actual, ya que el camino se toma del final al inicio
		//Por lo que se debe empezar a recorrer el vector de nodos desde el final

		//Si la posicion actual del cazador es la misma que la del nodo actual
		if (s_inicio->getPosition().x == ubicacion_nodos[nodo_actual]->x * 50 && s_inicio->getPosition().y == ubicacion_nodos[nodo_actual]->y * 50) {
			nodo_actual--;//Se avanza hacia el siguiente nodo
		}

	}
}


//------------------------------------------------------- FUNCIONES PARA LA VIZUALICACION DE DATOS ------------------------------------------
//Funcion para dibujar la matriz (sin textura) donde se muestran los datos
void Juego::dibujarMatriz() {
	int numero_cuadrados_x = (resolucion_x - 400) / (int)medidas_cuadrado.x;//Se calcula la cantiadad de cuadrados en x
	int numero_cuadrados_y = resolucion_y / (int)medidas_cuadrado.x;//Se calcula la cantidad de cuadrados en y

	//Se dibuja la matriz
	for (int y = 0; y < numero_cuadrados_y; y++) {
		for (int x = 0; x < numero_cuadrados_x; x++) {
			ventana->draw(matrizCuadrados[x][y]);
		}
	}

	//Se muestra la informacion de los nodos
	ubicacion_nodos = algoritmo.getCaminoTextura();//Se toma el camino encontrado por el algoritmo
	if (!ubicacion_nodos.empty()) {//Si existe un camino
		for (int n = ubicacion_nodos.size() - 1; n > nodo_actual; n--) {
			mostrarDatosNodo(ubicacion_nodos[n]);//Se dibujan los valores de los costos de cada nodo visitado y sus vecinos

		}
	}
}

//Funcion para dibujar los obstaculos y suelos sin textura
void Juego::dibujarObsSuelos() {
	vector <int> inicio = algoritmo.getInicio();//Se toma las cooredenadas del inicio
	vector <int> fin = algoritmo.getFinal();//Se toma las cooredenadas del final

	matrizCuadrados[inicio[0]][inicio[1]].setFillColor(Color::Blue);//Se asigna el color azul al nodo inicio
	matrizCuadrados[fin[0]][fin[1]].setFillColor(Color::Red);//Se asigna el color rojo al nodo final

	vector <int> obstaculos = algoritmo.getObstaculos();//Se toman las coordenadas de los obstaculos
	for (int i = 0; i < obstaculos.size(); i += 2) {
		matrizCuadrados[obstaculos[i]][obstaculos[i + 1]].setFillColor(Color::Black);//Se les asigna el color negro
	}

	vector <int> suelos = algoritmo.getSuelos();//Se toma la informaciond de los tipos de suelo
	for (int i = 0; i < suelos.size(); i += 3) {
		switch (suelos[i])
		{
		case 1://Se asigna el color amarillo al tipo de suelo 1 (excremento)
			matrizCuadrados[suelos[i + 1]][suelos[i + 2]].setFillColor(Color::Yellow); break;
		case 2://Se asigna el color cyan al tipo de suelo 2 (excremento)
			matrizCuadrados[suelos[i + 1]][suelos[i + 2]].setFillColor(Color::Cyan); break;
		case 3:
			matrizCuadrados[suelos[i + 1]][suelos[i + 2]].setFillColor(Color::Cyan); break;
		}
	}
}

//Funcion para mostrar los datos de cada nodo y sus vecinos
void Juego::mostrarDatosNodo(Nodo* n) {
	Font fuente;
	fuente.loadFromFile("RECURSOS/fuentes/OpenSerif-Book.ttf");//Se asigna la fuente

	Text **datos = new Text *[4];
	for (int i = 0; i < 4; i++) {//Se crea la matriz de acuerdo a la cantidad de nodos vecinos del nodo en cuestion
		datos[i] = new Text[n->vecinos.size()];
	}

	//Se crea el cuadro (rojo) que indica los nodos visitados
	RectangleShape nodo_camino(Vector2f(10, 10));
	nodo_camino.setFillColor(Color::Red);
	nodo_camino.setPosition((n->x * 50) + 20, (n->y * 50) + 20);
	ventana->draw(nodo_camino);

	//Se crea el cuadro (opaco) que indica los nodos vecinos
	RectangleShape nodo_vecino(Vector2f(50, 50));
	nodo_vecino.setFillColor(Color(0, 0, 0, 50));

	for (int ren = 0; ren < n->vecinos.size(); ren++) {
		for (int col = 0; col < 4; col++) {//Se le dan propiedades al texto de los costos
			datos[col][ren].setFont(fuente);
			datos[col][ren].setCharacterSize(18);
			datos[col][ren].setFillColor(Color::Black);
		}
		//Se asigan los valores de los costos del nodo vecino en la columna correspondiente
		datos[0][ren].setString(std::to_string((int)n->vecinos[ren]->costo_g));
		datos[1][ren].setString(std::to_string((int)n->vecinos[ren]->costo_c));
		datos[2][ren].setString(std::to_string((int)n->vecinos[ren]->costo_h));
		datos[3][ren].setString(std::to_string((int)n->vecinos[ren]->costo_f));

		//Se asigna la posicion del texto de cada costo del nodo vecino
		datos[0][ren].setPosition(n->vecinos[ren]->x * 50, n->vecinos[ren]->y * 50);//Izquierda superior
		datos[1][ren].setPosition(n->vecinos[ren]->x * 50, (n->vecinos[ren]->y * 50) + 27);//izquierda inferior
		datos[2][ren].setPosition((n->vecinos[ren]->x * 50) + 24, n->vecinos[ren]->y * 50);//Derecha superior
		datos[3][ren].setPosition((n->vecinos[ren]->x * 50) + 24, (n->vecinos[ren]->y * 50) + 27);//Derecha inferior

		//Se asigna la posicion del nodo vecino (opaco) y se dibuja
		nodo_vecino.setPosition((n->vecinos[ren]->x * 50), (n->vecinos[ren]->y * 50));
		ventana->draw(nodo_vecino);
	}

	for (int ren = 0; ren < n->vecinos.size(); ren++) {
		for (int col = 0; col < 4; col++) {
			ventana->draw(datos[col][ren]);//Se dibujan los datos de los costos
		}
	}
}

//Funcion para mostrar los datos de cada nodo cuando no se encuentra camino
void Juego::caminoExplorado() {
	vector <Nodo*> visitados = algoritmo.getNodosVisitados();//Se toman los nodos de la lista de nodos visitados (lista cerrada)
	if (!visitados.empty()) {//Si hay nodos visitados
		for (int n = 0; n < visitados.size(); n++) {
			mostrarDatosNodo(visitados[n]);//Se muestran sus datos

		}
	}
}

//Funcion para crear los cuadrados
RectangleShape Juego::crearCuadrado(Vector2f posicion) {
	RectangleShape cuadrado(medidas_cuadrado);
	cuadrado.setPosition(posicion);
	cuadrado.setFillColor(Color::White);
	cuadrado.setOutlineThickness(2.f);
	cuadrado.setOutlineColor(Color(0, 0, 0));

	return cuadrado;
}

//----------------------------------------------------- FUNCIONES PARA LOS EVENTOS DEL MOUSE ------------------------------------------------
void Juego::procesarEventos() {
	while (ventana->pollEvent(*evento)) {//Se revisa si hay eventos
		switch (evento->type)//Si los hay se toma el tipo
		{
		case Event::Closed: {//Si se da clic en el boton cerrar
			ventana->close();//Se cierra la ventana
			exit(1);//Se termina el progrma
		}break;

		case Event::MouseButtonPressed: {
			if (Mouse::isButtonPressed(Mouse::Left)) {//Cuando el boton izquierdo sea presionado
				//Y este dentro del area del boton de inicio/reinicio
				if (posicion_mouse.x > boton->getPosition().x && posicion_mouse.x < boton->getPosition().x + boton->getSize().x
					&& posicion_mouse.y > boton->getPosition().y && posicion_mouse.y < boton->getPosition().y + boton->getSize().y) {
					*color_boton = Color::Red;//El color del boton de inicio/reinicio cambiara a rojo
				}
			}
		}break;

		case Event::MouseButtonReleased: {//Cuando se suelte el boton
			//Si esta dentro del area del boton de inicio/reinicio
			if (posicion_mouse.x > boton->getPosition().x && posicion_mouse.x < boton->getPosition().x + boton->getSize().x
				&& posicion_mouse.y > boton->getPosition().y && posicion_mouse.y < boton->getPosition().y + boton->getSize().y) {
				*color_boton = Color::Blue;//regresa el color del boton inicio/reinicio a azul

				camino_encontrado = algoritmo.reiniciar();//Ejecuta el algoritmo
				reiniciar();//Ejecuta las funciones graficas
			}
			//Si esta dentro del boton de mostrar datos
			if (posicion_mouse.x > boton_2->getPosition().x && posicion_mouse.x < boton_2->getPosition().x + boton_2->getSize().x
				&& posicion_mouse.y > boton_2->getPosition().y && posicion_mouse.y < boton_2->getPosition().y + boton_2->getSize().y) {
				if (!mostrar_datos) {//Si no se estaban mostrando los datos
					mostrar_datos = true;//Se muestran
				}
				else {//Si ya se estaban mostrando los datos
					mostrar_datos = false;//Los oculta
				}

			}
		}break;

		}
	}
}

void Juego::procesarMouse() {
	posicion_mouse = Mouse::getPosition(*ventana);//Se toma la posicion del mouse dentro de la ventana
	posicion_mouse = Vector2i(ventana->mapPixelToCoords(posicion_mouse));//Se convierten la posicon del mouse de pixeles a coordenadas
}

//----------------------------------------------------- FUNCIONES PARA EL REINICIO DEL PROGRAMA ---------------------------------------------

//Funcion para le reinicio de variables
void Juego::reiniciar() {
	reiniciarMatriz();
	iniciado = true;
	movimiento = false;
	nodo_actual = 0;
}

//Funcion para el reinicio (repintado) de la matriz (sin textura)
void Juego::reiniciarMatriz() {
	int numero_cuadrados_x = (resolucion_x - 400) / (int)medidas_cuadrado.x;
	int numero_cuadrados_y = resolucion_y / (int)medidas_cuadrado.x;

	for (int y = 0; y < numero_cuadrados_y; y++) {
		for (int x = 0; x < numero_cuadrados_x; x++) {//Se repitan los nodos de la matriz ya creada
			matrizCuadrados[x][y].setFillColor(Color::White);
			matrizCuadrados[x][y].setOutlineThickness(2.f);
			matrizCuadrados[x][y].setOutlineColor(Color(0, 0, 0));
		}
	}
}
