#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <time.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int, char**)
{
    srand(time(NULL));
    Mat image;
    Vec3b val;
    int n; // raiz do numero de regioes

    image= imread("abc.jpg",CV_LOAD_IMAGE_COLOR);
    if(!image.data)
        cout << "nao abriu abc.jpg" << endl;

    namedWindow("janela",WINDOW_AUTOSIZE);

    cout << "Digite o Numero de Regioes : ";
    cin >> n;
    int wr= image.size().width/n, hr= image.size().height/n;
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            int rx= rand()%n, ry= rand()%n;
            Mat randpos= image(Rect(wr*rx,hr*ry,wr,hr));
            Mat aux= image(Rect(wr*i,hr*j,wr, hr)).clone();
            randpos.copyTo(image(Rect(wr*i,hr*j,wr, hr)));
            aux.copyTo(image(Rect(wr*rx,hr*ry,wr,hr)));
        }
    }
    imshow("janela", image);
    imwrite("abc2.jpg", image);
    waitKey();
    return 0;
}
