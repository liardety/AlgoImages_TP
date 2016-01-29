//
// Created by Patataa on 25/01/2016.
//
//  Windows
#ifdef _WIN32
#include <Windows.h>
double get_wall_time(){
    LARGE_INTEGER time,freq;
    if (!QueryPerformanceFrequency(&freq)){
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)){
        //  Handle error
        return 0;
    }
    return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time(){
    FILETIME a,b,c,d;
    if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
        //  Returns total user time.
        //  Can be tweaked to include kernel times as well.
        return
                (double)(d.dwLowDateTime |
                         ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    }else{
        //  Handle error
        return 0;
    }
}

//  Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}
#endif
#include <string>
#include <iostream>
#include <vector>
#include <Distance.h>
#include <limits>
#include <algorithm>

#include "CImg.h"
#include "KFCM.h"
#include "Kernel.h"
#include "EuclidianDistance.h"

using namespace cimg_library;

static const int nbCluster = 2;
static const unsigned int tMax = 200;
static const float EPSILON = 0.001;
static const double fuzzyCoef = 2;

static const double sigma = 150;
static const float alpha = 0.5;  // alpha lies between 0 and 1 inclusive
static const unsigned int nbNeighbors = 8;  // number of neighbor taken in count for the spacially constrained KFCM


int main(int argc, char **argv) {

    // Getting the sample
    const std::string fileLocation("");
    const std::string filename("./img/firstSample.bmp");

    const CImg<> origin((fileLocation + filename).c_str());

    // Clusters initializing

    CImgList<> clusters(nbCluster, origin.width(), origin.height(), 1,1, 1.0 / float(nbCluster) );
    CImgList<> clustersSave(clusters); // sauvegarde de l'itération précédente pour le critaire d'arrêt.
    CImgList<> influence(clusters);
    std::vector<float> nu(nbCluster, 122);

    float valueInit = 255 / nbCluster;
    for(unsigned int i = 0; i < nbCluster; ++i) {
        nu[i] = valueInit * i;
    }

    std::cout << EPSILON << std::endl;
    CImgDisplay disp(origin,"Input Image");
    

    unsigned int sizeBuffer = origin.width() * origin.height();

    auto euclidianDistance = make_EuclidianDistance<float>(nbCluster);
    auto kernel = make_kernel(euclidianDistance,sigma);
    auto kfcm = make_KFCM<float>(kernel, fuzzyCoef);

    //On definit l'epsilon comme très grande valeur
    float epsilon = std::numeric_limits<float>::max();

    //Boucle principale
    for(unsigned int t = 0 ; epsilon > EPSILON && t < tMax; ++t) {
        
        // mise à jour des nu
        for(unsigned int i = 0; i < nbCluster; ++i) { // iteration sur les clusters
            float* ptrCluster = clusters(i).data();
            const float* ptrOrigin = origin.data();
            float denomi = 0;
            float nume = 0;

            for( unsigned int k = 0; k < sizeBuffer; ++k ){  // iteration sur les pixels

                float computedKernel = std::pow(*(ptrCluster + k), fuzzyCoef) * kernel(*(ptrOrigin + k), nu[i]);
                denomi += computedKernel;
                nume += computedKernel * *(ptrOrigin + k);
            }
            assert(denomi != 0);

            nu[i] = nume / denomi;
            std::cout<<"Nu i : " <<i << " : " << nu[i] << std::endl;
        }



        double wall0 = get_wall_time();
        double cpu0  = get_cpu_time();

        // mise à jour des clusters
        const float* ptrOrigin = origin.data();
/*
        for( unsigned int k = 0; k < tailleBuffer; ++k){  // iteration sur les pixels 
          float denomi = 0;
          for(unsigned int j = 0; j < nbCluster; ++j){  // calcul de dénominateur nécessitant une sommation pour toutes les classes
            denomi += std::pow(1 - kernel(*(ptrOrigin + k), nu[j]), -1/(fuzzyCoef-1));
          }
          assert(denomi != 0);
          for(unsigned int i = 0; i < nbCluster; ++i){  // calcul pour chaque classe
            *(clusters(i).data() + k) = std::pow(1 - kernel(*(ptrOrigin + k), nu[i]),-1/(fuzzyCoef-1) ) / denomi;
          }
        }
        */
        for(unsigned int i = 0; i < nbCluster; ++i){
            CImg_3x3(I,float);
            cimg_for3x3(clustersSave(i),x,y,0,0,I,float) {

                float intermediate = 0;
                for(unsigned int l = 0; l < nbNeighbors + 1; ++l){
                    intermediate += std::pow((1-I[i]), fuzzyCoef);
                }
                intermediate -= std::pow((1-Icc), fuzzyCoef);

//                float intermediate = std::pow(1-Ipp, fuzzyCoef);
//                intermediate += std::pow(1-Ipc, fuzzyCoef);
//                intermediate += std::pow(1-Ipn, fuzzyCoef);
//                intermediate += std::pow(1-Ipc, fuzzyCoef);
//                intermediate += std::pow(1-Icp, fuzzyCoef);
//                intermediate += std::pow(1-Ipn, fuzzyCoef);
//                intermediate += std::pow(1-Icn, fuzzyCoef);
//                intermediate += std::pow(1-Inn, fuzzyCoef);


                influence(i)(x,y) = ( alpha / (float)nbNeighbors ) * intermediate;
            }
        }
        std::vector<float> neighborInfluence(nbCluster);
        std::vector<float> Uik;
        for( unsigned int k = 0; k < sizeBuffer; ++k){  // iteration sur les pixels

            for(unsigned int i = 0; i < nbCluster; ++i){
                neighborInfluence.push_back(*(*(influence.data(i)) + k));
            }
            Uik = kfcm.updateClusters(*(ptrOrigin + k), nu, neighborInfluence);

            for(unsigned int i = 0; i < nbCluster; ++i){  // calcul pour chaque classe
                *(clusters(i).data() + k) = Uik[i];
            }
            neighborInfluence.clear();
        }

        double wall1 = get_wall_time();
        double cpu1  = get_cpu_time();

        std::cout << "Wall Time = " << wall1 - wall0 <<  std::endl;
        std::cout << "CPU Time  = " << cpu1  - cpu0  <<  std::endl;


        //TODO : ameliorer le calcul de somme des différences pour l'EPSILON

        //Filtre des clusters
        //Operation de tri des pixels : un pixel de Uik n'appartient en fin de boucle qu'à un seul cluster
        for(unsigned int k = 0; k < sizeBuffer; ++k){
            unsigned int clusterOwner = 0;
            float max = 0;
            for(unsigned int i = 0; i < nbCluster; ++i){  // iteration sur les classes
              // iteration sur les pixels
                float * pixel = (clusters(i).data() + k);
                float value = *pixel;

                if(  value > max) {
                    max = value;
                    clusterOwner = i;
                }
                *pixel = 0;
            }
            *(clusters(clusterOwner).data() + k) = 255;
        }

        // Fixed : critère d'arrêt correct
        epsilon = std::abs(*(clusters(0).data())) - *(clustersSave(0).data());
        for(unsigned int k = 0; k < sizeBuffer; ++k){
            for(unsigned int i = 0; i < nbCluster; ++i){  // iteration sur les classes
                epsilon = std::max(epsilon, std::abs(*(clusters(i).data() + k)) - *(clustersSave(i).data() + k)) ;
            }
        }

//        epsilon /= 255;
        clustersSave = clusters;
        std::cout << "T : " << t << " Epsilon : " <<epsilon<<  std::endl;

    }


    while (!disp.is_closed())
    {

        disp.wait();
    }

    CImgDisplay disp2(clusters(0),"Input Image");
    while (!disp2.is_closed())
    {
        disp2.wait();
    }

    CImgDisplay disp3(clusters(1),"Input Image");
    while (!disp3.is_closed())
    {
        disp3.wait();
    }
    std::cout << "End"<<std::endl;

    return 1;
}
