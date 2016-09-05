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
    Mat histR, histG, histB;
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

    vector<Mat> prevHist;
    prevHist.resize(3);
    double tempo= time(NULL);
    while(1)
    {
//        Mat histR, histG, histB;
        cap >> image;
//        Mat ycrcb;
//        cvtColor(image,ycrcb,CV_BGR2YCrCb);
//        vector<Mat> channels;
//        split(ycrcb,channels);
//        equalizeHist(channels[0], channels[0]);
//        merge(channels,ycrcb);
//        cvtColor(ycrcb,image,CV_YCrCb2BGR);

        split (image, planes);
        equalizeHist(planes[0], planes[0]);
        equalizeHist(planes[1], planes[1]);
        equalizeHist(planes[2], planes[2]);
        merge(planes, image);
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

        if(prevHist[0].data)
        {
            cout << compareHist(histR, prevHist[0], CV_COMP_CORREL) << " "
            << compareHist(histG, prevHist[1], CV_COMP_CORREL) << " "
            << compareHist(histB, prevHist[2], CV_COMP_CORREL)<< endl;
        }
        prevHist[0]= histR.clone();
        prevHist[1]= histG.clone();
        prevHist[2]= histB.clone();
        imshow("image", image);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
