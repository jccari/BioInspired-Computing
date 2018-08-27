#include <iostream>
using namespace std;

#define IS_GRASS 0
#define IS_RABBIT 1
#define IS_WOLF 2

class Actor{
	protected:
		int xpos, ypos;
		int type;
		int iter_lazy; // iterations without eat someone // only for rabbits and wolves
	public:
		Actor(int x, int y, int _type){
			xpos = x;
			ypos = y;
			type = _type;
		}

		int getX(){ return xpos; }
		int getY(){ return ypos; }

		bool iterationsWithoutEating(int max_iter_without_eating){
			iter_lazy += 1;
			if(iter_lazy> max_iter_without_eating){
				iter_lazy = 0;
				return true;
			}
			return false;
		}

		int getType(){ return type; }
		
		virtual void eat(Actor* target){
			target->dead();
		}

		virtual void dead() = 0;
		virtual string print() = 0;

		virtual Actor* clone()= 0;

		virtual void doNothing(){}

		//TODO: correct this validations
		virtual void moveUp(int maxsize){
			if ( ypos+1 < maxsize)
				ypos += 1 ;
		}

		virtual void moveDown(int maxsize){
			if ( ypos-1 >= 0)
				ypos -= 1 ;	
		}

		virtual void moveRight(int maxsize){
			if ( xpos+1 < maxsize)
				xpos += 1 ;
		}

		virtual void moveLeft(int maxsize){
			if ( xpos-1 >= 0)
				xpos -= 1 ;	
		}
};

class Grass : public Actor{
	private:
		
	public:
		Grass(int x, int y) : Actor(x,y, IS_GRASS ){}

		void dead(){
			cout << "[GRASS] He muerto" << endl;
		}

		Actor* clone(){
			return new Grass(xpos, ypos);
		}

		void moveUp(int maxsize){}

		void moveDown(int maxsize){}

		void moveRight(int maxsize){}

		void moveLeft(int maxsize){}

		string print(){
			return "G";
		}
};

class Rabbit: public Actor{
	private:

	public:
		Rabbit(int x, int y) : Actor(x,y, IS_RABBIT){}

		//void eat(Grass* target){}

		void dead(){
			cout << "[RABBIT] He muerto" << endl;
		}

		Actor* clone(){
			return new Rabbit(xpos, ypos);
		}

		string print(){
			return "R";
		}
};

class Wolf : public Actor{
	private:

	public:
		Wolf(int x, int y) : Actor(x,y, IS_WOLF){}

		//void eat(Rabbit* target){}

		void dead(){
			cout << "[WOLF] He muerto" << endl;
		}

		Actor* clone(){
			return new Wolf(xpos, ypos);
		}

		string print(){
			return "W";
		}
};

/*
std::ostream& operator<< (std::ostream &out, Actor const& actor) {
    out << actor.print() << endl;
    return out;
}
*/