#include "imageabstration.h"
#include "math.h"
#include <assert.h>
#include <stdio.h>

ImageAbstraction::ImageAbstraction(const QString &fileName, const char *format):QImage(fileName, format ){
    UpdateColorRange();
}


ImageAbstraction::ImageAbstraction(const QSize &size, Format format):QImage(size,format){
    UpdateColorRange();
}

ImageAbstraction::ImageAbstraction(const QImage &im):QImage(im){
    UpdateColorRange();
}

ImageAbstraction::ImageAbstraction(const ImageAbstraction &im):QImage(im){
    UpdateColorRange();
}

QRgb* ImageAbstraction::getPixel(int x, int y){
    QRgb *pixel = (QRgb *)this->scanLine(x);
    pixel=(pixel+y);
    return pixel;
}

int ImageAbstraction::getPixelColorIntensity(enum ecolor color,int x, int y){

    switch(color){
        case ImageAbstraction::red:
            return qRed(*getPixel(x,y));
            break;

        case ImageAbstraction::green:
            return qGreen(*getPixel(x,y));
            break;

        case ImageAbstraction::blue:
            return qBlue(*getPixel(x,y));
            break;

    };

    return -1;

}

QRgb* ImageAbstraction::setPixel(enum ecolor color, int x, int y, int value){

    QRgb *pixel = getPixel(x,y);

    if(!this->isGreyScale||this->colorCount()==0){

        int red = color==ImageAbstraction::red?value:qRed(*pixel);
        int green = color==ImageAbstraction::green?value:qGreen(*pixel);
        int blue = color==ImageAbstraction::blue?value:qBlue(*pixel);
        *pixel = qRgba(red,green,blue,255);

    }else{

        uint uvalue=value;

        QImage::setPixel(QPoint(x,y), uvalue);


    }

    return pixel;


}

QRgb* ImageAbstraction::setPixel(int x, int y,int red, int green, int blue){

    setPixel(ImageAbstraction::red,x,y,red);
    setPixel(ImageAbstraction::green,x,y,green);
    return setPixel(ImageAbstraction::blue,x,y,blue);
}

int ImageAbstraction::getMaxNumberColor(void){
    int max_blue=-1;
    int max_red=-1;
    int max_green=-1;
    for(int i=0;i<256;i++){
        if(colorcounterblue[i]>max_blue)
            max_blue=colorcounterblue[i];
        if(colorcounterred[i]>max_red)
            max_red=colorcounterred[i];
        if(colorcountergreen[i]>max_green)
            max_green=colorcountergreen[i];
    }

    if(max_green>=max_red && max_green>=max_blue)
        return max_green;
    else if(max_blue>=max_red)
        return max_blue;
    else
        return max_red;
}

int ImageAbstraction::getMaxColorValue(enum ecolor color){
    switch(color){
        case ImageAbstraction::red:
            return this->redmax;
            break;

        case ImageAbstraction::green:
            return this->greenmax;
            break;

        case ImageAbstraction::blue:
            return this->bluemax;
            break;
    };
    return -1;
}

int ImageAbstraction::getMinColorValue(enum ecolor color){

    switch(color){
        case ImageAbstraction::red:
            return this->redmin;
            break;

        case ImageAbstraction::green:
            return this->greenmin;
            break;

        case ImageAbstraction::blue:
            return this->bluemin;
            break;
    };

    return -1;
}

void ImageAbstraction::ApplyFilterGreyScale(){

    for(int x=0;x<this->height();x++){
        for(int y=0;y<this->width();y++){

            int blue=qBlue(*getPixel(x,y));

            int red=qRed(*getPixel(x,y));

            int green=qGreen(*getPixel(x,y));

            int grey=red*0.33+green*0.33+blue*0.33;

            setPixel(x,y,grey,grey,grey);

        }
    }

}

void ImageAbstraction::ApplyFilterContrast(int newmin,int newmax){

    qDebug("New min:%i,max:%i",newmin,newmax);

    for(int x=0;x<this->height();x++){
        for(int y=0;y<this->width();y++){

            setPixel(x,y,
            ApplyFilterContrastRule(red,x,y,newmin,newmax),
            ApplyFilterContrastRule(green,x,y,newmin,newmax),
            ApplyFilterContrastRule(blue,x,y,newmin,newmax));

/*
            qDebug("blue actmax:%i actmin:%i value:%i percentual:%f",
                   this->getMinColorValue(ImageAbstraction::blue),
                   this->getMaxColorValue(ImageAbstraction::blue),
                   newvalue,
                   mul);
*/
        }

    }

    UpdateColorRange();

}

