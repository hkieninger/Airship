#ifndef RASBCAM_H
#define RASBCAM_H 1


#include <raspicam/raspicam.h>

//https://www.uco.es/investiga/grupos/ava/node/40
class Raspcam{
Rasbcam::Rasbcam();
Rasbcam::~Rasbcam();

int Rasbcam::innit();
char Rasbcam::capture();

};

#endif
