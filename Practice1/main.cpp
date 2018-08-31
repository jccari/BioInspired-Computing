// compile: g++ main.cpp GridWorld.cpp Actors.cpp -o test.out
// execute: ./test.out

#include <iostream>
#include "GridWorld.cpp"

using namespace std;

#define RANDOM 0
#define PROBABILISTIC 1

// configurations
#define EXIT 0
#define TESTS 1
#define CHANGE_ITERATIONS 2
#define CHANGE_WORLD_ITERATIONS 3
#define CHANGE_WORLD_TYPE 4
#define CHANGE_BIRTH_RATE 5
#define CHANGE_MORTALITY_RATE 6
#define CLEAR_SCREEN 99

void makeTests(GridWorld* world, int iterations, int worldIterations, int type){ // type : 0 -> random
	float aliveGrassTotal = 0; float deadGrassTotal = 0 ;  // all values in percentajes
	float aliveRabbitTotal = 0; float deadRabbitTotal = 0 ;  // all values in percentajes
	float aliveWolfTotal = 0; float deadWolfTotal = 0 ;  // all values in percentajes
	vector< vector<float> > results;
	
	for (int i = 0; i < iterations; ++i){
		if( type == RANDOM)
			world->randomWorld(worldIterations);
		else 
			world->probabilisticWorld(worldIterations);

		//world->printStats();
		world->getStats(results);

		aliveGrassTotal += results[0][1];
		deadGrassTotal += results[0][2];

		aliveRabbitTotal += results[1][1];
		deadRabbitTotal += results[1][2];

		aliveWolfTotal += results[2][1];
		deadWolfTotal += results[2][2];

		results.clear();
	}

	cout << "\n\t ESTADISTICAS PROMEDIO " << iterations << " ITERACIONES" << endl;
	cout << " PASTOS: " << endl;
	cout << " 	vivos: " << (float) aliveGrassTotal/iterations << endl;
	cout << " 	muertos: " << (float) deadGrassTotal/iterations << endl;

	cout << " CONEJOS: " << endl;
	cout << " 	vivos: " << (float) aliveRabbitTotal/iterations << endl;
	cout << " 	muertos: " << (float) deadRabbitTotal/iterations << endl;

	cout << " LOBOS: " << endl;
	cout << " 	vivos: " << (float) aliveWolfTotal/iterations << endl;
	cout << " 	muertos: " << (float) deadWolfTotal/iterations << endl;
}

int printOptions(){
	int choose;
	cout << "[1] Empezar Pruebas" << endl;
	cout << "[2] Setear Numero de Iteraciones" << endl;
	cout << "[3] Setear Numero de Iteraciones del Mundo" << endl;
	cout << "[4] Cambiar tipo de Mundo" << endl;
	cout << "[5] Cambiar tasa de Natalidad" << endl;
	cout << "[6] Cambiar tasa de Mortalidad" << endl;
	cout << "[99] Limpiar Pantalla" << endl;
	cout << "[0] Salir" << endl;

	cout << "Ingresa una opcion: "; cin >> choose;
	return choose;
}


int main(int argc, char const *argv[])
{
	int choose;
	// Default values 
	int iters = 50;
	int worldIters = 10;
	int worldType = RANDOM;
	int birthrate = 3;
	int mortalityrate = 5;
	int prob = 70;

	GridWorld* world = new GridWorld(10);

	
	do{
		choose = printOptions();
		switch(choose){
			case TESTS:{
				cout << " \tCONFIGURACIONES POR DEFAULT " << endl;
				cout << "	  Tipo de Mundo: " << worldType << endl;
				if(worldType==PROBABILISTIC) 
					cout << " 	    Probabilidad: " << prob << "%" << endl;
				cout << "	  Iteraciones: " << iters << endl;
				cout << "	  Iteraciones del Mundo: " << worldIters << endl;
				cout << "	  Natalidad (t): " << birthrate << endl;
				cout << "	  Mortalidad (v): " << mortalityrate << endl;

				makeTests(world, iters, worldIters, worldType);
				break;
			}
			case CHANGE_ITERATIONS: {
				cout << "Ingrese el nuevo numero de Iteraciones: "; 
				cin>> iters;
				break;
			}
			case CHANGE_WORLD_ITERATIONS:{
				cout << "Ingrese el nuevo numero de Iteraciones del Mundo: "; 
				cin>> worldIters;
				break;
			}
			case CHANGE_WORLD_TYPE: {
				if(worldType == RANDOM){
					cout << "[OK] Tipo de Mundo: PROBABILISTICO" << endl;
					//cout << " Ingrese la probabilidad: "; cin >> prob;
					//world->setProbability(prob);
					worldType = PROBABILISTIC;
				}
				else {
					cout << "[OK] Tipo de Mundo: ALEATORIO" << endl;
					worldType = RANDOM;
				}
				break;
			}
			case CHANGE_BIRTH_RATE:{
				cout <<" Ingrese el nuevo valor de Natalidad:"; cin>> birthrate;
				world->setBirthRate(birthrate);
				break;
			}
			case CHANGE_MORTALITY_RATE:{
				cout <<" Ingrese el nuevo valor de Natalidad:"; cin>> mortalityrate;
				world->setBirthRate(mortalityrate);
				break;
			}
			case CLEAR_SCREEN:{
				system("clear");
				break;
			}
		}
		cout << "\n ------------------------------------------------------------------ \n" ;
	}while( choose != EXIT );

	return 0;
}