int ImageAbstraction::ApplyFilterContrastRule(enum ecolor color,int x,int y,int min,int max){
    int colorvalue=0;

    switch(color){
    case blue:
        colorvalue=qBlue(*getPixel(x,y));
        break;
    case red:
        colorvalue=qRed(*getPixel(x,y));
        break;
    case green:
        colorvalue=qGreen(*getPixel(x,y));
        break;
    }

    int actmin=this->getMinColorValue(color);
    int actmax=this->getMaxColorValue(color);

    int actdelta=actmax-actmin;
    int newdelta=max-min;
    float mul=((float)newdelta)/((float)actdelta);
    int newcolor=min+mul*((float)(colorvalue-actmin));

    return newcolor;
}

int ImageAbstraction::getColorCounter(enum ecolor color,int level){

    switch(color){
    case blue:
        return colorcounterblue[level];
        break;
    case red:
        return colorcounterred[level];
        break;
    case green:
        return colorcountergreen[level];
        break;
    }

    return -1;

}

void ImageAbstraction::ApplyFilterFusion(ImageAbstraction *fimage,float percentage,int posx,int posy){

    int pcounter=0;

    for(int x=posx;x<this->height();x++){
        for(int y=posy;y<this->width();y++){

            if(y>fimage->width()||x>fimage->height()) break;
                QRgb *pix=getPixel(x,y);

                QRgb *pixext=fimage->getPixel(x-posx,y-posy);

                *pix=qRgba(((float)1-percentage)*qRed(*pix)+percentage*qRed(*pixext),
                           ((float)1-percentage)*qGreen(*pix)+percentage*qGreen(*pixext),
                           ((float)1-percentage)*qBlue(*pix)+percentage*qBlue(*pixext),
                           255);
                pcounter=0;

        }
    }
}

ImageAbstraction* ImageAbstraction::ApplyCrop(int startx,int starty,int endx,int endy){

    ImageAbstraction *newImage=new ImageAbstraction( QSize(endx-startx,endy-starty),format());//QImage::Format_RGB32


    for(int y=starty;y<endy;y++){
        for(int x=startx;x<endx;x++){

                QRgb *pixel = (QRgb *)newImage->scanLine(y-starty);
                pixel=(pixel+x-startx);

                *pixel=qRgba(getPixelColorIntensity(ImageAbstraction::red,y,x),
                             getPixelColorIntensity(ImageAbstraction::green,y,x),
                             getPixelColorIntensity(ImageAbstraction::blue,y,x),
                             255);
        }
    }

    return newImage;

}

void ImageAbstraction::UpdateColorRange(){

    this->isGreyScale=isGrayscale();

    for(int count=0;count<256;count++){
        colorcounterred[count]=0;
        colorcountergreen[count]=0;
        colorcounterblue[count]=0;
    }

    redmax=-1;
    redmin=300;

    greenmax=-1;
    greenmin=300;

    bluemax=-1;
    bluemin=300;

    for(int x=0;x<this->height();x++){
        for(int y=0;y<this->width();y++){
            int blue=qBlue(*getPixel(x,y));
            if(blue>bluemax) bluemax=blue;
            if(blue<bluemin) bluemin=blue;

            int red=qRed(*getPixel(x,y));
            if(red>redmax) redmax=red;
            if(red<redmin) redmin=red;

            int green=qGreen(*getPixel(x,y));
            if(green>greenmax) greenmax=green;
            if(green<greenmin) greenmin=green;

            colorcounterred[red]++;
            colorcountergreen[green]++;
            colorcounterblue[blue]++;        
            //ImageAbstraction* imageCopy = new ImageAbstraction(this));
               //qDebug("W: %i H:%i",imageCopy->width(),imageCopy->height());
        }
    }
}

ImageAbstraction* ImageAbstraction::copy(void){
    return new ImageAbstraction(QImage::copy(0,0,this->width(),this->height()));
}

ImageAbstraction* ImageAbstraction::ApplyGradientMagnitude(){
    ImageAbstraction *k1=this->copy();
    ImageAbstraction *k2=this->copy();

    k1->makeGradFilterX(3);
    k2->makeGradFilterY(3);

    int r1,g1,b1,r2,g2,b2,r,g,b;
    for (int i=0;i<this->height();++i)
        for (int j=0; j<this->width();++j)
        {
            r1 = k1->getPixelColorIntensity(ImageAbstraction::red,i,j);
            g1 = k1->getPixelColorIntensity(ImageAbstraction::green,i,j);
            b1 = k1->getPixelColorIntensity(ImageAbstraction::blue,i,j);

            r2 = k2->getPixelColorIntensity(ImageAbstraction::red,i,j);
            g2 = k2->getPixelColorIntensity(ImageAbstraction::green,i,j);
            b2 = k2->getPixelColorIntensity(ImageAbstraction::blue,i,j);

            r=abs(r1-r2);
            g=abs(g1-g2);
            b=abs(b2-b1);

            this->setPixel(i,j,r,g,b);

        }

    delete(k1);
    delete(k2);

    return this;

}

