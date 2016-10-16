#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 5

using namespace cv;
using namespace std;

Mat complexImage;
Mat padded, filter;
Mat image, imagegray, tmp;
Mat_<float> realInput, zeros;
vector<Mat> planos;
char key;
int dft_M, dft_N;

int gammaL, gammaLmax=100;
int gammaH, gammaHmax=100;
int D0, D0max=100;

void CalculaFiltro(int, void*)
{
    int dft_M= tmp.size().height;
    int dft_N= tmp.size().width;
    double gH= gammaH/10.0;
    double gL= gammaL/10.0;
    double d0= D0/10.0;
    for(int i=0; i<dft_M; i++)
    {
        for(int j=0; j<dft_N; j++)
        {
            double D= pow(i-dft_M/2,2)+pow(j-dft_N/2, 2);
            tmp.at<float> (i,j) = (gH-gL)*(1-exp(-D/pow(d0,2)))+gL;
        }
    }
    Mat comps[]= {tmp, tmp};
    merge(comps, 2, filter);
}

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image )
{
    Mat tmp, A, B, C, D;

    // se a imagem tiver tamanho impar, recorta a regiao para
    // evitar cÃ³pias de tamanho desigual
    image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols/2;
    int cy = image.rows/2;

    // reorganiza os quadrantes da transformada
    // A B   ->  D C
    // C D       B A
    A = image(Rect(0, 0, cx, cy));
    B = image(Rect(cx, 0, cx, cy));
    C = image(Rect(0, cy, cx, cy));
    D = image(Rect(cx, cy, cx, cy));

    // A <-> D
    A.copyTo(tmp);
    D.copyTo(A);
    tmp.copyTo(D);

    // C <-> B
    C.copyTo(tmp);
    B.copyTo(C);
    tmp.copyTo(B);
}


int main(int, char**)
{
    VideoCapture cap;

    namedWindow("filtrada", CV_WINDOW_AUTOSIZE);
    createTrackbar("GammaH", "filtrada", &gammaH, gammaHmax, CalculaFiltro);
    createTrackbar("GammaL", "filtrada", &gammaL, gammaLmax, CalculaFiltro);
    createTrackbar("D0", "filtrada", &D0, D0max, CalculaFiltro);

    cap.open(0);
    if(!cap.isOpened())
        return -1;

//    image= imread("input.png", CV_LOAD_IMAGE_COLOR);
    cap >> image;

    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);


    copyMakeBorder(image, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, BORDER_CONSTANT, Scalar::all(0));

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

    // a funÃ§Ã£o de transferÃªncia (filtro frequencial) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();

    // cria uma matriz temporÃ¡ria para criar as componentes real
    // e imaginaria do filtro ideal
    tmp = Mat(dft_M, dft_N, CV_32F);

    // prepara o filtro passa-baixas ideal


    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa

    for(;;)
    {
        cap >> image;
        cvtColor(image, imagegray, CV_BGR2GRAY);
        imshow("original", imagegray);

        // realiza o padding da imagem
        copyMakeBorder(imagegray, padded, 0,
                       dft_M - image.rows, 0,
                       dft_N - image.cols,
                       BORDER_CONSTANT, Scalar::all(0));

        // limpa o array de matrizes que vao compor a
        // imagem complexa
        planos.clear();
        // cria a compoente real
        realInput = Mat_<float>(padded);

        realInput += Scalar::all(1);
        log(realInput, realInput);
        // insere as duas componentes no array de matrizes
        planos.push_back(realInput);
        planos.push_back(zeros);

        // combina o array de matrizes em uma unica
        // componente complexa
        merge(planos, complexImage);

        // calcula o dft
        dft(complexImage, complexImage);

        // realiza a troca de quadrantes
        deslocaDFT(complexImage);

        // aplica o filtro frequencial
        mulSpectrums(complexImage,filter,complexImage,0);

        // troca novamente os quadrantes
        deslocaDFT(complexImage);

        // calcula a DFT inversa
        idft(complexImage, complexImage, DFT_SCALE);

        // limpa o array de planos
        planos.clear();

        // separa as partes real e imaginaria da
        // imagem filtrada
        split(complexImage, planos);
        exp(planos[0], planos[0]);

        // normaliza a parte real para exibicao
        normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
        imshow("filtrada", planos[0]);

        key = (char) waitKey(10);
        if( key == 27 ) break; // esc pressed!
    }
    return 0;
}
