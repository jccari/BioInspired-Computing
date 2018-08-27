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
		float probability; //[0.0 to 1.0]

	public:
		GridWorld(int n){
			size = n;
			t_grass = 5;
			t_rabbit = 5;
			t_wolf = 5;

			v_rabbit = 3;
			v_wolf = 3;

			probability = 70;

			inicializeWorld();
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

			Grass* g = new Grass(1,1);
			saveActor(g,1,1, IS_GRASS );
			saveActor(new Rabbit(2,2),2,2, IS_RABBIT );
			saveActor(new Wolf(3,3),3,3, IS_WOLF );

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
			int totalRabbits = rabbitList.size();
			int totalWolves = wolfList.size();
			for (int i = 0; i < iterations; ++i){
				for (int i = 0; i < totalRabbits; ++i){
					if( rabbitList[i]->iterationsWithoutEating(v_rabbit)){
						removeActor(rabbitList[i], rabbitList[i]->getX(), rabbitList[i]->getY());
						totalRabbits--;
					}
					else 
						randomBehaviour(rabbitList[i]);
				}
				for (int i = 0; i < totalWolves; ++i){
					if( wolfList[i]->iterationsWithoutEating(v_rabbit)){
						removeActor(wolfList[i], wolfList[i]->getX(), wolfList[i]->getY());
						totalWolves--;
					}
					else 
						randomBehaviour(wolfList[i]);
				}
				
				if( i%t_grass==0)
					breadActors(grassList[ i%grassList.size() ]);

				if( i%t_rabbit==0)
					breadActors(rabbitList[ i%rabbitList.size() ]);
				
				if( i%t_wolf==0)
					breadActors(wolfList[ i%wolfList.size() ]);
				
				print();
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
						}
					}else{
						if ( actor->getType() == IS_RABBIT ){
							//cout << "[DEBUG] I have been recogniced as a Rabbit" << endl;
							Grass* target = findGrass(actor->getX(), actor->getY() );
							if( target!= nullptr){
								actor->eat(target);
								removeActor(target, target->getX(), target->getY());
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
			int totalRabbits = rabbitList.size();
			int totalWolves = wolfList.size();
			for (int i = 0; i < iterations; ++i){
				for (int i = 0; i < totalRabbits; ++i){
					if( rabbitList[i]->iterationsWithoutEating(v_rabbit)){
						removeActor(rabbitList[i], rabbitList[i]->getX(), rabbitList[i]->getY());
						totalRabbits--;
					}
					else 
						probabilisticBehaviour(rabbitList[i]);
				}
				for (int i = 0; i < totalWolves; ++i){
					if( wolfList[i]->iterationsWithoutEating(v_rabbit)){
						removeActor(wolfList[i], wolfList[i]->getX(), wolfList[i]->getY());
						totalWolves--;
					}
					else 
						probabilisticBehaviour(wolfList[i]);
				}
				
				if( i%t_grass==0)
					breadActors(grassList[ i%grassList.size() ]);

				if( i%t_rabbit==0)
					breadActors(rabbitList[ i%rabbitList.size() ]);
				
				if( i%t_wolf==0)
					breadActors(wolfList[ i%wolfList.size() ]);
				
				print();
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

			if( probability_gen < this->probability){
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

			//cout <<"[DEBUG] action value: "<< action << endl;
			switch(action){
				case ACTION_EAT : {
					//cout << " ACTION_EAT "<< actor->getType() << endl;
					if ( actor->getType() == IS_WOLF ){
						Rabbit* target = findRabbit(actor->getX(), actor->getY() );
						if( target!= nullptr){
							actor->eat(target);
							removeActor(target, target->getX(), target->getY());
						}
					}else{
						if ( actor->getType() == IS_RABBIT ){
							//cout << "[DEBUG] I have been recogniced as a Rabbit" << endl;
							Grass* target = findGrass(actor->getX(), actor->getY() );
							if( target!= nullptr){
								actor->eat(target);
								removeActor(target, target->getX(), target->getY());
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
				}
				if(actor->getType()==IS_RABBIT){
					new_actor = new Rabbit( x_possible[i], y_possible[i]);
					saveActor( new_actor, x_possible[i], y_possible[i], IS_RABBIT);
				}
				if(actor->getType()==IS_WOLF){
					new_actor = new Wolf( x_possible[i], y_possible[i]);
					saveActor( new_actor, x_possible[i], y_possible[i], IS_WOLF);
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
			cout <<"\n ----------------------------------------------------------- \n";
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
		/*
		void findGrassAround(int x, int y, vector<Actor*>& aux_grass){
			Grass* grass = nullptr;
			if(x-1 >=0 and x-1 <size){
				grass = findGrass(x-1,y);
				if(grass!=nullptr)
					aux_grass.push_back( grass );
			}
			if( y+1 >=0 and y+1 <size )	{
				grass = findGrass(x,y+1);
				if(grass!=nullptr)
					aux_grass.push_back( grass );
			}
			if( x+1 >=0 and x+1 <size )	{
				grass = findGrass(x+1,y);
				if(grass!=nullptr)
					aux_grass.push_back( grass );
			}
			if( y-1 >=0 and y-1 <size ){
				grass = findGrass(x,y-1);
				if(grass!=nullptr)
					aux_grass.push_back( grass );
			}
		}

		void findRabbitAround(int x, int y, vector<Actor*>& aux_rabbit){
			Rabbit* rabbit = nullptr;
			if(x-1 >=0 and x-1 <size){
				rabbit = findRabbit(x-1,y);
				if(rabbit!=nullptr)
					aux_rabbit.push_back( rabbit );
			}
			if( y+1 >=0 and y+1 <size )	{
				rabbit = findRabbit(x,y+1);
				if(rabbit!=nullptr)
					aux_rabbit.push_back( rabbit );
			}
			if( x+1 >=0 and x+1 <size )	{
				rabbit = findRabbit(x+1,y);
				if(rabbit!=nullptr)
					aux_rabbit.push_back( rabbit );
			}
			if( y-1 >=0 and y-1 <size ){
				rabbit = findRabbit(x,y-1);
				if(rabbit!=nullptr)
					aux_rabbit.push_back( rabbit );
			}
		}
		*/
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
					for (int i = 0; i < grassList.size(); ++i){
						if(grassList[i] == (Grass*) actor)
							grassList.erase(grassList.begin() + i);
					}
				}
				case IS_RABBIT: {
					for (int i = 0; i < rabbitList.size(); ++i){
						if(rabbitList[i] == (Rabbit*) actor)
							rabbitList.erase( rabbitList.begin() + i);
					}
				}
				case IS_WOLF: {
					for (int i = 0; i < wolfList.size(); ++i){
						if(wolfList[i] == (Wolf*) actor)
							wolfList.erase(wolfList.begin() + i);
					}
				}
			}
		}

		int min(int a, int b){
			return a <= b? a : b;
		}
};