void ImageAbstraction::makeFilterGaussian(int dim, double sig){
        const double pi = 3.141592;
        const double ee = 2.718281;
        double* kernel = (double*)malloc(sizeof(double)*dim*dim);
        double tmp1, tmp2;
        int center = (int)(dim/2.0);
        for (int i=0; i<dim; ++i)
                for (int j=0; j<dim; ++j)
                {
                        tmp1 = 1.0/(2.0*pi*sig*sig);
                        tmp2 = -1.0*(((i-center)*(i-center)+(j-center)*(j-center))/(2.0*sig*sig));
                        kernel[i*dim+j] = tmp1*pow(ee,tmp2);
                }
        ImageAbstraction::ApplyConvolution(dim, kernel);
        free(kernel);
}
void ImageAbstraction::makeLoG(int dim, double sig){

        const double pi = 3.141592;
        const double ee = 2.718281;
        double* kernel = (double*)malloc(sizeof(double)*dim*dim);
        double tmp1, tmp2,tmp3;
        int center = (int)(dim/2.0);
        for (int i=0; i<dim; ++i)
                for (int j=0; j<dim; ++j)
                {
                        tmp1 = -1.0/(pi*sig*sig*sig*sig);
                        tmp2 = 1-((i*i+j*j)/(2*sig*sig));
                        tmp3 = -1.0*(((i-center)*(i-center)+(j-center)*(j-center))/(2.0*sig*sig));
                        kernel[i*dim+j] = tmp1*tmp2*pow(ee,tmp3);
                }
        ImageAbstraction::ApplyConvolution(dim, kernel);
        free(kernel);

}
int ImageAbstraction::findMax(int* array, int len)
{
    int max = array[0];
    for (int i=1;i<len; ++i)
        if(array[i]>max)
            max = array[i];
    return max;
}
int ImageAbstraction::findMin(int* array, int len)
{
    int min = array[0];
    for (int i=1;i<len; ++i)
        if (array[i]<min)
            min = array[i];
    return min;
}
void ImageAbstraction::minMax(int* oldArr, int oldMin, int oldMax, int newMin, int newMax, int len)
{
    double tmp;
    for (int i=0;i<len; ++i)
    {
        tmp = (((oldArr[i]-oldMin)/(double)(oldMax-oldMin))*(newMax-newMin))+newMin;
        oldArr[i] = (int)tmp;
    }
}
void ImageAbstraction::minMaxDouble(double* oldArr, double oldMin, double oldMax, double newMin, double newMax, int len)
{
    double tmp;
    for (int i=0;i<len; ++i)
    {
        tmp = (((oldArr[i]-oldMin)/(double)(oldMax-oldMin))*(newMax-newMin))+newMin;
        oldArr[i] = tmp;
    }
}

void ImageAbstraction::makeGradFilterX(int dim)
{
    double* kernel = (double*)(malloc(sizeof(double)*dim*dim));
    kernel[0] = -1;
    kernel[1] = 0;
    kernel[2] = 1;
    kernel[3] = -1;
    kernel[4] = 0;
    kernel[5] = 1;
    kernel[6] = -1;
    kernel[7] = 0;
    kernel[8] = 1;
    ImageAbstraction::ApplyConvolution (3,kernel);
    free(kernel);
}
void ImageAbstraction::makeGradFilterY(int dim)
{
    double* kernel = (double*)(malloc(sizeof(double)*dim*dim));
    kernel[0] = 1;
    kernel[1] = 1;
    kernel[2] = 1;
    kernel[3] = 0;
    kernel[4] = 0;
    kernel[5] = 0;
    kernel[6] = -1;
    kernel[7] = -1;
    kernel[8] = -1;
    this->ApplyConvolution(3,kernel);

    free(kernel);
}
void ImageAbstraction::makeLaplacianFilter(int dim)
{
    double* kernel = (double*)(malloc(sizeof(double)*dim*dim));
    if (dim==3)
    {
        kernel[0] = 0;
        kernel[1] = -1;
        kernel[2] = 0;
        kernel[3] = -1;
        kernel[4] = 4;
        kernel[5] = -1;
        kernel[6] = 0;
        kernel[7] = -1;
        kernel[8] = 0;
        ImageAbstraction::ApplyConvolution(3,kernel);
    }
    else
    {
        for (int i=0;i<dim*dim;++i)
            kernel[i] = -1;
        kernel[12] = 24;
        ImageAbstraction::ApplyConvolution(5,kernel);
    }

    free(kernel);

}

