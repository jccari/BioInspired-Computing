#include <iostream>
#include <vector>
#include <random>

#include "Actors.cpp"

//#define ACTOR_DIE 0

#define ACTION_EAT 1
#define ACTION_MOVE_UP 2
#define ACTION_MOVE_DOWN 3
#define ACTION_MOVE_RIGHT 4
#define ACTION_MOVE_LEFT 5
#define ACTION_DO_NOTHING 6
#define ACTION_BREED 7

#define IS_GRASS 0
#define IS_RABBIT 1
#define IS_WOLF 2

using namespace std;

class GridWorld{
	private:
		int size;

		int t_grass, t_rabbit, t_wolf; // breed after this parameters
		int v_rabbit, v_wolf; // die after this parameters

		vector< vector< vector<Actor*> > > world; // grid world

		// auxiliary lists
		vector< Grass*> grassList;
		vector< Rabbit*> rabbitList;
		vector< Wolf*> wolfList;

		//auxiliary variable for probabilistic world
		float probability; //[0 to 100] %

		// variables for stats
		int deadRabbits;
		int deadWolves;
		int deadGrasses;
		int totalRabbits;
		int totalWolves;
		int totalGrasses;

	public:
		GridWorld(int n){
			size = n;
			t_grass = 3; 	// este parametro es cambiado en main.cpp
			t_rabbit = 3;	// este parametro es cambiado en main.cpp
			t_wolf = 3;	// este parametro es cambiado en main.cpp

			v_rabbit = 5;	// este parametro es cambiado en main.cpp
			v_wolf = 5;	// este parametro es cambiado en main.cpp
	
			probability = 70; // este parametro es cambiado en main.cpp

			//inicialize variables for stats
			deadGrasses =  0;
			deadWolves = 0;
			deadRabbits = 0;
			totalGrasses = 1; //this will be created in inicializeWorld
			totalRabbits = 1; //this will be created in inicializeWorld
			totalWolves = 1; //this will be created in inicializeWorld

			inicializeWorld();
		}

		void setProbability(int prob){
			this->probability = prob;
		}

		void setBirthRate(int rate){
			t_grass = rate;
			t_rabbit = rate;
			t_wolf = rate;
		}

		void setMortalityRate(int rate){
			v_rabbit = rate;
			v_wolf = rate;
		}

		void saveActor(Actor* actor,int x, int y, int type){
			world[x][y].push_back(actor);

			switch(type){
				case IS_GRASS:	grassList.push_back((Grass*) actor); break;
				case IS_RABBIT:	rabbitList.push_back((Rabbit*) actor); break;
				case IS_WOLF:	wolfList.push_back((Wolf*) actor); break;
			}
			//cout << "[DEBUG] Saving new actor (" <<x << "," << y<< ")"<<endl;
		}

		void inicializeWorld(){
			world.resize(size);
			for (int i = 0; i < size; ++i){
				world[i].resize(size);
			}

			//TODO: iniciliaze in random positions
			/*
			Grass* g = new Grass(1,1);
			saveActor(g,1,1, IS_GRASS );
			saveActor(new Rabbit(2,2),2,2, IS_RABBIT );
			saveActor(new Wolf(3,3),3,3, IS_WOLF );
			*/
			Grass* g = new Grass(getRandomNumber(0,size),getRandomNumber(0,size));
			Rabbit* r = new Rabbit(getRandomNumber(0,size),getRandomNumber(0,size));
			Wolf* w = new Wolf(getRandomNumber(0,size),getRandomNumber(0,size));
			
			saveActor(g, g->getX(), g->getY(), IS_GRASS );
			saveActor(r, r->getX(), r->getY(), IS_RABBIT );
			saveActor(w, w->getX(), w->getY(), IS_WOLF );
		}

