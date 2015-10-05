//
// Created by Patataa on 21/09/2015.
//
#include "./../include/CImg.h"

#include <string>
#include <iostream>
#include <stdexcept>

const std::string c_imgSrc("./../img/");

using namespace cimg_library;


CImgList<int> getGradientMasks() {
    CImgList<int> masks(2, 3, 3);

    masks(0) = CImg<int>(3, 3, 1, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1);
    masks(1) = CImg<int>(3, 3, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1);

    return masks;
}

template<typename T>
CImg<T> sigma(const CImg<T> & origin){



}

template<typename T, typename S>
CImg<T> phase(const CImg<T> & origin,const CImgList<S> & masks){

    if(masks.size() > 2)
        throw std::runtime_error("Too many masks");

    CImgList<T> results(masks.size(), origin);
    for(unsigned int i = 0; i < masks.size(); ++i)
        results(i) = origin.get_convolve(masks(i));

    return results(1).atan2(results(0));
}

template<typename T, typename S>
CImg<T> getNorme(const CImg<T>& origin, const CImgList<S> & masks){


    CImgList<T> results(masks.size(), origin);

    for(unsigned int i = 0; i < masks.size(); ++i)
        results(i) = origin.get_convolve(masks(i));

    return sqrt( results(0) * results(0) + results(1) * results(1));
}

template<typename T>
CImg<T> getGradientPhase(const CImg<T> & origin){
    CImgList<int> masks(2, 3, 3);
    masks(0) = CImg<int>(3, 3, 1, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1);
    masks(1) = CImg<int>(3, 3, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1);

    return phase(origin, masks);
}

template<typename T>
CImg<T> generateImgTp3(const CImg<T> & first, const CImg<T> & second) {
    CImgList<T> firstFft = first.get_FFT(false),
        secondFft = second.get_FFT(false),
        resultFft(2, std::max(first.width(), second.width()), std::max(first.height(), second.height())),
        final(2);
    firstFft(0).display();
    resultFft(0) = sqrt(firstFft(0).get_mul(firstFft(0)) + firstFft(1).get_mul(firstFft(1)) );
    resultFft(1) = secondFft(1).atan2(secondFft(0));

    final(0) = resultFft(0).get_mul(resultFft(1).get_cos());
    final(1) = resultFft(0).get_mul(resultFft(1).get_sin());

    return final.get_FFT(true)(0);
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
            img("./img/im.bmp" );

    //(gradientNorme(img.get_channel(0))).display();
    (generateImgTp3(CImg<float>("./img/clown.bmp"), CImg<float>("./img/gatlin.bmp"))).display();
    //(filtre(img)).display();

    while(!std::cin);

    std::cout << "End"<<std::endl;
    return 1;
}