int ImageAbstraction::ApplyConvolution(int dim, double* kernel){
        //minMaxDouble(kernel,-1,1,0,1,9);
    /*
        for (int i=0;i<dim;++i)
            for (int j=0;j<dim;++j)
                qDebug("%f KERNEL", (double)(kernel[i*dim+j]));
     */

       int j;  // row    index of the current image
       int i;  // column index of the current image
       int jk; // row    index of the kernel;
       int ik; // column index of the kernel;
       int newval[3]; // new colors
       int size = this->height()*this->width();
       int* tmpImageR = (int*)(malloc(sizeof(int)*size)); // separated by colors
       int* tmpImageG = (int*)(malloc(sizeof(int)*size)); // separated by colors
       int* tmpImageB = (int*)(malloc(sizeof(int)*size)); // separated by colors
       int kernelCenteri; // index of the central column of the kernel
       int kernelCenterj; // index of the central row of the kernel
       double kernelTotalValue;
       kernelCenteri = dim / 2;
       kernelCenterj = dim / 2;
       kernelTotalValue = 0.0;
       for (j = 0; j < dim; j++)
         for(i = 0; i < dim; i++)
           kernelTotalValue += (double)(kernel[j*dim+i]);

       if (kernelTotalValue<=0)
           kernelTotalValue=1;
       // convolution computation
       for (j = 0; j < this->height(); j++) {
         for (i = 0; i < this->width(); i++) {
           newval[0] = 0;
           newval[1] = 0;
           newval[2] = 0;
           for (jk = 0; jk < dim; jk++) {
             for (ik = 0; ik < dim; ik++) {
               int ii = i + ik - kernelCenteri;
               int jj = j + jk - kernelCenterj;
               if ((jj >= 0) && (jj <this->height() ) && (ii >= 0) && (ii < this->width()))
               {
                   newval[0] += getPixelColorIntensity(ImageAbstraction::red,jj,ii) * (double)(kernel[jk*dim+ik]);
                   newval[1] += getPixelColorIntensity(ImageAbstraction::green,jj,ii) * (double)(kernel[jk*dim+ik]);
                   newval[2] += getPixelColorIntensity(ImageAbstraction::blue,jj,ii) * (double)(kernel[jk*dim+ik]);
               }
             }
           }
           newval[0] = newval[0] / kernelTotalValue;
           newval[1] = newval[1] / kernelTotalValue;
           newval[2] = newval[2] / kernelTotalValue;

           tmpImageR[i*this->height()+j] = newval[0];
           //qDebug("%i %i",(i*this->height())+j,tmpImageR[(i*this->height())+j]);
           tmpImageG[i*this->height()+j] = newval[1];
           //qDebug("%i %i",j*this->height()+i,tmpImageG[j*this->height()+i]);
           tmpImageB[i*this->height()+j] = newval[2];
            //qDebug("%i %i",j*this->height()+i,tmpImageB[j*this->height()+i]);

            //qDebug("%i %i %i", this->height(),this->width(),size);
           //setPixel(j,i,newval[0],newval[1],newval[2]);


         }
       }

       minMax(tmpImageR,findMin(tmpImageR,size),findMax(tmpImageR,size),0,255,size);
       minMax(tmpImageG,findMin(tmpImageG,size),findMax(tmpImageG,size),0,255,size);
       minMax(tmpImageB,findMin(tmpImageB,size),findMax(tmpImageB,size),0,255,size);
       for (int i=0; i<this->height();++i)
            for (int j=0;j<this->width();++j)
                setPixel(i,j,tmpImageR[j*this->height()+i],tmpImageG[j*this->height()+i],tmpImageB[j*this->height()+i]);

       return 1;
}

