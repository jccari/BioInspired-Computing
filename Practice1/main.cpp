#include <iostream>

#include "GridWorld.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
	GridWorld* world = new GridWorld(10);
	world->print();
	

	return 0;
}