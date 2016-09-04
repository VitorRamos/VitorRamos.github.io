#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat image, mask;
    int width, height;
    int nobjects, nobjectshole;

    CvPoint p;
    image = imread("abc.png",CV_LOAD_IMAGE_GRAYSCALE);

    if(!image.data)
    {
        std::cout << "imagem nao carregou corretamente\n";
        return(-1);
    }
    width=image.size().width;
    height=image.size().height;

    p.x=0;
    p.y=0;

    imshow("ha", image);
    waitKey();
    // busca objetos com buracos presentes
    for(int i=0; i<width; i++)
    {
        if(image.at<uchar>(0,i) == 255)
            floodFill(image, cvPoint(i,0), 0);
        if(image.at<uchar>(height-1, i) == 255)
            floodFill(image, cvPoint(i, height-1), 0);
    }
    for(int i=0; i<height; i++)
    {
        if(image.at<uchar>(i,0) == 255)
            floodFill(image, cvPoint(0,i), 0);
        if(image.at<uchar>(i,width-1) == 255)
            floodFill(image, cvPoint(width-1,i), 0);
    }
    nobjects=0;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(image.at<uchar>(i,j) == 255)
            {
                // achou um objeto
                nobjects++;
                p.x=j;
                p.y=i;
                floodFill(image,p,100);
            }
        }
    }
    floodFill(image,cvPoint(0,0),10);
    nobjectshole=0;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(image.at<uchar>(i,j) == 0)
            {
                // achou um objeto
                nobjectshole++;
                p.x=j;
                p.y=i;
                floodFill(image,p, 10);
                floodFill(image,cvPoint(p.x-1,p.y), 70);
            }
        }
    }
    cout << "Numero de objetos : " << nobjects << endl
        <<  "Numero de objetos com buraco : "<< nobjectshole << endl
        << "Numero de objetos sem buraco : " << nobjects-nobjectshole << endl;
    imshow("image", image);
    imwrite("labeling.png", image);
    waitKey();
    return 0;
}
