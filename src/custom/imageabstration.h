#include <QImage>
#ifndef IMAGEABSTRATION_H
#define IMAGEABSTRATION_H

class ImageAbstraction : public QImage
{
     public:
          enum ecolor {red, green, blue};
          enum ecolorcmyk {cyan, magenta, yellow, black};
          explicit ImageAbstraction(const QString &fileName, const char *format = 0);
          explicit ImageAbstraction(const QSize &size, Format format);
          explicit ImageAbstraction(const QImage &im);
          explicit ImageAbstraction(const ImageAbstraction &im);
          /** Pixels get/set **/
          QRgb* getPixel(int x, int y);
          int getPixelColorIntensity(enum ecolor color,int x, int y);
          QRgb* setPixel(enum ecolor color, int x, int y,int value);
          QRgb* setPixel(int x, int y,int red, int green, int blue);
          ImageAbstraction* scRemoveLine(int* matrix,int total_columns);
          ImageAbstraction* scInsertLine(int* seam,int* red_level,int* green_level,int* blue_level, ImageAbstraction * ia);

          /** Util  methods **/
          void UpdateColorRange(void);
          ImageAbstraction* copy(void);
          int getMinColorValue(enum ecolor color);
          int getMaxColorValue(enum ecolor color);
          int getColorCounter(enum ecolor color,int level);
          int getMaxNumberColor(void);
          ImageAbstraction* transposeLeftImage();
          ImageAbstraction* transposeRightImage();

          /** Filter and masks **/
          void ApplyFilterGreyScale();
          void ApplyFilterContrast(int min,int max);
          void ApplyFilterFusion(ImageAbstraction *fimage,float percentage,int posx,int posy);
          ImageAbstraction* ApplyScale(float xpercentage,float ypercentage);
          void makeFilterGaussian(int dim, double sig);
          void makeLoG(int dim, double sig);
          int findMax(int* array, int len);
          int findMin(int* array, int len);
          void minMax(int* oldArr, int oldMin, int oldMax, int newMin, int newMax, int len);
          void minMaxDouble(double* oldArr, double oldMin, double oldMax, double newMin, double newMax, int len);
          void makeGradFilterX(int dim);
          void makeGradFilterY(int dim);
          void makeLaplacianFilter(int dim);
          int ApplyConvolution(int dim, double* kernel);
          int RGB2CMYK(int x, int y, enum ecolorcmyk color);
          ImageAbstraction* ApplyCrop(int startx,int starty,int endx,int endy);
          void makeMeanFilter(int dim);
          void applyGradConvolution(char ax);
          void makeCustomKernel(int dim);
          double getMean();
          double getStd();
          int ApplyConvolutionLaplacian(int dim, double* kernel);
          ImageAbstraction* ApplyGradientMagnitude();

          /** Intelligent Resizing **/
          ImageAbstraction* applySeamCarving(float width,float height);

    private:
          int redmax;
          int redmin;
          int greenmax;
          int greenmin;
          int bluemax;
          int bluemin;
          int colorcounterred[256];
          int colorcountergreen[256];
          int colorcounterblue[256];
          bool isGreyScale;
          int ApplyFilterContrastRule(enum ecolor color,int x,int y,int min,int max);

          /** Intelligent Resizing **/
          ImageAbstraction* decreaseImage(int n_paths,int * energy_matrix,int * path,int * removal_paths,int * removal_paths_red,int * removal_paths_green,int * removal_paths_blue,ImageAbstraction* ia);
          void createEnergyMatrix(int * energy_matrix,ImageAbstraction *ia);
          int findMinValue(int value1,int value2, int value3);
          int findColumnMinValue(int * energy_matrix,int lin,int prev_col,int col,int next_col,ImageAbstraction *ia);
          int findMinimunValueLastLine(int * energy_matrix,ImageAbstraction *ia);
          void calculatePrevAndNextColumn(int * prev_column,int * next_column,int col_min_value, ImageAbstraction *ia);
          void findPath(int * energy_matrix,int * path,int * removal_paths,int * removal_paths_red,int * removal_paths_green,int * removal_paths_blue,int id_path,int n_paths, ImageAbstraction *ia);
          void printMatrix(int * matrix,int n_lin, int n_col);
          void saveMatrixInFile(char * nome_file,int * matrix,int n_cols,int n_lines);
          ImageAbstraction* resizeImage(int n_paths,ImageAbstraction* ia,int increase_image);
};

#endif // IMAGEABSTRATION_H
