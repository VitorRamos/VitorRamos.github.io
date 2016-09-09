#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void MostraHist(Mat &image, bool equaliza= false)
{
    vector<Mat> planes;
    Mat histR, histG, histB;
    int nbins = 256;
    float range[] = {0, 256};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;

    int histw = nbins, histh = nbins/2;
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
    Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
    Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

    split (image, planes);
    if(equaliza) // equaliza os 3 planos R G B
    {
        equalizeHist(planes[0], planes[0]);
        equalizeHist(planes[1], planes[1]);
        equalizeHist(planes[2], planes[2]);
    }
    merge(planes, image); // junta os planos novamente na imagem
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);

    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for(int i=0; i<nbins; i++)
    {
        line(histImgR, Point(i, histh),
             Point(i, histh-cvRound(histR.at<float>(i))),
             Scalar(0, 0, 255), 1, 8, 0);
        line(histImgG, Point(i, histh),
             Point(i, histh-cvRound(histG.at<float>(i))),
             Scalar(0, 255, 0), 1, 8, 0);
        line(histImgB, Point(i, histh),
             Point(i, histh-cvRound(histB.at<float>(i))),
             Scalar(255, 0, 0), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh,nbins, histh)));
}


int main(int argc, char** argv)
{
    Mat image;
    int width, height;
    VideoCapture cap;
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

    while(1)
    {
        cap >> image;
        resize(image, image, Size(640,480));
        MostraHist(image);
        imshow("Original", image);
        MostraHist(image, 1);
        imshow("image", image);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
