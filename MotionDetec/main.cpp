#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat image;
    int width, height;
    VideoCapture cap;
    vector<Mat> planes;
    Mat histR;
    int nbins = 64;
    float range[] = {0, 256};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;
    bool movimento= false;

    cap.open(0);

    if(!cap.isOpened())
    {
        cout << "cameras indisponiveis";
        return -1;
    }

    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    int histw = nbins, histh = nbins/2;
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));

    vector<Mat> prevHist; // histogramas anteriores
    prevHist.resize(1);
    double contFrame= 0;
    while(1)
    {
        cap >> image;
        resize(image, image, Size(640,480));
        split (image, planes);
        calcHist(&planes[0], 1, 0, Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
        histImgR.setTo(Scalar(0));
        for(int i=0; i<nbins; i++)
        {
            line(histImgR, Point(i, histh),
                 Point(i, histh-cvRound(histR.at<float>(i))),
                 Scalar(0, 0, 255), 1, 8, 0);
        }
        histImgR.copyTo(image(Rect(0, 0,nbins, histh)));
        if(prevHist[0].data) // verifica se ja existe histograma anterior
        {
            movimento= false; // movimento inicialmente falso
            // verifica se existe uma diferenca muito grande
            if(compareHist(histR, prevHist[0], CV_COMP_CHISQR)>10)
                movimento= true;
        }
        if(movimento) // se tiver movimento escreve na tela
        {
            putText(image, "Movimento Detectado.", cvPoint(1,110),
                FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,250), 1, CV_AA);
        }
        if(contFrame>15) // atualiza histograma de comparacao a cada 15 frames
        {
            contFrame= 0;
            prevHist[0]= histR.clone(); // copia o histograma
        }
        contFrame++;
        imshow("image", image);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
