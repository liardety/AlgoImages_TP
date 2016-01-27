//
// Created by Patataa on 27/01/2016.
//

#ifndef ALGOIMAGE_EUCLIDIANDISTANCE_H
#define ALGOIMAGE_EUCLIDIANDISTANCE_H

#include "CImg.h"
#include "Distance.h"

template <typename T>
struct EuclidianDistance : public Distance<T> {
    int _dimension;

    EuclidianDistance(int dimension):_dimension(_dimension){}

    cimg_library::CImg<T> operator()(const T && nu, const cimg_library::CImg<T> origine)const{
        return origine.get_distance(nu,_dimension);
    }
    T operator()(const T & a, const T & b)const{
        T temp = a-b;
        temp *= temp;
        return sqrt(temp);
    }
};

template<typename T>
EuclidianDistance<T> make_EuclidianDistance(int arg);

template<typename T>
EuclidianDistance<T> make_EuclidianDistance(int arg) {
    return EuclidianDistance<T>(arg);
};
#endif //ALGOIMAGE_EUCLIDIANDISTANCE_H
