#include <iostream>
#include <opencv2/opencv.hpp>
#include <stack>

using namespace std;
using namespace cv;

void seedfill(Mat& image, CvPoint p, uchar mark)
{
    uchar inten= image.at<uchar>(p.x,p.y);
    stack<CvPoint> x;
    x.push(p);
    while(!x.empty())
    {
        CvPoint aux= x.top();
        image.at<uchar>(aux.x, aux.y)= mark;
        x.pop();
        if(aux.x+1<image.size().width && image.at<uchar>(aux.x+1, aux.y) == inten)
        {
            x.push(cvPoint(aux.x+1, aux.y));
        }
        if(aux.x-1>=0 && image.at<uchar>(aux.x-1, aux.y) == inten)
        {
            x.push(cvPoint(aux.x-1, aux.y));
        }
        if(aux.y+1<image.size().height && image.at<uchar>(aux.x, aux.y+1) == inten)
        {
            x.push(cvPoint(aux.x, aux.y+1));
        }
        if(aux.y-1>=0 && image.at<uchar>(aux.x, aux.y-1) == inten)
        {
            x.push(cvPoint(aux.x, aux.y-1));
        }
    }
}

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
    imshow("image", image);
    waitKey();
    // busca objetos com buracos presentes

    for(int i=0; i<width; i++)
    {
        if(image.at<uchar>(i,0) == 255)
            seedfill(image, cvPoint(i,0), 0);
        if(image.at<uchar>(i,height-1) == 255)
            seedfill(image, cvPoint(i,height-1), 0);
    }
    for(int i=0; i<height; i++)
    {
        if(image.at<uchar>(0,i) == 255)
            seedfill(image, cvPoint(0,i), 0);
        if(image.at<uchar>(width-1,i) == 255)
            seedfill(image, cvPoint(width-1,i), 0);
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
                p.x=i;
                p.y=j;
                seedfill(image,p,100);
            }
        }
    }
    seedfill(image,cvPoint(0,0),2);
    nobjectshole=0;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(image.at<uchar>(i,j) == 0)
            {
                // achou um objeto
                nobjectshole++;
                p.x=i;
                p.y=j;
                seedfill(image,p, 2);
                seedfill(image,cvPoint(p.x-1,p.y), 70);
            }
        }
    }
    cout << nobjects << " " << nobjectshole << " " << nobjects-nobjectshole << endl;
//  cout << nobjects << endl;
    imshow("image", image);
    imwrite("labeling.png", image);
    waitKey();
    return 0;
}
