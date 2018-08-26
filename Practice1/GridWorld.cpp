#include <iostream>
#include <vector>
#include <random>

#include "Actors.cpp"

#define ACTOR_DIE 0

#define ACTION_EAT 1
#define ACTION_MOVE_UP 2
#define ACTION_MOVE_DOWN 3
#define ACTION_MOVE_RIGHT 4
#define ACTION_MOVE_LEFT 5
#define ACTION_DO_NOTHING 6

using namespace std;

class GridWorld{
	private:
		int size;

		int t_grass, t_rabbit, t_wolf;
		int v_rabbit, v_wolf;

		vector< vector< vector<Actor*> > > world;

		vector< Grass*> grassList;
		vector< Rabbit*> rabbitList;
		vector< Wolf*> wolfList;

	public:
		GridWorld(int n){
			size = n;

			inicializeWorld();
		}

		void inicializeWorld(){
			world.resize(size);
			for (int i = 0; i < size; ++i){
				world[i].resize(size);
			}
			/*
			for (int i = 0; i < size; ++i){
				for (int j = 0; j < size; ++j){
					world[i][j][0] = nullptr;
				}
			}
			*/
			world[1][1].push_back( new Rabbit(1,1) );
			world[1][1].push_back( new Wolf(1,1) );
			world[1][1].push_back( new Grass(1,1) );
		}

		void randomBehaviour(Actor* actor){
		   
		    int action = getRandomNumber(1,6);

			cout << action << endl;
			switch(action){
				case ACTION_EAT :
					break;
				case ACTION_MOVE_UP :
					break;
				case ACTION_MOVE_DOWN :
					break;
				case ACTION_MOVE_RIGHT :
					break;
				case ACTION_MOVE_LEFT :
					break;
				case ACTION_DO_NOTHING :
					break;
			}
		}

		void probabilisticBehaviour(){

		}

		void print(){
			for (int i = 0; i < size; ++i){
				for (int j = 0; j < size; ++j){
					if(world[i][j].size()>0)
						for (int k = 0; k < world[i][j].size(); ++k)
							cout << world[i][j][k]->print() ;
					else
						cout << "0 ";
				}
				cout << endl;
			}

		}

		int getRandomNumber(int min, int max){
			mt19937 rng;
		    rng.seed(random_device()());
		    uniform_int_distribution<mt19937::result_type> dist6(min,max); // distribution in range [1, 6]

		    //cout << dist6(rng) << std::endl;
		    return (int) dist6(rng);
		}

};