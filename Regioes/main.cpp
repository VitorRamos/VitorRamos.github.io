#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**)
{
    Mat image;
    Vec3b val;
    image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
    if(!image.data)
        cout << "nao abriu biel.png" << endl;
    namedWindow("janela",WINDOW_AUTOSIZE);
    Point p1, p2;
    do
    {
        cout << "Digite p1 : ";
        cin >> p1.x >> p1.y;
        cout << "Digite p2 : ";
        cin >> p2.x >> p2.y;
    }while(p1.x > image.size().width || p2.x > image.size().width  ||
          p2.y > image.size().height || p2.y > image.size().height ||
          p1.x > p2.x || p1.y > p2.y || p1.x < 0 || p1.y < 0 || p2.x < 0 || p2.y < 0); // enquato os pontos nao sao validos
    for(int i=p1.x; i<p2.x; i++)
        for(int j=p1.y; j<p2.y; j++)
            image.at<uchar>(j, i)= 255-image.at<uchar>(j, i); // inverte o valor do pixel
    imshow("janela", image);
    imwrite("biel2.png", image); // salva a imagem
    waitKey();
    return 0;
}
