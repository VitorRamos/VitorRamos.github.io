#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

// nome das barras deslizantes
char TrackbarH[50];
char TrackbarL[50];
// h - barra deslizante da altura
// l - barra deslizante da tamanho da janela de foco
// d - forca do decaimento da janela
int h_slider, h_max;
int l_slider, l_max;
int d_slider, d_max= 200;
double l1, l2;

double alphaPeso(int x, double l1, double l2, double d)
{
    return 0.5*(tanh((x-l1)/d)-tanh((x-l2)/d));
}

void on_trackbar_d(int, void* )
{

}

void on_trackbar_h(int, void* )
{
    // calcula l1 e l2 da funcao
    l1= h_slider-l_slider;
    l2= h_slider+l_slider;
}

void on_trackbar_l(int, void* )
{
    // calcula l1 e l2 da funcao
    l1= h_slider-l_slider;
    l2= h_slider+l_slider;
}

int main(int argvc, char** argv)
{
    Mat img= imread("pessoas.jpg");
    resize(img,img,Size(640,480));
    Mat borrada= img.clone();
    Mat result= Mat::zeros(img.size, CV_8UC3);

    // bora a imagem passando 10 vezes o filtro gaussiano com kernel 9x9
    for(int i=0; i<10; i++)
        GaussianBlur(borrada, borrada, Size(9,9),0,0);

    namedWindow("imr", 1);
    h_max= img.size().height; // altura maxima regiao central
    sprintf(TrackbarH, "Altura    %d", h_max);
    createTrackbar(TrackbarH, "imr", &h_slider, h_max, on_trackbar_h);
    on_trackbar_h(h_slider, 0);

    l_max= img.size().height; // largura regiao central maxima
    sprintf(TrackbarL, "Largura %d", l_max);
    createTrackbar(TrackbarL, "imr", &l_slider, l_max, on_trackbar_l);
    on_trackbar_l(l_slider, 0);

    createTrackbar("Forca", "imr", &d_slider, d_max, on_trackbar_d);
    on_trackbar_d(d_slider, 0);

    while(1)
    {
        // calcula a imagem resultante aplicando a funcao de transicao a cada linha da imagem
        for(int i=0; i<img.size().height; i++)
            addWeighted(img.row(i),alphaPeso(i,l1,l2,d_slider),borrada.row(i),1-alphaPeso(i,l1,l2,d_slider),0,result.row(i));
        imshow("imr",result);
        if(waitKey(30) == 27) break;
    }
    imwrite("resultado.jpg",result); // salva a imagem
    return 0;
}
