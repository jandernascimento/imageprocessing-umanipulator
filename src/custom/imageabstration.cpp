#include "imageabstration.h"
#include "math.h"

ImageAbstraction::ImageAbstraction(const QString &fileName, const char *format):QImage(fileName, format ){

    qDebug("Calling custom constructor");

    //ApplyFilterGreyScale();

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
    int red = color==ImageAbstraction::red?value:qRed(*pixel);
    int green = color==ImageAbstraction::green?value:qGreen(*pixel);
    int blue = color==ImageAbstraction::blue?value:qBlue(*pixel);
    *pixel = qRgba(red,green,blue,255);
    return pixel;

}

QRgb* ImageAbstraction::setPixel(int x, int y,int red, int green, int blue){
    setPixel(ImageAbstraction::red,x,y,red);
    setPixel(ImageAbstraction::green,x,y,green);
    return setPixel(ImageAbstraction::blue,x,y,blue);
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

            if(!this->isGrayscale()||true){
                setPixel(x,y,
                ApplyFilterContrastRule(red,x,y,newmin,newmax),
                ApplyFilterContrastRule(green,x,y,newmin,newmax),
                ApplyFilterContrastRule(blue,x,y,newmin,newmax));
            }else{
            }
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

    qDebug("Fusion called");

    int total=this->height()*this->width();
    int numbertoplot=percentage*total;
    int space=total/numbertoplot;
    int pcounter=0;

    for(int x=0;x<this->height();x++){
        for(int y=0;y<this->width();y++){

            if(y>fimage->width()) break;

            if(++pcounter==space){
                //qDebug("fundiu");
                QRgb *pix=getPixel(x,y);
                QRgb *pixext=fimage->getPixel(x,y);
                *pix=qRgba(qRed(*pixext),qGreen(*pixext),qBlue(*pixext),255);
                //*pix=qRgba(255,0,0,255);
                pcounter=0;
            }

        }
    }
}

void ImageAbstraction::UpdateColorRange(){

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

    qDebug("Red min:%i max:%i",getMinColorValue(ImageAbstraction::red),getMaxColorValue(ImageAbstraction::red));
    qDebug("Green min:%i max:%i",getMinColorValue(ImageAbstraction::green),getMaxColorValue(ImageAbstraction::green));
    qDebug("Blue min:%i max:%i",getMinColorValue(ImageAbstraction::blue),getMaxColorValue(ImageAbstraction::blue));

}
