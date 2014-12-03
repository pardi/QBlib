#ifndef QBHAND_H
#define QBHAND_H

#include <QBcube.h>


class QBHand : public QBcube
{
    public:
        QBHand(int);
        QBHand(comm_settings*, int);
        virtual ~QBHand();

        virtual short int* setPosition(double); // Imposta la posizione
        virtual short int* getMeas(); // Ottiene Misure del motore
        virtual double getAngle();

    protected:


    private:
};

#endif // QBHAND_H
