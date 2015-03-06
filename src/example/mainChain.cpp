#include "QBchain.h"
#include "QBcube.h"
#include <iostream>
#include <csignal>
#include <cstdlib>

	// Define an element type QBchain class
	QBchain p; 

void comm_handler (int sig)
{
	std::cout << "Force Close" << std::endl;

	if(!p.close())
		std::cout << "Problema nella close" << std::endl;

	exit(sig);
}



/* 
/
--> Simple Test-QBchain
/
*/

int main(){

	// Define an element vector for ID of cubes

	std::vector<int> a;
	std::vector<double> refPos, refPres;

	// Push IDs 

	a.push_back(9);


	// Setting Communication config.

	p.setPort("/dev/ttyUSB0");

	/*
	/
	ATTENTION!
	Put function open() before function addcubes() to make program work fine.
	Without this sequence cubes doesn't start and communication fall down.
	/
	*/

	// Open the communication channel

	if(!p.open())
		std::cout << "un fa la porta" << std::endl;

	// Add Cubes to the chain

	p.addHands(a);

	double meas, mhand, pos, pre;

	// Add signal
	signal(SIGINT, comm_handler);

	for(int i = 0; i < 2; i++ ){
	        p.readAng(&meas, &mhand, "deg");

		std::cout << "Actual Position "  <<  mhand << std::endl;

		std::cout << "Position" << std::endl;
		std::cin >> pos;
//		std::cout << "Preset" << std::endl;
//		std::cin >> pre;

		refPos.push_back(pos);
//		refPres.push_back(pre);

	        //p.moveIt(refPos, refPres, "deg");
		p.closeIt(refPos);

		refPos.clear();
//		refPres.clear();
	}	

	std:: cin >> meas;
	// Close Communication

	p.close();


	return 0;
}


