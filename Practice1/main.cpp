#include <iostream>

#include "GridWorld.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
	GridWorld* world = new GridWorld(10);
	
	world->print();

	world->randomWorld(10);
	//world->testFunction();


	return 0;
}