int ImageAbstraction::ApplyConvolutionLaplacian(int dim, double* kernel){
    /*
        for (int i=0;i<dim;++i)
            for (int j=0;j<dim;++j)
                qDebug("%f KERNEL", (double)(kernel[i*dim+j]));
     */
       int j;  // row    index of the current image
       int i;  // column index of the current image
       int jk; // row    index of the kernel;
       int ik; // column index of the kernel;
       int newval[3]; // new colors
       int kernelCenteri; // index of the central column of the kernel
       int kernelCenterj; // index of the central row of the kernel
       double kernelTotalValue;
       kernelCenteri = dim / 2;
       kernelCenterj = dim / 2;
       kernelTotalValue = 0.0;
       for (j = 0; j < dim; j++)
         for(i = 0; i < dim; i++)
           kernelTotalValue += (double)(kernel[j*dim+i]);

       if (kernelTotalValue<=0)
           kernelTotalValue=1;
       // convolution computation
       for (j = 0; j < this->height(); j++) {
         for (i = 0; i < this->width(); i++) {
           newval[0] = 0;
           newval[1] = 0;
           newval[2] = 0;
           for (jk = 0; jk < dim; jk++) {
             for (ik = 0; ik < dim; ik++) {
               int ii = i + ik - kernelCenteri;
               int jj = j + jk - kernelCenterj;
               if ((jj >= 0) && (jj <this->height() ) && (ii >= 0) && (ii < this->width()))
               {
                   newval[0] += getPixelColorIntensity(ImageAbstraction::red,jj,ii) * (double)(kernel[jk*dim+ik]);
                   newval[1] += getPixelColorIntensity(ImageAbstraction::green,jj,ii) * (double)(kernel[jk*dim+ik]);
                   newval[2] += getPixelColorIntensity(ImageAbstraction::blue,jj,ii) * (double)(kernel[jk*dim+ik]);
               }
             }
           }
           if (newval[0]<0)
               newval[0] = 0;
           if (newval[1]<0)
               newval[1] = 0;
           if (newval[2]<0)
               newval[2] = 0;
           if (newval[0]>255)
               newval[0] = 255;
           if (newval[1]>255)
               newval[1] = 255;
           if (newval[2]>255)
               newval[2] = 255;
           setPixel(j,i,newval[0],newval[1],newval[2]);
         }
       }
       return 1;
}
double ImageAbstraction::getMean()
{
    double sum = 0;
    int r,g,b;
    for (int i=0;i<this->height();++i)
        for (int j=0; j<this->width();++j)
        {
            r = this->getPixelColorIntensity(ImageAbstraction::red,i,j);
            g = this->getPixelColorIntensity(ImageAbstraction::green,i,j);
            b = this->getPixelColorIntensity(ImageAbstraction::blue,i,j);
            sum += (r+g+b)/3;
        }
        return sum/(this->height()*this->width());

}

double ImageAbstraction::getStd()
{
    double std = 0.0;
    double myMean = getMean();
    double tmp = 0;
    int r,g,b;
    for (int i=0;i<this->height();++i)
        for (int j=0; j<this->width();++j)
        {
            r = this->getPixelColorIntensity(ImageAbstraction::red,i,j);
            g = this->getPixelColorIntensity(ImageAbstraction::green,i,j);
            b = this->getPixelColorIntensity(ImageAbstraction::blue,i,j);
            tmp += pow((r+g+b/3)-myMean,2);
        }
    std = sqrt(tmp/((this->height()*this->width())-1));
    return std;
}
int ImageAbstraction::RGB2CMYK(int x, int y, enum ecolorcmyk color){
    float r = getPixelColorIntensity(ImageAbstraction::red,x,y);
    float g = getPixelColorIntensity(ImageAbstraction::green,x,y);
    float b = getPixelColorIntensity(ImageAbstraction::blue,x,y);
    float c,m,ye,k;
    k = 1.0;
    if (r==0 && b==0 && g==0)
    {
        c = 0;
        m = 0;
        ye = 0;
        switch(color){
            case ImageAbstraction::cyan:
                return c*100;
                break;
            case ImageAbstraction::black:
                return k*100;
                break;
            case ImageAbstraction::magenta:
                    return m*100;
                    break;
            case ImageAbstraction::yellow:
                    return ye*100;
                    break;
        }
    }
    c = 1-(r/255.0);
    m = 1-(g/255.0);
    ye = 1-(b/255.0);
    if (c<k)
        k = c;
    if (m<k)
        k = m;
    if (ye<k)
        k = ye;    
    c = (c-k)/(1-k);
    m = (m-k)/(1-k);
    ye = (ye-k)/(1-k);
    //qDebug("%i",getPixelColorIntensity(ImageAbstraction::red,0,0));
    //qDebug("%i",getPixelColorIntensity(ImageAbstraction::green,0,0));
    //qDebug("%i",getPixelColorIntensity(ImageAbstraction::blue,0,0));
    //qDebug("%f",c);
    //qDebug("%f",m);
    //qDebug("%f",ye);
    //qDebug("%f",k);

    switch(color){
        case ImageAbstraction::cyan:
            return c*100;
            break;
        case ImageAbstraction::black:
            return k*100;
            break;
        case ImageAbstraction::magenta:
                return m*100;
                break;
        case ImageAbstraction::yellow:
                return ye*100;
                break;
    }
}
void ImageAbstraction::makeMeanFilter(int dim){
    qDebug("MAKING MEAN FILTER");
    double* kernel = (double*)malloc(sizeof(double)*dim*dim);
    for (int i=0; i<dim; ++i)
            for (int j=0; j<dim; ++j)
                    kernel[i*dim+j] = 1.0/(dim*dim);
    ImageAbstraction::ApplyConvolution(dim,kernel);
    free(kernel);

}
void ImageAbstraction::makeCustomKernel(int dim){

    qDebug("MAKING CUSTOM FILTER");
    double* kernel = (double*)malloc(sizeof(double)*dim*dim);
    for (int i=0; i<dim; ++i)
            for (int j=0; j<dim; ++j)
                    kernel[i*dim+j] = 1.0;
    ImageAbstraction::ApplyConvolution(dim,kernel);
    free(kernel);

}


