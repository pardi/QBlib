#include "QBchain.h"
#include "QBcube.h"
#include <iostream>


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

	a.push_back(3);

	// Define an element type QBchain class

	QBchain p; 
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

	p.open();

	// Add Cubes to the chain

	p.addCubes(a);

	double meas, err, K = 0.1, ref;

	p.readAng(&meas, NULL, "deg");

	std::cout << "Set Ref angle" << std::endl;
	std::cin >> ref;

	err = ref - meas;

	while((err > 0.01) && (err < -0.01 )){

		err *= K;
	
		refPos.push_back(err);
		refPres.push_back(0);

	        p.moveIt(refPos, refPres, "deg");

		refPos.clear();
		refPres.clear();

		p.readAng(&meas, NULL, "deg");
		err = ref - meas;
	}


	// Close Communication

	p.close();


	return 0;
}


