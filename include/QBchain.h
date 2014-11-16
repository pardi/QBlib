#ifndef QBCHAIN_H
#define QBCHAIN_H

#include <vector>
#include "QBcube.h"
#include "QBHand.h"


class QBchain
{

    comm_settings* cube_comm;
    std::vector<QBcube*> cubes;
    std::vector<QBHand*> hands;
    std::string port_;

    public:
        QBchain();
        virtual ~QBchain();

// FUNZIONI OPERATIVE

// ---------------------------------------------------------------------
// Setta la porta di comunicazione
// ---------------------------------------------------------------------
        bool setPort(const char*);
// ---------------------------------------------------------------------
// Apertura comunicazione
// ---------------------------------------------------------------------
        bool open();
// ---------------------------------------------------------------------
// Chiusura comunicazione
// ---------------------------------------------------------------------
        bool close();

// FUNZIONI MEMBRO

// ---------------------------------------------------------------------
// Aggiunta elementi cubo alla catena
// ---------------------------------------------------------------------
        bool addCubes(std::vector<int>);
// ---------------------------------------------------------------------
// Aggiunta elementi mano alla catena
// ---------------------------------------------------------------------
        bool addHands(std::vector<int>);
// ---------------------------------------------------------------------
// Muovi elementi della catena
// ---------------------------------------------------------------------
        bool moveIt(std::vector<double>, std::vector<double>);
// ---------------------------------------------------------------------
// Chiudi elementi Hand
// ---------------------------------------------------------------------
        bool closeIt(std::vector<double>);
// ---------------------------------------------------------------------
// Legge informazioni dagli elementi della catena
// ---------------------------------------------------------------------
        bool readIt();
// ---------------------------------------------------------------------
// Raggiungi Posizione E-E
// ---------------------------------------------------------------------
        bool reachPosionEE(const double*);
// ---------------------------------------------------------------------
// Legge Posizione degli angoli [rad]
// ---------------------------------------------------------------------
        double* readAng();

    protected:
// ---------------------------------------------------------------------
// Genera Jacobiano tramite la matrice di Denavit-Hartemberg
// ---------------------------------------------------------------------
        void DH2Jacobian(const char *);

    private:
// ---------------------------------------------------------------------
// Attiva gli elementi QB~
// ---------------------------------------------------------------------
        bool Activate();

};

#endif // QBCHAIN_H

