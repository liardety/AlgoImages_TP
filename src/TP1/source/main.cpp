//
// Created by Patataa on 21/09/2015.
//
#include "./../include/CImg.h"

#include <string>
#include <iostream>
#include <stdexcept>

const std::string c_imgSrc("./../img/");

using namespace cimg_library;







template<typename T, typename S>
CImg<T> filter(const CImg<T> & origin,const CImgList<S> & masks){

    if(masks.size() > 2)
        throw std::runtime_error("Too many masks");

    CImgList<T> results(masks.size(), origin);
    for(unsigned int i = 0; i < masks.size(); ++i)
        results(i) = origin.get_convolve(masks(i));

    return -results(1).atan2(results(0));
}

template<typename T>
CImg<T> getGradient(const CImg<T> & origin){
    CImgList<int> masks(2, 3, 3);
    masks(0) = CImg<int>(3, 3, 1, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1);
    masks(1) = CImg<int>(3, 3, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1);

    return filter(origin.get_channel(0), masks);
}

template<typename T>
CImg<T> tp1(const CImg<T> & origin ){
    CImg<T>result( origin.width(), origin.height());
    CImgList<float> gradient(2, origin.width(), origin.height());
    //img.blur(2);
    std::cout<<"Blur"<<std::endl;

    cimg_forXY(origin, x, y) {

            gradient(0)(x, y) = origin(x + 1, y) - origin(x, y);
            gradient(1)(x, y) = origin(x, y + 1) - origin(x, y);

            result(x,y) = gradient(0)(x,y) * gradient(0)(x,y) + gradient(1)(x,y) * gradient(1)(x,y);
        };


    return result;
}

int main(int argc, char **argv) {
    std::cout << "Begin"<<std::endl;

    CImg<float>
        img("./img/lena.bmp" );

    (getGradient(img.get_channel(0))).display();

    //(filtre(img)).display();

    while(!std::cin);

    std::cout << "End"<<std::endl;
    return 1;
}