ImageAbstraction* ImageAbstraction::ApplyScale(float xpercentage,float ypercentage){

    //ImageAbstraction *ia=new ImageAbstraction(this->scaled(QSize(xpercentage*width(),ypercentage*height())));
    //return ia;

    ImageAbstraction *newImage=new ImageAbstraction( QSize(xpercentage*width(),ypercentage*height()),format());//QImage::Format_RGB32

    for(int y=0;y<newImage->height();y++){

        for(int x=0;x<newImage->width();x++){

                float yoldfloat=((float)(height()*y))/((float)newImage->height());
                float xoldfloat=((float)(width()*x))/((float)newImage->width());

                int yold=(int)round(yoldfloat);
                int xold=(int)round(xoldfloat);

                /*
                qDebug("in(%i,%i and %i,%i) %f,%f->%i,%i",newImage->height(),newImage->width(),
                    height(),width(),
                    yoldfloat,xoldfloat,
                    y,x);
                */

                /*** INTERPOLATION: start **/

                int xup=ceil(xoldfloat);
                int xlow=floor(xoldfloat);

                int yup=ceil(yoldfloat);
                int ylow=floor(yoldfloat);

                int red=(getPixelColorIntensity(ImageAbstraction::red,yold,xup)+
                        getPixelColorIntensity(ImageAbstraction::red,yold,xlow)+
                        getPixelColorIntensity(ImageAbstraction::red,yup,xold)+
                        getPixelColorIntensity(ImageAbstraction::red,ylow,xold)
                        )/4;

                int green=(getPixelColorIntensity(ImageAbstraction::green,yold,xup)+
                        getPixelColorIntensity(ImageAbstraction::green,yold,xlow)+
                        getPixelColorIntensity(ImageAbstraction::green,yup,xold)+
                        getPixelColorIntensity(ImageAbstraction::green,ylow,xold)
                        )/4;

                int blue=(getPixelColorIntensity(ImageAbstraction::blue,yold,xup)+
                        getPixelColorIntensity(ImageAbstraction::blue,yold,xlow)+
                        getPixelColorIntensity(ImageAbstraction::blue,yup,xold)+
                        getPixelColorIntensity(ImageAbstraction::blue,ylow,xold)
                        )/4;

                newImage->setPixel(y,x,red,
                         green,
                         blue
                         );

                /*** INTERPOLATION: end **/

                /**
                newImage->setPixel(y,x,getPixelColorIntensity(ImageAbstraction::red,yold,xold),
                             getPixelColorIntensity(ImageAbstraction::green,yold,xold),
                             getPixelColorIntensity(ImageAbstraction::blue,yold,xold)
                             );

                             **/

        }
    }

    return newImage;


}

ImageAbstraction* ImageAbstraction::scRemoveLine(int* matrix,int total_columns){

    ImageAbstraction *newImage=new ImageAbstraction( QSize(width()-total_columns,height()),format());

    for(int y=0;y<height();y++){

        for(int x=0,x_last=0;x<width();x++){

            bool found=false;
            for(int xm=0;xm<total_columns;xm++){
               if(matrix[y*total_columns+xm]==x){
                   found=true;
                }
            }
            if(!found){
               newImage->setPixel(y,x_last,
                                   getPixelColorIntensity(ImageAbstraction::red,y,x),
                                   getPixelColorIntensity(ImageAbstraction::green,y,x),
                                   getPixelColorIntensity(ImageAbstraction::blue,y,x));
               x_last++;

            }

        }

    }

    return newImage;
}


