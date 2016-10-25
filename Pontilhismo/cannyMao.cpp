#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;


enum Orientacao
{
    HORIZONTAL,
    MAIS45,
    VERTICAL,
    MENOS45
};

int main(int argc, char** argv)
{
//
//    for(int i=0; i<=10; i++)
//    {
//        float theta= i/10.0*CV_PI*2;
//        float x= cos(theta), y= sin(theta);
//        float angulo= atan2(y, x)*180/CV_PI;
//        Orientacoes pixel;
//
//        if(angulo >= -22.5 && angulo < 22.5)
//            pixel= HORIZONTAL;
//        else if(angulo >= 22.5 && angulo < 67.5)
//            pixel= MAIS45;
//        else if(angulo >= 67.5 && angulo < 112.5)
//            pixel= VERTICAL;
//        else if(angulo >= 112.5 && angulo < 157.5)
//            pixel= MENOS45;
//        else if(angulo >= 157.5 && angulo <= 180)
//            pixel= HORIZONTAL;
//        else if(angulo >= -180 && angulo < -157.5)
//            pixel= HORIZONTAL;
//        else if(angulo >= -157.5 && angulo < -112.5)
//            pixel= MAIS45;
//        else if(angulo >= -112.5 && angulo < -67.5)
//            pixel= VERTICAL;
//        else if(angulo >= -67.5 && angulo < -22.5)
//            pixel= MENOS45;
//
//        cout << theta*180/CV_PI << " " << angulo << " " << pixel << endl;
//    }
//    cin.get();

    VideoCapture cap(0);
    Mat image;

    while(1)
    {
        double T1=30, T2=10;
        Mat borrada, gradX, gradY, grad, ang, sup, strongWeakBords, finalCanny;
        cap >> image;
        cvtColor(image, image, CV_BGR2GRAY);
        imshow("Original", image);
        // filtro gaussiano
        GaussianBlur(image, borrada, Size(5,5), 20, 20);
        // gradiente x
        Sobel(borrada, gradX, CV_32F, 1, 0, 3);
        // gradiente y
        Sobel(borrada, gradY, CV_32F, 0, 1, 3);
        // modulo gradiente g = |x|+|y|
        grad= Mat::zeros(image.size(), CV_32F);
        for(int i=0; i<image.size().width; i++)
        {
            for(int j=0; j<image.size().height; j++)
            {
                grad.at<float>(j,i)= abs(gradX.at<float>(j,i)) + abs(gradY.at<float>(j,i));
            }
        }
//        addWeighted(gradX, 0.5, gradY, 0.5, 0, grad);
        // classificacao pixel
        ang= Mat::zeros(image.size(), CV_8UC1);
        for(int i=0; i<image.size().width; i++)
        {
            for(int j=0; j<image.size().height; j++)
            {
                Orientacao pixel;

                double imaginario=gradY.at<double>(j,i), real= gradX.at<double>(j,i);
                double angulo= atan2(imaginario, real)*180/CV_PI;

                if(angulo >= -22.5 && angulo < 22.5)
                    pixel= HORIZONTAL;
                else if(angulo >= 22.5 && angulo < 67.5)
                    pixel= MAIS45;
                else if(angulo >= 67.5 && angulo < 112.5)
                    pixel= VERTICAL;
                else if(angulo >= 112.5 && angulo < 157.5)
                    pixel= MENOS45;
                else if(angulo >= 157.5 && angulo <= 180)
                    pixel= HORIZONTAL;
                else if(angulo >= -180 && angulo < -157.5)
                    pixel= HORIZONTAL;
                else if(angulo >= -157.5 && angulo < -112.5)
                    pixel= MAIS45;
                else if(angulo >= -112.5 && angulo < -67.5)
                    pixel= VERTICAL;
                else if(angulo >= -67.5 && angulo < -22.5)
                    pixel= MENOS45;

                ang.at<uchar>(j,i)= pixel;
            }
        }
//        sup= Mat::zeros(image.size(), CV_32F);
        strongWeakBords= Mat::zeros(image.size(), CV_8UC1);
        // supressao de nao maximos
        for(int i=1; i<image.size().width-1; i++)
        {
            for(int j=1; j<image.size().height-1; j++)
            {
                float valorGrad= grad.at<float>(j,i);
//                sup.at<float>(j,i)= valorGrad;
                Orientacao pixelOri= (Orientacao)ang.at<uchar>(j,i), bordaOri;
                for(int k=-1; k<=1; k++)
                {
                    for(int l=-1; l<=1; l++)
                    {
                        bordaOri= (Orientacao)ang.at<uchar>(j+k,i+l);
                        float valorGradVizinho= grad.at<float>(j+k,i+l);
                        if(pixelOri == bordaOri && valorGradVizinho>valorGrad)
                        {
                            valorGrad=0;
//                            sup.at<float>(j,i)= 0;
                        }
                    }
                }
                if(valorGrad > T1) // borda forte
                    strongWeakBords.at<uchar>(j,i)= 1;
                else if(valorGrad > T2) // borda fraca
                    strongWeakBords.at<uchar>(j,i)= 2;
            }
        }
        finalCanny= Mat::zeros(image.size(), CV_8UC1);
        for(int i=1; i<image.size().width-1; i++)
        {
            for(int j=1; j<image.size().height-1; j++)
            {
                if(strongWeakBords.at<uchar>(j,i) == 1)
                {
                    finalCanny.at<uchar>(j,i)=255;
                }
                else if(strongWeakBords.at<uchar>(j,i) == 2)
                {
                    for(int k=-1; k<=1; k++)
                    {
                        for(int l=-1; l<=1; l++)
                        {
                            if(strongWeakBords.at<uchar>(j+k,i+l) == 1)
                            {
                                finalCanny.at<uchar>(j,i)=255;
                            }
                        }
                    }
                }
            }
        }
        Mat res;
        Canny(borrada, res, T1, T2);
        imshow("Borrada", finalCanny);
        imshow("Canny", res);
        if(waitKey(30)!=-1)
            break;
    }
    return 0;
}