		void testFunction(){
			Grass* g = new Grass(8,8);
			saveActor(g,1,1, IS_GRASS );	
			//saveActor(new Rabbit(1,1),1,1, IS_RABBIT );
			saveActor(new Wolf(1,1),1,1, IS_WOLF);
			
			vector<int> x_possible;
			vector<int> y_possible;
			findPossiblePositionForChildren(g, x_possible, y_possible);

			for (int i = 0; i < x_possible.size(); ++i){
				cout << i << ": "<<x_possible[i] << " " << y_possible[i] << endl;
			}
		}

		void randomWorld(int iterations){
			
			for (int i = 0; i < iterations; ++i){
				for (int r = 0; r < totalRabbits; ++r){
					if( rabbitList[r]->iterationsWithoutEating(v_rabbit)){
						removeActor(rabbitList[r], rabbitList[r]->getX(), rabbitList[r]->getY());
						totalRabbits--;
						deadRabbits++; // [STATS]
					}
					else 
						randomBehaviour(rabbitList[r]);
				}
				for (int w = 0; w < totalWolves; ++w){
					if( wolfList[w]->iterationsWithoutEating(v_wolf)){
						removeActor(wolfList[w], wolfList[w]->getX(), wolfList[w]->getY());
						totalWolves--;
						deadWolves++; // [STATS]
					}
					else 
						randomBehaviour(wolfList[w]);
				}
				
				if( i%t_grass==0)
					breadActors(grassList[ i%grassList.size() ]);

				if( i%t_rabbit==0)
					breadActors(rabbitList[ i%rabbitList.size() ]);
				
				if( i%t_wolf==0)
					breadActors(wolfList[ i%wolfList.size() ]);
				
				//cout << "---------------------- ITERATION"<< i+1 << "----------------------"<<endl;
				//print();
			}
		}

		void randomBehaviour(Actor* actor){
		   
		    int action = getRandomNumber(1,6);

			//cout <<"[DEBUG] action value: "<< action << endl;
			switch(action){
				case ACTION_EAT : {
					//cout << " ACTION_EAT "<< actor->getType() << endl;
					if ( actor->getType() == IS_WOLF ){
						Rabbit* target = findRabbit(actor->getX(), actor->getY() );
						if( target!= nullptr){
							actor->eat(target);
							removeActor(target, target->getX(), target->getY());
							totalRabbits--;
							deadRabbits++; // [STATS]
						}
					}else{
						if ( actor->getType() == IS_RABBIT ){
							//cout << "[DEBUG] I have been recogniced as a Rabbit" << endl;
							Grass* target = findGrass(actor->getX(), actor->getY() );
							if( target!= nullptr){
								actor->eat(target);
								removeActor(target, target->getX(), target->getY());
								totalGrasses--;
								deadGrasses++; // [STATS]
							}
						}
					}
					break;
				}
				case ACTION_MOVE_UP :
					actor->moveUp(size);
					break;
				case ACTION_MOVE_DOWN :
					actor->moveDown(size);
					break;
				case ACTION_MOVE_RIGHT :
					actor->moveRight(size);
					break;
				case ACTION_MOVE_LEFT :
					actor->moveLeft(size);
					break;
				//case ACTION_BREED : { break; }
				case ACTION_DO_NOTHING :
					actor->doNothing();
					break;
			}
		}

