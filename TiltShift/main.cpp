#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

char TrackbarH[50];
char TrackbarL[50];

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
    l1= h_slider-l_slider;
    l2= h_slider+l_slider;
}

void on_trackbar_l(int, void* )
{
    l1= h_slider-l_slider;
    l2= h_slider+l_slider;
}

int main(int argvc, char** argv)
{
    Mat img= imread("pessoas.jpg");
    resize(img,img,Size(640,480));
    Mat borrada= img.clone();
    Mat result= Mat::zeros(Size(640,480), CV_8UC3);

    for(int i=0; i<10; i++)
        GaussianBlur(borrada, borrada, Size(9,9),0,0);

    namedWindow("imr", 1);
    h_max= img.size().height;
    sprintf(TrackbarH, "Altura    %d", h_max);
    createTrackbar(TrackbarH, "imr", &h_slider, h_max, on_trackbar_h);
    on_trackbar_h(h_slider, 0);

    l_max= img.size().height;
    sprintf(TrackbarL, "Largura %d", l_max);
    createTrackbar(TrackbarL, "imr", &l_slider, l_max, on_trackbar_l);
    on_trackbar_l(l_slider, 0);

    createTrackbar("Forca", "imr", &d_slider, d_max, on_trackbar_d);
    on_trackbar_d(d_slider, 0);

    while(1)
    {
        for(int i=0; i<img.size().height; i++)
            addWeighted(img.row(i),alphaPeso(i,l1,l2,d_slider),borrada.row(i),1-alphaPeso(i,l1,l2,d_slider),0,result.row(i));
        imshow("imr",result);
        if(waitKey(30) != -1) break;
    }
    imwrite("resultado.jpg",result);
    return 0;
}
