#include "QBchain.h"
#include "QBcube.h"
#include <iostream>

/* 
/
--> Simple Test-QBcubo <--
/
*/


/*
int main(){

	// Define an element type QBcube class

        QBcube c(3);

	// Setting Communication config.

	c.setPort("/dev/ttyUSB0");

	// Open the communication channel

        c.open();

	// Define temporary variables 

        short int* meas;

        while(true){

		// Get measurements of Pos1/Pos2/PosL

                meas = c.getMeas();

                std::cout <<"P1: " << meas[0] <<" P2: " << meas[1]<< " L: "<< meas[2]<< std::endl;

	std::cout << "St.ness: " << c.getStiff() << " Angle: " << c.getAngle() << std::endl;
        }

	// Close Communication

	c.close();

        return 0;
}
*/



/* 
/
--> Simple Test-QBchain
/
*/

int main(){

	// Define an element vector for ID of cubes

	std::vector<int> a;

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
	Without this sequence cubes dosen't start and communication fall down.
	/
	*/

	// Open the communication channel

	p.open();

	// Add Cubes to the chain

	p.addCubes(a);

	double* meas;

	while(true){

		// Print angles

		p.readIt();

		// Get angles of each cubes in the chain

		meas = p.readAng();

		std::cout << *meas;

	}

	// Close Communication

	p.close();

	return 0;
}