		void probabilisticWorld(int iterations){
			
			for (int i = 0; i < iterations; ++i){
				//cout << "[DEBUG] countRabbits: " << totalRabbits << endl;
				for (int r = 0; r < totalRabbits; ++r){
					if( rabbitList[r]->iterationsWithoutEating(v_rabbit)){
						removeActor(rabbitList[r], rabbitList[r]->getX(), rabbitList[r]->getY());
						totalRabbits--;
						deadRabbits++; // [STATS]
						//cout << "[DEBUG] RABBIT: Eliminado por no comer" << endl;
					}
					else{ 
						//cout << "[DEBUG] RABBIT: REALIZAR ALGO PROBABILISTICO" << endl;
						probabilisticBehaviour(rabbitList[r]);
					}
				}
				for (int w = 0; w < totalWolves; ++w){
					if( wolfList[w]->iterationsWithoutEating(v_wolf)){
						removeActor(wolfList[w], wolfList[w]->getX(), wolfList[w]->getY());
						totalWolves--;
						deadWolves++; // [STATS]
						//cout << "[DEBUG] LOBO: Eliminado por no comer" << endl;
					}
					else {
						probabilisticBehaviour(wolfList[w]);
					}
				}
				
				if( i%t_grass==0) 
					breadActors(grassList[ i%grassList.size() ]);

				if( i%t_rabbit==0)
					breadActors(rabbitList[ i%rabbitList.size() ]);
				
				if( i%t_wolf==0)
					breadActors(wolfList[ i%wolfList.size() ]);
				
				//cout << "---------------------- ITERATION"<< i+1 << "----------------------"<<endl;
				//print();
			}
		}

		void probabilisticBehaviour(Actor* actor){
			int action;
			int probability_gen = getRandomNumber(0,100);

			vector<int> possibleDirsTargets; 

			switch(actor->getType()){
				case IS_RABBIT:	findGrassAround( actor->getX(), actor->getY(), possibleDirsTargets); break;
				case IS_WOLF:	findRabbitAround( actor->getX(), actor->getY(), possibleDirsTargets); break;
			}

			if( probability_gen <= this->probability){
				switch(getRandomNumber(2,5)){ //switch directions
					case ACTION_MOVE_UP : 		actor->moveUp(size); break;
					case ACTION_MOVE_DOWN :		actor->moveDown(size); break;
					case ACTION_MOVE_RIGHT :	actor->moveRight(size); break;
					case ACTION_MOVE_LEFT :		actor->moveLeft(size); break;
				}
				action = ACTION_EAT;
			}
			else
				action = getRandomNumber(1,6);

//			cout <<"[DEBUG] action value: "<< action << "	number: "<< probability_gen << endl; 
			switch(action){
				case ACTION_EAT : {
					//cout << " ACTION_EAT "<< actor->getType() << endl;
					if ( actor->getType() == IS_WOLF ){
						Rabbit* target = findRabbit(actor->getX(), actor->getY() );
						if( target!= nullptr){
							actor->eat(target);
							removeActor(target, target->getX(), target->getY());
							totalRabbits--;
							deadRabbits++; // [STATS]
						}
					}else{
						if ( actor->getType() == IS_RABBIT ){
							Grass* target = findGrass(actor->getX(), actor->getY() );
							if( target!= nullptr){
								actor->eat(target);
								removeActor(target, target->getX(), target->getY());
								totalGrasses--;
								deadGrasses++; // [STATS]
							}
						}
					}
					break;
				}
				case ACTION_MOVE_UP :
					actor->moveUp(size);
					break;
				case ACTION_MOVE_DOWN :
					actor->moveDown(size);
					break;
				case ACTION_MOVE_RIGHT :
					actor->moveRight(size);
					break;
				case ACTION_MOVE_LEFT :
					actor->moveLeft(size);
					break;
				//case ACTION_BREED : { break; }
				case ACTION_DO_NOTHING :
					actor->doNothing();
					break;
			}
		}

		void breadActors(Actor* actor){
			vector<int> x_possible;
			vector<int> y_possible;

			findPossiblePositionForChildren( actor, x_possible, y_possible);

			int number = getRandomNumber(1,4); // C rabbits or Z wolves that will be created in a grid (N,W,S,E)

			number = min( x_possible.size(), number);
			//cout << "[DEBUG] Number of Children: " << number << endl;
			for (int i = 0; i < number; ++i){
				Actor* new_actor;
				if(actor->getType()==IS_GRASS){
					new_actor = new Grass( x_possible[i], y_possible[i]);
					saveActor( new_actor, x_possible[i], y_possible[i], IS_GRASS);
					totalGrasses++; //[STATS]
				}
				if(actor->getType()==IS_RABBIT){
					//cout << "[DEBUG] Number of Children: " << number << endl;
					new_actor = new Rabbit( x_possible[i], y_possible[i]);
					saveActor( new_actor, x_possible[i], y_possible[i], IS_RABBIT);
					totalRabbits++; //[STATS]
				}
				if(actor->getType()==IS_WOLF){
					new_actor = new Wolf( x_possible[i], y_possible[i]);
					saveActor( new_actor, x_possible[i], y_possible[i], IS_WOLF);
					totalWolves++; //[STATS]
				}

			}			
		}

