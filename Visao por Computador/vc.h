//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    VIS�O POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define VC_DEBUG
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)
#define MAX3(a, b, c) (a > b ? (a > c ? a : c) : (b > c ? b : c))
#define MIN3(a, b, c) (a < b ? (a < c ? a : c) : (b < c ? b : c))
#ifndef _TOOLS_H_
#define _TOOLS_H_




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct
{
	unsigned char *data;
	int width, height;
	int channels;	  // Bin�rio/Cinzentos=1; RGB=3
	int levels;		  // Bin�rio=1; Cinzentos [1,255]; RGB [1,255]
	int bytesperline; // width * channels
} IVC;

typedef struct
{
	int x, y, width, height; // Caixa Delimitadora (Bounding Box)
	int area;				 // area
	int xc, yc;				 // Centro-de-massa
	int perimeter;			 // Per�metro
	int label;				 // Etiqueta
} OVC;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROT�TIPOS DE funcoes
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// funcoes: ALOCAR E LIBERTAR UMA IMAGEM
IVC *vc_image_new(int width, int height, int channels, int levels);
IVC *vc_image_free(IVC *image);
int vc_gray_negative(IVC *srcdst);
// funcoes: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
IVC *vc_read_image(char *filename);
int vc_write_image(char *filename, IVC *image);
int vc_read_to_gray(IVC *filename, IVC *image);
int vc_gray_negative(IVC *srcdst);
int High_Threshold(IVC *src, IVC *dst, int Threshold);
int vc_binary_dilate(IVC *src, IVC *dst, int kernel);
int vc_etiquetagem(IVC *src, IVC *dst);

OVC *vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels);
int vc_binary_blob_info(IVC *src, OVC *blobs, int nblobs);
int vc_gray_histogram_show(IVC *src, IVC *dst);
int vc_gray_histogram_equalization(IVC* srcdst);
int vc_gray_to_binary(IVC* srcdst, int Threshold);
int vc_gray_to_binary_midpoint(IVC* src, IVC* dst);
int vc_gray_negative(IVC* srcdst);
int vc_read_to_gray(IVC* src, IVC* dst); 
int vc_rgb_negative(IVC* srcdst);
int vc_bgr_rgb(IVC* srcdst);
int vc_read_to_gray(IVC* src, IVC* dst);
int vc_gray_to_binary_global_mean(IVC* src, IVC* dst);
int vc_etiquetagem(IVC* src, IVC* dst);
int vc_rgb_to_gray(IVC* src, IVC* dst);
int vc_rgb_to_hsv1(IVC* src);
int vc_hsv_segmentation(IVC* src, IVC* dst, int hmin, int hmax, int smin,int smax, int vmin, int vmax);
OVC* vc_binary_blob_labelling(IVC* src, IVC* dst, int* nlabels);
int vc_binary_blob_info(IVC* src, OVC* blobs, int nblobs);
int vc_binary_erode(IVC* src, IVC* dst, int kernel);
int vc_binary_open(IVC* src, IVC* dst, int kernelErosion, int kernelDilation);
int vc_binary_close(IVC* src, IVC* dst, int kernelDilation, int kernelErosion);
int rgb_hsv(IVC* src, IVC* dst);
int vc_change_color(IVC* src, IVC* dst);
int px_mm(int px);

#endif