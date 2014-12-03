#include "QBchain.h"

QBchain::QBchain(){

    cube_comm = NULL;
}

bool QBchain::addCubes(std::vector<int> cubeBuf)
{
    QBcube* tmp_cube;

    // CREO GLI ELEMENTI CUBO

    for (std::vector<int>::iterator it = cubeBuf.begin(); it != cubeBuf.end(); ++it){
        tmp_cube = new QBcube(cube_comm, *it);
        cubes.push_back(tmp_cube);
    }

    return true;
}

bool QBchain::addHands(std::vector<int> handBuf){
    QBHand* tmp_hand;

    // CREO GLI ELEMENTI MANO

    for (std::vector<int>::iterator it = handBuf.begin(); it != handBuf.end(); ++it){
        tmp_hand = new QBHand(cube_comm, *it);
        hands.push_back(tmp_hand);
    }

    return true;
}

QBchain::~QBchain()
{
    delete cube_comm;
}


bool QBchain::setPort(const char * P_IN){

    if (P_IN == NULL)
        return false;

    if (!port_.empty()){
        std::cout << "Porta gia' settata" << std::endl;
        return false;
    }

    port_ = P_IN;
    return true;
}

bool QBchain::open(){

    cube_comm = new comm_settings;

    openRS485(cube_comm, port_.c_str());

    if (cube_comm->file_handle == INVALID_HANDLE_VALUE)
        return false;

    Init();

    return true;
}

bool QBchain::close(){

    if (cube_comm == NULL)
        return false;

    closeRS485(cube_comm);

    if (cube_comm->file_handle == INVALID_HANDLE_VALUE)
        return false;
    return true;
}

bool QBchain::Activate(){

    if (cube_comm == NULL)
        return false;

    for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it)
        (*it)->activate();

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it)
        (*it)->activate();

    return true;
}


bool QBchain::moveIt(std::vector<double> position, std::vector<double> stiffness){

    if (position.size() != stiffness.size())
        return false;

    std::vector<QBcube*>::iterator it = cubes.begin();
    std::vector<double>::iterator nP = position.begin();
    std::vector<double>::iterator nS = stiffness.begin();

    while(it != cubes.end()){
        (*it)->setPosition(*nP, *nS);
        ++it;
        ++nP;
        ++nS;
    }

    return true;
}


bool QBchain::closeIt(std::vector<double> position){

    std::vector<QBHand*>::iterator it = hands.begin();
    std::vector<double>::iterator nP = position.begin();


    while(it != hands.end()){
        (*it)->setPosition(*nP);
        ++it;
        ++nP;
    }

    return true;
}


bool QBchain::readIt(){

    short int *meas = new short int(3);
    short int *mHand = new short int;

    for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it){
        meas = (*it)->getMeas();
       std::cout << meas[0] << ' ' << meas[1] << " "<< meas[2] << std::endl;
    }

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it){
        mHand = (*it)->getMeas();
       std::cout << *mHand << std::endl;
    }
    return true;
}

//bool QBchain::reachPosEE(const double *point){
//
//}

double* QBchain::readAng(){

	if ((cubes.size() == 0) && (hands.size() == 0))
		return NULL;

	double* buf = new double(cubes.size() + hands.size());
	int i = 0;

	for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it)
        buf[i++] = (*it)->getAngle();

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it)
        buf[i++] = (*it)->getAngle();

    std::cout << cubes.size() + hands.size() << " " << i << std::endl;

	return buf;
}


void QBchain::Init(){

    for (std::vector<QBcube*>::iterator it = cubes.begin(); it!=cubes.end(); ++it)
        (*it)->Init();

    for (std::vector<QBHand*>::iterator it = hands.begin(); it!=hands.end(); ++it)
        (*it)->Init();
}