		void findPossiblePositionForChildren(Actor* actor, vector<int>& x_pos, vector<int>& y_pos){
			if(actor->getX()-1 >=0 and actor->getX()-1 <size){
				x_pos.push_back( actor->getX()-1 );
				y_pos.push_back( actor->getY() );
			}
			if( actor->getY()+1 >=0 and actor->getY()+1 <size )	{
				x_pos.push_back( actor->getX() );
				y_pos.push_back( actor->getY()+1 );
			}
			if( actor->getX()+1 >=0 and actor->getX()+1 <size )	{
				x_pos.push_back( actor->getX()+1 );
				y_pos.push_back( actor->getY() );
			}
			if( actor->getY()-1 >=0 and actor->getY()-1 <size ){
				x_pos.push_back( actor->getX() );
				y_pos.push_back( actor->getY()-1 );
			}
		}

		void print(){
			//cout <<"\n ----------------------------------------------------------- \n";
			for (int i = 0; i < size; ++i){
				for (int j = 0; j < size; ++j){
					if(world[i][j].size()>0){
						for (int k = 0; k < world[i][j].size(); ++k)
							cout << world[i][j][k]->print() ;
						cout << " ";
					}
					else
						cout << "* ";
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

		Rabbit* findRabbit(int x, int y){
			Rabbit* target = nullptr;
			for (int i = 0; i < world[x][y].size(); ++i){
				if(world[x][y][i]->getType() == IS_RABBIT )
					return (Rabbit*) world[x][y][i];
			}
			return target;
		}

		Grass* findGrass(int x, int y){
			Grass* target = nullptr;
			for (int i = 0; i < world[x][y].size(); ++i){
				if(world[x][y][i]->getType() == IS_GRASS )
					return (Grass*) world[x][y][i];
			}
			return target;
		}
	
		void findGrassAround(int x, int y, vector<int>& aux_grass){
			Grass* grass = nullptr;
			if(x-1 >=0 and x-1 <size){
				grass = findGrass(x-1,y);
				if(grass!=nullptr)
					aux_grass.push_back( ACTION_MOVE_LEFT );
			}
			if( y+1 >=0 and y+1 <size )	{
				grass = findGrass(x,y+1);
				if(grass!=nullptr)
					aux_grass.push_back( ACTION_MOVE_UP );
			}
			if( x+1 >=0 and x+1 <size )	{
				grass = findGrass(x+1,y);
				if(grass!=nullptr)
					aux_grass.push_back( ACTION_MOVE_RIGHT );
			}
			if( y-1 >=0 and y-1 <size ){
				grass = findGrass(x,y-1);
				if(grass!=nullptr)
					aux_grass.push_back( ACTION_MOVE_DOWN );
			}
		}

		void findRabbitAround(int x, int y, vector<int>& aux_rabbit){
			Rabbit* rabbit = nullptr;
			if(x-1 >=0 and x-1 <size){
				rabbit = findRabbit(x-1,y);
				if(rabbit!=nullptr)
					aux_rabbit.push_back( ACTION_MOVE_LEFT );
			}
			if( y+1 >=0 and y+1 <size )	{
				rabbit = findRabbit(x,y+1);
				if(rabbit!=nullptr)
					aux_rabbit.push_back( ACTION_MOVE_UP );
			}
			if( x+1 >=0 and x+1 <size )	{
				rabbit = findRabbit(x+1,y);
				if(rabbit!=nullptr)
					aux_rabbit.push_back( ACTION_MOVE_RIGHT );
			}
			if( y-1 >=0 and y-1 <size ){
				rabbit = findRabbit(x,y-1);
				if(rabbit!=nullptr)
					aux_rabbit.push_back( ACTION_MOVE_DOWN );
			}
		}

		void removeActor(Actor* actor, int x, int y){
			for (int i = 0; i < world[x][y].size(); ++i){
				if( world[x][y][i] == actor)
					world[x][y].erase(world[x][y].begin() + i);
			}

			switch(actor->getType()){
				case IS_GRASS: {
					for (int g = 0; g < grassList.size(); ++g){
						if(grassList[g] == (Grass*) actor)
							grassList.erase(grassList.begin() + g);
					}
				}
				case IS_RABBIT: {
					for (int r = 0; r < rabbitList.size(); ++r){
						if(rabbitList[r] == (Rabbit*) actor)
							rabbitList.erase( rabbitList.begin() + r);
					}
				}
				case IS_WOLF: {
					for (int w = 0; w < wolfList.size(); ++w){
						if(wolfList[w] == (Wolf*) actor)
							wolfList.erase(wolfList.begin() + w);
					}
				}
			}
		}

		int min(int a, int b){
			return a <= b? a : b;
		}

		void printAux(){
			cout << "**********************************************" << endl;
			cout << "grasses: " << grassList.size() << endl;
			cout << "rabbits: " << rabbitList.size() << endl;
			cout << "wolves : " << wolfList.size() << endl;
			cout << "**********************************************" << endl;
		}

		void getStats(vector< vector<float> >& results){
			vector<float> grassStats;
			vector<float> rabbitStats;
			vector<float> wolfStats;

			grassStats.push_back(abs(totalGrasses) + abs(deadGrasses));
			grassStats.push_back((float) abs(totalGrasses)*100.0f /(abs(totalGrasses) + abs(deadGrasses)) );
			grassStats.push_back((float) abs(deadGrasses)*100.0f /(abs(totalGrasses) + abs(deadGrasses)) );

			rabbitStats.push_back(abs(totalRabbits) + abs(deadRabbits));
			rabbitStats.push_back((float) abs(totalRabbits)*100.0f/(abs(totalRabbits) + abs(deadRabbits)) );
			rabbitStats.push_back((float) abs(deadRabbits)*100.0f/(abs(totalRabbits) + abs(deadRabbits)) );
			
			wolfStats.push_back(abs(totalWolves) + abs(deadWolves));
			wolfStats.push_back((float) abs(totalWolves)*100.0f/(abs(totalWolves) + abs(deadWolves)) );
			wolfStats.push_back((float) abs(deadWolves)*100.0f/(abs(totalWolves) + abs(deadWolves)) );

			results.push_back(grassStats);
			results.push_back(rabbitStats);
			results.push_back(wolfStats);
		}

		void printStats(){

			cout << "\t--------------- Stats ---------------"<< endl;

			cout << "* PASTOS:"<< endl;
			cout << "	total : 	" << abs(totalGrasses) + abs(deadGrasses)<< endl;
			cout << "	alive : 	"<< abs(totalGrasses) << " " << (float) totalGrasses*100 /(totalGrasses + deadGrasses) << endl;
			cout << "	dead: 		" << abs(deadGrasses) << " "<< (float) deadGrasses*100 /(totalGrasses + deadGrasses) <<endl;

			cout << "* CONEJOS:"<< endl;
			cout << "	total : 	" << abs(totalRabbits) + abs(deadRabbits) << endl;
			cout << "	alive : 	"<< abs(totalRabbits) << endl;
			cout << "	dead: 		" << abs(deadRabbits) << endl;

			cout << "* LOBOS:"<< endl;
			cout << "	total : 	" << abs(totalWolves) + abs(deadWolves) << endl;
			cout << "	alive : 	"<< abs(totalWolves) << endl;
			cout << "	dead: 		" << abs(deadWolves) << endl;

		}
};