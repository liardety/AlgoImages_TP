//
// Created by Patataa on 30/11/2015.
//

#ifndef ALGOIMAGE_DISTANCE_H
#define ALGOIMAGE_DISTANCE_H

#include <math.h>
#include "../include/CImg.h"

using cimg_library::Cimg<T>

template <typename T>
struct Distance {
    Distance(){}
    ~Distance(){}

    template <typename T1, typename T2,typename ...Args>
    T operator()(const T1 & x, const T2 & y, Args && ... others)const;
};

template <typename T>
struct EuclidianDistance : public Distance<T> {
  //  première implémentation "sale"
  CImg<T> operator()(const T & nu, const CImg<T> origine)const{
    return origine.get_distance(nu,2);
  }
};

#endif //ALGOIMAGE_DISTANCE_H
