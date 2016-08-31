#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    Mat image;
    int width, height;
    VideoCapture cap;
    vector<Mat> planes;
    Mat prevhistR, prevhistG, prevhistB, prevhist;
    int nbins = 256;
    float range[] = {0, 256};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;

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
    Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
    Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));
    Mat histGray(histh, histw, CV_8UC1, Scalar(0,0,0));
    int histograma[256]= {};
    double tempo= time(NULL);
    while(1)
    {
        Mat histR, histG, histB, hist;
        cap >> image;
        split (image, planes);
        Mat gray;
        cvtColor(image, gray, CV_BGR2GRAY);
        calcHist(&gray, 1, 0, Mat(), hist, 1, &nbins, &histrange);

//        imshow("HAHA", planes[0]);
//        for(int i=0; i<planes[0].size().height; i++)
//            for(int j=0; j<planes[0].size().width; j++)
//                histograma[cvRound(planes[0].at<uchar>(i,j))]++;
//        int maxval= 0;
//        for(int i=0; i<256; i++)
//            if(histograma[i]>maxval)
//                maxval= histograma[i];
//        for(int i=0; i<256; i++)
//            histograma[i]= histograma[i]/maxval*histImgR.rows;

//        calcHist(&planes[0], 1, 0, Mat(), histR, 1,
//                 &nbins, &histrange,
//                 uniform, acummulate);
//        calcHist(&planes[1], 1, 0, Mat(), histG, 1,
//                 &nbins, &histrange,
//                 uniform, acummulate);
//        calcHist(&planes[2], 1, 0, Mat(), histB, 1,
//                 &nbins, &histrange,
//                 uniform, acummulate);
//
//        normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
//        normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
//        normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());
          normalize(hist, hist, 0, histGray.rows, NORM_MINMAX, -1, Mat());
          histGray.setTo(Scalar(0));
//        histImgR.setTo(Scalar(0));
//        histImgG.setTo(Scalar(0));
//        histImgB.setTo(Scalar(0));

        for(int i=0; i<nbins; i++)
        {
            line(histGray, Point(i, histh), Point(i, histh-cvRound(hist.at<float>(i))), Scalar(127));
//            line(histImgR, Point(i, histh),
//                 Point(i, histh-cvRound(histR.at<float>(i))),
//                 Scalar(0, 0, 255), 1, 8, 0);
//            line(histImgG, Point(i, histh),
//                 Point(i, histh-cvRound(histG.at<float>(i))),
//                 Scalar(0, 255, 0), 1, 8, 0);
//            line(histImgB, Point(i, histh),
//                 Point(i, histh-cvRound(histB.at<float>(i))),
//                 Scalar(255, 0, 0), 1, 8, 0);
        }
        int ttd= 0;
        if(prevhist.data)
        {
            for(int i=0; i<nbins; i++)
                ttd+= abs(prevhist.at<float>(i)-hist.at<float>(i));
//            for(int i=0; i<nbins; i++)
//                ttd+= abs(histR.at<float>(i)-prevhistR.at<float>(i));
//            for(int i=0; i<nbins; i++)
//                ttd+= abs(histG.at<float>(i)-prevhistG.at<float>(i));
//            for(int i=0; i<nbins; i++)
//                ttd+= abs(histB.at<float>(i)-prevhistB.at<float>(i));
            cout << ttd << endl;
        }
        prevhist= hist;
//        prevhistR= histR;
//        prevhistG= histG;
//        prevhistB= histB;
        histGray.copyTo(gray(Rect(0, 0,nbins, histh)));
//        histImgR.copyTo(image(Rect(0, 0,nbins, histh)));
//        histImgG.copyTo(image(Rect(0, histh,nbins, histh)));
//        histImgB.copyTo(image(Rect(0, 2*histh,nbins, histh)));
        if(ttd>50)
        {
            tempo= time(NULL);
        }
//        cout<< (time(NULL)-tempo) << endl;
        if((time(NULL)-tempo)<10)
            imshow("image", gray);
//        waitKey();
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
