#include "SFML/Graphics.hpp"
#include <iostream>
#include "Juego.h"

using namespace sf; //namespace para sfml
using namespace std;

int main(int argc, char* args[]) {

	Juego *partida;
	partida = new Juego(1200, 600, "Cazador de tesoros");//800x600


	return 0;
}