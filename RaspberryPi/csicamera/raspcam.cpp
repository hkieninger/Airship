#include "raspcam.hpp"

Rasbcam::Rasbcam(){
  innit();
}

Rasbcam::~Rasbcam(){

}


int Rasbcam::innit(){
    raspicam::RaspiCam Camera; //Camaera object
    if ( !Camera.open()) return -1;
}

char Rasbcam::capture(){
    Camera.grab();
    //allocate memory
    unsigned char *data=new unsigned char[  Camera.getImageTypeSize (raspicam::RASPICAM_FORMAT_YUV420)];
    //extract the image in rgb format
    Camera.retrieve (data,raspicam::RASPICAM_FORMAT_YUV420); //get camera image
    return data;
}


int Rasbcam::getSize(){
  return Camera.getImageTypeSize (raspicam::RASPICAM_FORMAT_YUV420);
}