ImageAbstraction* ImageAbstraction::scInsertLine(int* seam){

    ImageAbstraction *newImage=new ImageAbstraction( QSize(width()+2,height()),format());

    for(int line=0;line<newImage->height();line++){
        for(int column=0;column<newImage->width();column++){

            newImage->setPixel(line,column,
                                getPixelColorIntensity(ImageAbstraction::red,line,column),
                                getPixelColorIntensity(ImageAbstraction::green,line,column),
                                getPixelColorIntensity(ImageAbstraction::blue,line,column));

            if(seam[4*line]==column){
                int blue_level=(getPixelColorIntensity(ImageAbstraction::blue,line,column-1)+getPixelColorIntensity(ImageAbstraction::blue,line,column+1))/2;
                int green_level=(getPixelColorIntensity(ImageAbstraction::green,line,column-1)+getPixelColorIntensity(ImageAbstraction::green,line,column+1))/2;
                int red_level=(getPixelColorIntensity(ImageAbstraction::red,line,column-1)+getPixelColorIntensity(ImageAbstraction::red,line,column+1))/2;
                newImage->setPixel(line,column,//255,0,0);/*
                                    seam[(4*line)+1],
                                    seam[(4*line)+2],
                                    seam[(4*line)+3]);

      //          newImage->setPixel(line,column+1,//255,0,0);/*
    //                                red_level,
  //                                  green_level,
//                                    blue_level);
            }else if(column>seam[4*line]){
                newImage->setPixel(line,column,
                                    getPixelColorIntensity(ImageAbstraction::red,line,column-2),
                                    getPixelColorIntensity(ImageAbstraction::green,line,column-2),
                                    getPixelColorIntensity(ImageAbstraction::blue,line,column-2));
            }

        }

    }

    return newImage;
}

//transpose the image 45 gradians on the left
ImageAbstraction* ImageAbstraction::transposeLeftImage(){
    ImageAbstraction *newImage=new ImageAbstraction(QSize(height(),width()),format());

    for(int col=0,newlin=this->width()-1;col<width();col++,newlin--)
        for(int lin=0;lin<height();lin++){
            newImage->setPixel(newlin,lin,
                                getPixelColorIntensity(ImageAbstraction::red,lin,col),
                                getPixelColorIntensity(ImageAbstraction::green,lin,col),
                                getPixelColorIntensity(ImageAbstraction::blue,lin,col));
        }


    return newImage;
}

//transpose the image 45 gradians on the right
ImageAbstraction* ImageAbstraction::transposeRightImage(){
    ImageAbstraction *newImage=new ImageAbstraction(QSize(height(),width()),format());

    for(int lin=0,newcol=this->height()-1;lin<height();lin++,newcol--)
        for(int col=0;col<width();col++){
            newImage->setPixel(col,newcol,
                                getPixelColorIntensity(ImageAbstraction::red,lin,col),
                                getPixelColorIntensity(ImageAbstraction::green,lin,col),
                                getPixelColorIntensity(ImageAbstraction::blue,lin,col));
        }

    return newImage;
}

/******************************** SEAM CARVING ***********************************/

//****************** GENERAL METHODS ****************************//
//reference: http://en.wikipedia.org/wiki/Seam_carving
ImageAbstraction* ImageAbstraction::applySeamCarving(float n_width,float n_height){
    int n_w_paths,n_h_paths;
    int * path=(int *) malloc(sizeof(int) * height() * width());
    int * energy_matrix=(int *) malloc(sizeof(int) * (height()*width()) );
    ImageAbstraction *itmp;

    itmp=this;
    //vertical seams
    if (n_width>1){
        /*n_w_paths=round(width() * (n_width-1));
        qDebug("SC: Width: Enlarging image: total paths:%i",n_w_paths);
        increaseImage(n_w_paths);*/
    }else if (n_width<1){
        n_w_paths=round(width() * (1-n_width));
        qDebug("SC: Width: Reducing image: total paths:%i",n_w_paths);
        itmp=decreaseImage(n_w_paths,energy_matrix,path,itmp);
    }

    //horizontal seams
    itmp=itmp->transposeLeftImage();
    if (n_height>1){
    /*    qDebug("SC: Height: Enlarging image");
        n_h_paths=round(height() * (n_height-1));
        increaseImage(n_h_paths);*/
    }else if (n_height<1){
        n_h_paths=round(height() * (1-n_height));
        qDebug("SC: Height: Reducing image: total paths:%i",n_h_paths);
        itmp=decreaseImage(n_h_paths,energy_matrix,path,itmp);
    }
    itmp=itmp->transposeRightImage();


    free(path);
    free(energy_matrix);

    return itmp;
}

void ImageAbstraction::createEnergyMatrix(int * energy_matrix,ImageAbstraction *ia){
    int line,column,current_pixel,neighbor_pixel1,neighbor_pixel2,neighbor_pixel3,prev_column,next_column,prev_line;

    //first line:
    line=0;
    for(int col = 0; col < ia->width(); col++) {
        current_pixel = ia->getPixelColorIntensity(ImageAbstraction::blue,line,col);
        energy_matrix[ia->width()*line+col]=current_pixel;
    }
    //*/

    //next lines:
    line=1;
    prev_line=line-1;
    do{
        column=0;
        prev_column=column;
        next_column=findMinValue(column+1,column+1,ia->width()-1);
        do{
            current_pixel   = ia->getPixelColorIntensity(ImageAbstraction::blue,line,column);
            neighbor_pixel1 = energy_matrix[ia->width()*prev_line+prev_column];
            neighbor_pixel2 = energy_matrix[ia->width()*prev_line+column];
            neighbor_pixel3 = energy_matrix[ia->width()*prev_line+next_column];
            energy_matrix[ia->width()*line+column]=findMinValue(current_pixel+neighbor_pixel1,
                                                                current_pixel+neighbor_pixel2,
                                                                current_pixel+neighbor_pixel3);

            column++;
            prev_column=column-1;
            next_column=findMinValue(column+1,column+1,ia->width()-1);
        }while(column<ia->width());

        line++;
        prev_line=line-1;
    }while(line<ia->height());
}

