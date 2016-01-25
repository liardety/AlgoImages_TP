//
// Created by Patataa on 25/01/2016.
//

#include <string>
#include <iostream>
#include "../include/CImg.h"

using namespace cimg_library;

static const int nbCluster = 2;
static const unsigned int tMax = 10;
static const double epsilon = 0.01;

int main(int argc,char **argv) {
    const std::string fileLocation("");
    const std::string filename("./img/firstSample.bmp");

    CImg<> origin((fileLocation + filename).c_str());
    CImgList<> clusters(nbCluster, origin.width(), origin.height(), 1,1, 1.0 / float(nbCluster) );

    std::cout << epsilon << std::endl;
    CImgDisplay disp(origin,"Input Image");

    boolean end = false;
    for(unsigned int i = 0 ; !end && i < tMax; ++i) {

    }


    while (!disp.is_closed())
    {

        disp.wait();
    }
    std::cout << "End"<<std::endl;

    return 1;
}