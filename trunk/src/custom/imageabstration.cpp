#include "imageabstration.h"
#include "math.h"
#include <assert.h>


ImageAbstraction::ImageAbstraction(const QString &fileName, const char *format):QImage(fileName, format ){

    UpdateColorRange();

}

ImageAbstraction::ImageAbstraction(const QSize &size, Format format):QImage(size,format){

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

    if(!this->isGreyScale){

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
    //float mul=(float)newdelta/(float)actdelta;//((actmax+level))/(actmax-actmin);
    float mul=(float)newdelta/(float)actdelta;//((actmax+level))/(actmax-actmin);
    int newcolor=actmin+mul*(colorvalue-actmin);

    return newcolor;

    /*
    float brightness=-1*((float)min)/((float)255);

    float value=((float)colorvalue)/((float)255);

    float contrast=((float)max)/((float)255);

    if (brightness < 0.0)  value = value * ( 1.0 + brightness);
                      else value = value + ((1.0 - value) * brightness);
    //value = (value - 0.5) * (tan ((contrast + 1) * 2.1415/4) ) + 0.5;

    return (int)(value*(float)255);
    */

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

    int total=this->height()*this->width();
    int numbertoplot=percentage*total;
    int space=total/numbertoplot;
    int pcounter=0;

    for(int x=posx;x<this->height();x++){
        for(int y=posy;y<this->width();y++){

            if(y>fimage->width()) break;

            if(++pcounter==space){
                QRgb *pix=getPixel(x,y);
                QRgb *pixext=fimage->getPixel(x-posx,y-posy);
                *pix=qRgba(qRed(*pixext),qGreen(*pixext),qBlue(*pixext),255);
                pcounter=0;
            }

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

/*
    int minimum=300;
    int maximum=-1;
*/
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
            /*
            if(qRed(*getPixel(x,y))<minimum){
                minimum=qRed(*getPixel(x,y));
            }
            if(qGreen(*getPixel(x,y))<minimum){
                minimum=qGreen(*getPixel(x,y));
            }
            if(qBlue(*getPixel(x,y))<minimum){
                minimum=qBlue(*getPixel(x,y));
            }

            if(qRed(*getPixel(x,y))>maximum){
                maximum=qRed(*getPixel(x,y));
            }
            if(qGreen(*getPixel(x,y))>maximum){
                maximum=qGreen(*getPixel(x,y));
            }
            if(qBlue(*getPixel(x,y))>maximum){
                maximum=qBlue(*getPixel(x,y));
            }

            redmax=maximum;
            redmin=minimum;
            greenmax=maximum;
            greenmin=minimum;
            bluemax=maximum;
            bluemin=minimum;
            */

        }
    }

    /*
    qDebug("Red min:%i max:%i",getMinColorValue(ImageAbstraction::red),getMaxColorValue(ImageAbstraction::red));
    qDebug("Green min:%i max:%i",getMinColorValue(ImageAbstraction::green),getMaxColorValue(ImageAbstraction::green));
    qDebug("Blue min:%i max:%i",getMinColorValue(ImageAbstraction::blue),getMaxColorValue(ImageAbstraction::blue));
    */

    /*/printing the array
    for(int count=0;count<256;count++){
        qDebug("red,%i,%i",count,colorcounterred[count]);
    }
    for(int count=0;count<256;count++){
        qDebug("green,%i,%i",count,colorcountergreen[count]);
    }
    for(int count=0;count<256;count++){
        qDebug("blue,%i,%i",count,colorcounterblue[count]);
    }
    /*/
}

double* ImageAbstraction::makeFilterGaussian(int dim, int sig){

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
        return kernel;

}
void ImageAbstraction::ApplyConvolution(int dim, int sig, char filter){
    double* kernel;
    switch (filter){
    case 'G':
        kernel = ImageAbstraction::makeFilterGaussian(dim,sig);
        break;
    case 'M':
        kernel = ImageAbstraction::makeMeanFilter(dim);
        break;
    default:
        break;

    }
    /*
    for (int i=0;i<dim;++i)
        for (int j=0;j<dim;++j)
            qDebug("%f", (double)(kernel[i*dim+j]));
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
               if ((jj > 0) && (jj <this->height() ) && (ii > 0) && (ii < this->width()))
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
           setPixel(j,i,newval[0],newval[1],newval[2]);
         }
       }
       free(kernel);
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
    //qDebug("%f CC",c);
    //qDebug("%f CC",m);
    //qDebug("%f CC",ye);
    if (c<k)
        k = c;
    if (m<k)
        k = m;
    if (ye<k)
        k = ye;
    qDebug("%f KK",k);
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
double* ImageAbstraction::makeMeanFilter(int dim){
    qDebug("MAKING MEAN FILTER");
    double* kernel = (double*)malloc(sizeof(double)*dim*dim);
    for (int i=0; i<dim; ++i)
            for (int j=0; j<dim; ++j)
                    kernel[i*dim+j] = 1.0/(dim*dim);
    return kernel;

}
