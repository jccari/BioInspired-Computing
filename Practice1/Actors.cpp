#include <iostream>
using namespace std;

class Actor{
	protected:
		int xpos, ypos;
		string name;
	public:
		Actor(int x, int y){
			xpos = x;
			ypos = y;
		}
		virtual void dead() = 0;
		virtual string print() = 0;
};

class Grass : public Actor{
	private:
		
	public:
		Grass(int x, int y) : Actor(x,y){}

		void dead(){
			cout << "[GRASS] He muerto" << endl;
		}

		string print(){
			return "G";
		}
};

class Rabbit: public Actor{
	private:

	public:
		Rabbit(int x, int y) : Actor(x,y){}

		void eat(Grass* target){

		}

		void doNothing(){}

		void dead(){
			cout << "[RABBIT] He muerto" << endl;
		}

		void moveUp(int maxsize){
			if ( ypos+1 < maxsize)
				ypos += 1 ;
		}

		void moveDown(int maxsize){
			if ( abs(ypos-1) < maxsize)
				ypos -= 1 ;	
		}

		void moveRight(int maxsize){
			if ( xpos+1 < maxsize)
				xpos += 1 ;
		}

		void moveLeft(int maxsize){
			if ( abs(xpos-1) < maxsize)
				xpos -= 1 ;	
		}

		string print(){
			return "R";
		}
};

class Wolf : public Actor{
	private:

	public:
		Wolf(int x, int y) : Actor(x,y){}

		void eat(Rabbit* target){

		}

		void doNothing(){}

		void dead(){
			cout << "[WOLF] He muerto" << endl;
		}

		void moveUp(int maxsize){
			if ( ypos+1 < maxsize)
				ypos += 1 ;
		}

		void moveDown(int maxsize){
			if ( abs(ypos-1) < maxsize)
				ypos -= 1 ;	
		}

		void moveRight(int maxsize){
			if ( xpos+1 < maxsize)
				xpos += 1 ;
		}

		void moveLeft(int maxsize){
			if ( abs(xpos-1) < maxsize)
				xpos -= 1 ;	
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