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
    VideoWriter res;
    // arquivo a se salvo
    res.open("result3.avi", CV_FOURCC('M','J','P','G'), 15, Size(640,480));
    if(!res.isOpened())
        return -1;
    // abre arquivo do computador
    VideoCapture cap("teste3.avi");
    Mat img, hsv; // imagem em RGB e HSV
    cap >> img;
    resize(img,img,Size(640,480));
    Mat borrada= img.clone(); // imagem borrada
    Mat result= Mat::zeros(img.size(), CV_8UC3); // imagem resultante


    // barras deslizantes
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
    // configura posicao do tilt-shift
    while(1)
    {
        if(img.empty())
            break;
        borrada= img.clone();
        for(int i=0; i<10; i++)
            GaussianBlur(borrada, borrada, Size(9,9),0,0);

        for(int i=0; i<img.size().height; i++)
            addWeighted(img.row(i),alphaPeso(i,l1,l2,d_slider),borrada.row(i),1-alphaPeso(i,l1,l2,d_slider),0,result.row(i));
        imshow("imr",result);
        if(waitKey(30) == 27) break;
    }
    // comeca a gravar
    while(1)
    {
        for(int i=0; i<4; i++)
            cap >> img;
        cap >> img;
        resize(img,img, Size(640,480));
        // converte para hsv
        cvtColor(img, hsv, CV_BGR2HSV);
        vector<Mat> planes;
        split(hsv, planes);
        // aumenta a saturacao da imagem
        planes[1]*=2;
        // junta novamente a imgame
        merge(planes, hsv);
        cvtColor(hsv,img,CV_HSV2BGR);
        if(img.empty())
            break;
        borrada= img.clone();
        // Borra a imagem
        for(int i=0; i<10; i++)
            GaussianBlur(borrada, borrada, Size(9,9),0,0);

        // efeito tilt-shift
        for(int i=0; i<img.size().height; i++)
            addWeighted(img.row(i),alphaPeso(i,l1,l2,d_slider),borrada.row(i),1-alphaPeso(i,l1,l2,d_slider),0,result.row(i));
        res << result; // guarda o frame resultante
        imshow("imr",result);
        if(waitKey(30) != -1) break;
    }
    res.release();
    imwrite("resultado.jpg",result);
    return 0;
}
