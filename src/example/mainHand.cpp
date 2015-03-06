#include <iostream>
#include "QBHand.h"
#include <csignal>
#include <cstdlib>

using std::cerr;
using std::cout;
using std::endl;

QBHand qb(10);

void comm_handler (int sig)
{
        std::cout << "Force Close" << std::endl;


        exit(sig);
}


/*
/ Simple example for QBcube object
*/

int main(){

 	//qb.setPort("/dev/ttyUSB0");
	qb.open("/dev/ttyUSB0");
	qb.activate();

        // Add signal
        signal(SIGINT, comm_handler);

	double pos, meas;

	while(true){
		if (qb.getAngle(&meas))
			std::cout << "Read Position: " << meas << std::endl;

			
		std::cout << "Position" << std::endl;
		std::cin >> pos;

		if (!qb.setPosPerc(pos))
			std::cout << "Errore nella position" << std::endl;
	}
	
	return 0;
}
