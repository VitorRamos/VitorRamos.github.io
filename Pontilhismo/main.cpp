#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;
using namespace cv;

#define STEP 2
#define JITTER 2
#define RAIO 3

int main(int argc, char** argv)
{

    vector<int> yrange;
    vector<int> xrange;

    Mat image, points;

    int width, height;
    int x, y;

    Vec3b gray;

    image= imread("abc.jpg",CV_LOAD_IMAGE_COLOR);

    srand(time(NULL));

    if(!image.data)
    {
        cout << "nao abriu" << argv[1] << endl;
        cout << argv[0] << " imagem.jpg";
        exit(0);
    }

    width=image.size().width;
    height=image.size().height;

    xrange.resize(height/STEP);
    yrange.resize(width/STEP);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for(uint i=0; i<xrange.size(); i++)
    {
        xrange[i]= xrange[i]*STEP+STEP/2;
    }

    for(uint i=0; i<yrange.size(); i++)
    {
        yrange[i]= yrange[i]*STEP+STEP/2;
    }

    points = Mat(height, width, CV_8UC3, Scalar(255,255,255));

    random_shuffle(xrange.begin(), xrange.end());

    for(auto i : xrange)
    {
        random_shuffle(yrange.begin(), yrange.end());
        for(auto j : yrange)
        {
            x = i+rand()%(2*JITTER)-JITTER+1;
            y = j+rand()%(2*JITTER)-JITTER+1;
            gray = image.at<Vec3b>(x,y);
            circle(points, Point(y,x), RAIO, CV_RGB(gray[2],gray[1],gray[0]), -1, CV_AA);
        }
    }
    imwrite("Antes.jpg", points);
    imshow("Antes", points);
    Mat grayImg, borrada, bordas;
    cvtColor(image, grayImg, CV_BGR2GRAY);
    GaussianBlur(grayImg, borrada, Size(3,3), 25, 25);
    Canny(borrada, bordas, 150, 50);
    for(int i=JITTER; i<bordas.size().width-JITTER; i++)
    {
        for(int j=JITTER; j<bordas.size().height-JITTER; j++)
        {
            if(bordas.at<uchar>(j,i) == 255)
            {
                x = i+rand()%(2*JITTER)-JITTER+1;
                y = j+rand()%(2*JITTER)-JITTER+1;
                gray = image.at<Vec3b>(y,x);
                circle(points, Point(x,y), 1, CV_RGB(gray[2],gray[1],gray[0]), -1, CV_AA);
            }
        }
    }
    imshow("Depois", points);
    imwrite("Depois.jpg", points);
    imshow("Canny", bordas);
    imwrite("Canny.jpg", bordas);
    waitKey(0);
    return 0;
}
