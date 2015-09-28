//
// Created by Patataa on 21/09/2015.
//
#include "./../include/CImg.h"

#include <string>
#include <iostream>

const std::string c_imgSrc("./../img/");

using namespace cimg_library;


int main(int argc, char **argv) {
    std::cout << "Begin"<<std::endl;

    CImg<>
        img("./img/clown.bmp" ),
        result("./img/clown.bmp" );
    CImgList<float> gradient(2, img);
    img.blur(2);
    std::cout<<"Blur"<<std::endl;

    cimg_forXY(img, x, y) {

        gradient(0)(x, y) = img(x + 1, y) - img(x, y);
        gradient(1)(x, y) = img(x, y + 1) - img(x, y);

        result(x,y) = gradient(0)(x,y) * gradient(0)(x,y) + gradient(1)(x,y) * gradient(1)(x,y);
    };
    img.display();
    result.display();

    while(!std::cin);

    std::cout << "End"<<std::endl;
    return 1;
}
