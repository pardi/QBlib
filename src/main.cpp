#include "QBchain.h"
#include "QBcube.h"
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <ctime>



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

	a.push_back(1);
	a.push_back(2);
	a.push_back(3);

	// Setting Communication config.

	/*
	/
	ATTENTION!
	Put function open() before function addcubes() to make program work fine.
	Without this sequence cubes doesn't start and communication fall down.
	/
	*/

	// Open the communication channel

	if (!p.open("/dev/ttyUSB0"))
		std::cout << "Un' fa' la connessione" << std::endl;

	// Add Cubes to the chain

	p.addCubes(a);
	double ref, ref2, ref3;
	double meas[3];


	double Kp1 = 0, Kp2 = 0, Kp3 = 0.3, Kd1 = 0, Kd2 = 0, Kd3 = 0, Ki1 = 0, Ki2 = 0, Ki3 = 0.5 ;
	double errP = 0, errD = 0, errI = 0;
	double errP2 = 0, errD2 = 0, errI2 = 0;
	double errP3 = 0, errD3 = 0, errI3 = 0;
	double tauP = 0, tauD = 0, tauI = 0;
	double tauP2 = 0, tauD2 = 0, tauI2 = 0;
	double tauP3 = 0, tauD3 = 0, tauI3 = 0;
	double tau;
	double tau2;
	double tau3;

		// Add signal
	signal(SIGINT, comm_handler);
	p.activate();

	std::clock_t stepK1, stepK0;
	double interval;


while(true){
	std::cout << "Set P" << std::endl;
	std::cin >> Kp1;
    std::cout << "Set D" << std::endl;
    std::cin >> Kd1;
    std::cout << "Set I" << std::endl;
    std::cin >> Ki1;


    ref = 0;
    ref2 = 0;
    ref3 = 0;


	//errD = ADSASD;
	//errI = asdasd;
	stepK0 = std::clock();
	stepK1 = stepK0;

	for (int i = 0; i < 5000; i++){

		// Intervallo di tempo

		interval = (stepK1 - stepK0)/ (double)(CLOCKS_PER_SEC);

		// -----

		stepK0 = std::clock();

	  	while(!p.readAng(meas, NULL, "deg"));
    	
        // position error
        errP = ref - meas[0];
		errP2 = ref2 - meas[1];
		errP3 = ref3 - meas[2];

		// Proporzionale
		tauP = errP * Kp1;
		tauP2 = errP2 * Kp2;
		tauP3 = errP3 * Kp3;

		// Derivativo

		tauD = (errP/interval)* Kd1;
		tauD2 = (errP/interval)* Kd2;
		tauD3 = (errP/interval)* Kd3;

		// Integrativo

		tauI += (errP)*interval*Ki1;
		tauI2 += (errP2)*interval*Ki2;
		tauI3 += (errP3)*interval*Ki3;

		// ------

		tau = tauP + tauD + tauI;

		tau2 = tauP2 + tauD2 + tauI2;

		tau3 = tauP3 + tauD3 + tauI3;
	
		// ------

		refPos.push_back(ref + tau);
		refPos.push_back(ref2 + tau2);
		refPos.push_back(ref3 + tau3);
		refPres.push_back(0);
		refPres.push_back(0);
		refPres.push_back(0);

	    p.moveIt(refPos, refPres, "deg");

		refPos.clear();
		refPres.clear();

		stepK1 = std::clock();


	}
	// Close Communication

}

	return 0;
}


