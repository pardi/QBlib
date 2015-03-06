#include <iostream>
#include "QBcube.h"
#include <csignal>
#include <cstdlib>


QBcube qb(1);

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

	double pre, pos, meas;

	while(true){
		if (qb.getAngle(&meas, "deg"))
			std::cout << "Read Position: " << meas << std::endl;

		if (qb.getPreset(&meas))
			std::cout << "Read Preset: " << meas << std::endl;
		
		std::cout << "Position" << std::endl;
		std::cin >> pos;
		std::cout << "Preset" << std::endl;
		std::cin >> pre;

		if (!qb.setPosAndPreset(pos, pre, "deg"))
			std::cout << "Errore nella position" << std::endl;
	}
	
	return 0;
}
