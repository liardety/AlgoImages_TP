//
// Created by Patataa on 21/09/2015.
//
#include "./../include/CImg.h"

#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <functional>


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
CImg<T> getNorme(const CImgList<T> & origin) {
    if(origin.size() != 2)
        throw std::runtime_error("Incorrect number of img in the list");

    return sqrt(origin(0).get_mul(origin(0)) + origin(1).get_mul(origin(1)));
}

template<typename T>
CImg<T> generateImgTp3(const CImg<T> & first, const CImg<T> & second) {
    CImgList<T> firstFft = first.get_FFT(false),
        secondFft = second.get_FFT(false),
        resultFft(2),
        final(2);


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


template <typename T, typename S>
double  getG_a(const T & gradient, const S & lambda ) {

    double intermediate = lambda * gradient;
    return 1 / (1 + intermediate * intermediate);
}


template <typename T, typename S>
double getG_b(const T & gradient, const S & lambda) {
    double intermediate = lambda * gradient;
    return exp(- intermediate * intermediate);
}

template<typename T, typename S, typename U >
CImg<> PeronaMalik2D(
        CImg<T> imageIn,
        unsigned int nbiter,
        int option,
        S delta_t,
        U lambda) {
    CImg<T> imageOut(imageIn);
    std::vector<CImgList<CImg<T> > > gradient(2);

    std::vector<double (*)(const T &, const U &)> func = {getG_a, getG_b };
    //GetGradient

    //First : Backward : N / W
    //Second : Forward : E / S
    unsigned j = 0;

    for(unsigned int iter = 0; iter < nbiter; ++iter) {
        cimg_forXY(imageIn, x, y) {
                for (int i = -1; i <= 1; i += 2) {
                    gradient[j] = imageIn.get_gradient("xy", i);
                    ++j;
                }

                imageOut += delta_t * (
                        func[option](abs(gradient[1](0)(x, y)), lambda) * gradient[1](0)(x, y) -
                        func[option](abs(gradient[0](0)(x, y)), lambda) * gradient[0](0)(x, y) +
                        func[option](abs(gradient[1](1)(x, y)), lambda) * gradient[1](1)(x, y) -
                        func[option](abs(gradient[0](1)(x, y)), lambda) * gradient[0](1)(x, y)
                );
            }
    }

    return imageOut;
};
//====================================================================================
// TP6
//====================================================================================

CImg<> avg_filter_3x3( const CImg<> & orig){

    static const CImg<float> mask(3,3,1,1, 1/9 );

    return orig.get_convolve(mask);
}
CImgList<> avg_filter_3x3(const CImgList<> & orig){
    CImgList<> result(orig);
    for(unsigned int i = 0; i < orig.size(); ++i) {
        result(i) = avg_filter_3x3(orig(i));
        ++i;
    }
    return result;
}

CImg<> HornSchunck(CImg<> seq)
{

    CImg<> field(seq.width(),seq.height(),1,2);

    CImgList<> gradient = seq.get_gradient("xyz");
    double factor = 0;
    cimg_forXYC(field,x,y,v)
    field(x,y,v) = 0.01;

    CImgList<> UV(2, seq.width(),seq.height(),1,1,0.f),
        UV_avg(UV),
        UV_2(UV);

    static const unsigned int k_max = 5;
    static const double lambda = 10;

    double denominator = 0;
    for(unsigned int k = 0; k <= k_max; ++k ) {
        UV_avg = avg_filter_3x3(UV);

        cimg_forXY(seq, x, y){
            denominator =
                    (lambda * lambda) +
                    gradient(0)(x,y) * gradient(0)(x,y) +
                    gradient(1)(x,y) * gradient(1)(x,y);

            factor = (gradient(0)(x,y) *
                    UV_avg(0)(x,y) +
                    gradient(1)(x,y) *
                    UV_avg(1)(x,y) +
                    gradient(2)(x,y) )
                 /denominator;

            for(unsigned int i = 0; i <= 1 ; ++i)
                UV_2(i) = UV_avg(i) - gradient(i) * factor;

        }
    }

    cimg_forXYC(field,x,y,c)
    field(x,y,c) = UV_2(c)(x,y);


    return field;
}


int main(int argc, char **argv) {
    std::cout << "Begin"<<std::endl;

    //CImg<float>
      //      img("./img/gatlin.bmp");

    //(gradientNorme(img.get_channel(0))).display();
    //(generateImgTp3(CImg<float>("./img/clown.bmp"), CImg<float>("./img/gatlin.bmp"))).display();
    //(filtre(img)).display();


    //std::function<const char *(CImg<float>) > func = [](const char * filename) { return getNorme(CImg<float>(filename).get_FFT(false)).get_log()};
    /*
    (
            (0.000001 + getNorme(CImg<float>("./img/lena_trame.bmp").get_FFT(false))).get_log(),
            (0.000001 + getNorme(CImg<float>("./img/lena.bmp").get_FFT(false))).get_log()
    ).display();
    */

    CImg<> seq = CImg<>("./img/rubic1.bmp").channel(0);
    seq.append(CImg<>("./img/rubic2.bmp").channel(0),'z');


    (CImg<>("./img/rubic1.bmp")).display();

    CImg<> displacementHS = HornSchunck(seq);
    float color=500; unsigned int  sampling = 8; float  factor = 40; int  quiver_type = 0; float  opacity = 0.5;

    CImg<> imageHS = seq.get_slice(0).draw_quiver(displacementHS,&color,opacity,sampling,factor,quiver_type);
    CImgDisplay resHS_disp(imageHS,"Horn et Schunck");

    while (!resHS_disp.is_closed())
    {

        resHS_disp.wait();
    }
    std::cout << "End"<<std::endl;
    return 1;
}