void ImageAbstraction::calculatePrevAndNextColumn(int * prev_column,int * next_column,int col_min_value, ImageAbstraction *ia){
    *prev_column=col_min_value-1;
    *next_column=col_min_value+1;
    if(col_min_value==0)
        *prev_column=col_min_value;
    else if(col_min_value==ia->width()-1)
        *next_column=col_min_value;
}


//seek the min value at last line
int ImageAbstraction::findMinimunValueLastLine(int * energy_matrix,ImageAbstraction *ia){
    int col_min_value=0;
    int lin=ia->height()-1;
    for(int col=0;col<ia->width();col++)
        if (energy_matrix[ia->width()*lin+col] < energy_matrix[ia->width()*lin+col_min_value])
            col_min_value=col;
    return col_min_value;
}

int ImageAbstraction::findMinValue(int value1,int value2, int value3){
    if (value1<=value2 && value1<=value3)
        return value1;
    else if (value2<=value3)
        return value2;
    else
        return value3;
}

int ImageAbstraction::findColumnMinValue(int * energy_matrix,int lin,int prev_col,int col,int next_col,ImageAbstraction *ia){
    if      (energy_matrix[ia->width()*lin+prev_col] <= energy_matrix[ia->width()*lin+col]
            &&
            energy_matrix[ia->width()*lin+prev_col] <= energy_matrix[ia->width()*lin+next_col])
        return prev_col;
    else if (energy_matrix[ia->width()*lin+col] <= energy_matrix[ia->width()*lin+next_col])
        return col;
    else
        return next_col;
}

void ImageAbstraction::printMatrix(int * matrix,int n_lin, int n_col){
    for (int col = 0; col < n_col; col++) {
        qDebug("col: %i",col);
        for (int lin = 0; lin < n_lin; lin++){
            int current_pixel   = matrix[n_col*lin+col];
            qDebug("lin,%i,%i",lin,current_pixel);
        }
    }
}

void ImageAbstraction::saveMatrixInFile(char * nome_file,int * matrix,int n_cols,int n_lines){
    FILE *fp;
    fp = fopen(nome_file,"w");
    fprintf(fp, "X,");
    for (int col = 0; col < n_cols; col++)
        fprintf(fp, "%i,",col);
    for (int lin = 0; lin < n_lines; lin++){
        fprintf(fp, "\n%i,",lin);
        for (int col = 0; col < n_cols; col++) {
            int current_pixel   = matrix[n_cols*lin+col];
            fprintf(fp, "%i,", current_pixel);
        }
    }
    fclose(fp) ;
}

//******************** DECREASING METHODS *****************************//
ImageAbstraction* ImageAbstraction::decreaseImage(int n_paths,int * energy_matrix,int * path,ImageAbstraction* ia){
    ImageAbstraction *image_temp=ia->copy(),*energy_image=ia->copy();

    energy_image->ApplyGradientMagnitude();
    energy_image->ApplyFilterGreyScale();

    for(int i=0;i<n_paths;i++){
        createEnergyMatrix(energy_matrix,energy_image);
        findPath(energy_matrix,path,path,0,1,energy_image);
        image_temp=image_temp->scRemoveLine(path,1);
        energy_image=energy_image->scRemoveLine(path,1);
    }
    delete(energy_image);

    return image_temp;
}

void ImageAbstraction::findPath(int * energy_matrix,int * path,int * removal_paths,int id_path,int n_paths, ImageAbstraction *ia){
    int prev_column,next_column,lin;
    int col_min_value=findMinimunValueLastLine(energy_matrix,ia);
    lin=ia->height()-1;
    path[lin]=col_min_value;

    //find the path, from mininum value up, with its neighbors
    for(int lin=ia->height()-2;lin>=0;lin--){
        calculatePrevAndNextColumn(&prev_column,&next_column,col_min_value,ia);

        col_min_value = findColumnMinValue(energy_matrix, lin, prev_column, col_min_value, next_column,ia);

        path[lin]=col_min_value;
    }

}


