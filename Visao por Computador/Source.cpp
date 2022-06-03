#include <iostream>
#include <string>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

extern "C" {
#include "vc.h"
}


/*
1px-----  0,264mm
x----------53

*/



int main(void) {
	// Video
	int calibre = -1;
	int blobAnterior = 10000;
	int raio;
	int areaAnterior = 0;
	int lar = 0;
	int nlaranjas = 1;
	char videofile[20] = "video.avi";
	cv::VideoCapture capture;
	struct
	{
		int width, height;
		int ntotalframes;
		int fps;
		int nframe;
	} video;
	// Outros
	std::string str;
	int key = 0;
	int nblobs;
	OVC* blobs;

	/* Leitura de video de um ficheiro */
	/* NOTA IMPORTANTE:
	O ficheiro video.avi dever? estar localizado no mesmo direct?rio que o ficheiro de c?digo fonte.
	*/
	capture.open(videofile);

	/* Em alternativa, abrir captura de video pela Webcam #0 */
	//capture.open(0, cv::CAP_DSHOW); // Pode-se utilizar apenas capture.open(0);

	/* Verifica se foi poss?vel abrir o ficheiro de video */
	if (!capture.isOpened())
	{
		std::cerr << "Erro ao abrir o ficheiro de video!\n";
		return 1;
	}

	/* N?mero total de frames no video */
	video.ntotalframes = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);
	/* Frame rate do video */
	video.fps = (int)capture.get(cv::CAP_PROP_FPS);
	/* Resolu??o do video */
	video.width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
	video.height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	/* Cria uma janela para exibir o video */
	cv::namedWindow("VC - VIDEO", cv::WINDOW_AUTOSIZE);

	cv::Mat frame;
	while (key != 'q') {
		/* Leitura de uma frame do video */
		capture.read(frame);

		/* Verifica se conseguiu ler a frame */
		if (frame.empty()) break;

		/* N?mero da frame a processar */
		video.nframe = (int)capture.get(cv::CAP_PROP_POS_FRAMES);

		/* Exemplo de inser??o texto na frame */
		str = std::string("RESOLUCAO: ").append(std::to_string(video.width)).append("x").append(std::to_string(video.height));
		cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("TOTAL DE FRAMES: ").append(std::to_string(video.ntotalframes));
		cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("FRAME RATE: ").append(std::to_string(video.fps));
		cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("N. DA FRAME: ").append(std::to_string(video.nframe));
		cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);


		str = std::string("Laranjas:").append(std::to_string(nlaranjas));
		cv::putText(frame, str, cv::Point(20, 200), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

			str = std::string("Caligre:").append(std::to_string(calibre));
			cv::putText(frame, str, cv::Point(20, 225), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		
		IVC* image = vc_image_new(video.width, video.height, 3, 255);
		IVC* image_hsv = vc_image_new(image->width, image->height, 3, 255);
		IVC* image_segmented = vc_image_new(image->width, image->height, 1, 255);
		IVC* image_erode = vc_image_new(image->width, image->height, 1, 255);
		IVC* image_dilate = vc_image_new(image->width, image->height, 1, 255);	
		IVC* image_blob = vc_image_new(image->width, image->height, 1, 255);
		IVC* image_changed = vc_image_new(image->width, image->height, 1, 255);
		memcpy(image->data, frame.data, video.width * video.height * 3);
		//segmentar cor de laranja
		vc_bgr_rgb(image);
		rgb_hsv(image, image_hsv);
		vc_hsv_segmentation(image_hsv, image_segmented,15,50,30,100,25,100);
		vc_binary_erode(image_segmented, image_erode, 3);
		vc_binary_dilate(image_erode, image_dilate,15);


		
		//segmentar preto e branco
/*
		IVC* image_segmented_toseg = vc_image_new(image->width, image->height, 1, 255);
		IVC* image_erode_toseg = vc_image_new(image->width, image->height, 1, 255);
		IVC* image_dilate_toseg = vc_image_new(image->width, image->height, 1, 255);
		IVC* image_blob_toseg = vc_image_new(image->width, image->height, 1, 255);
		IVC* image_changed_toseg = vc_image_new(image->width, image->height, 1, 255);



		vc_hsv_segmentation(image_segmented, image_segmented_toseg, 0, 360, 0, 20, 30 , 100);
		vc_binary_erode(image_segmented_toseg, image_erode_toseg, 3);
		vc_binary_dilate(image_erode_toseg, image_dilate_toseg, 10);
		vc_change_color(image_dilate_toseg, image_changed_toseg);

		*/

		//blobs = vc_binary_blob_labelling(image_changed_toseg, image_blob, &nblobs);
		
		blobs = vc_binary_blob_labelling(image_dilate, image_blob, &nblobs);
		vc_binary_blob_info(image_blob, blobs, nblobs);

		for (int i = 0; i < nblobs; i++)
		{
			
			if (blobs[i].area > 65000 &&  blobs[i].yc + 100 && blobs[i].yc > 100)
			{
				//Controlo de forma
				if ((blobs[i].yc + blobs[i].y) != (blobs[i].xc + blobs[i].x))
				{
					//verificar
				}



				if(blobs[i].area < 65000)
				{
					i++;
				}
				int x = blobs[i].xc - blobs[i].x;
				int y = blobs[i].yc - blobs[i].y;

				if (x > y)
				{
					raio = x;
				}
				else
				{
					raio = y;
				}

				printf("blobs are  %d\n", blobs[i].area);
				
				if (blobs[i].area < areaAnterior && i!=blobAnterior) {
					nlaranjas++;

					printf("laranjas %d\n", nlaranjas);
					
				}
				else
				{
					areaAnterior = blobs[i].area;

				}
				
				
				cv::circle(frame, cv::Point(blobs[i].xc, blobs[i].yc), raio, cv::Scalar(255, 0, 0), 4, 4, 0);
				auto nav = blobs[i].yc + raio + 20;
				str = std::string("Perimetro:").append(std::to_string(blobs[i].perimeter));
				putText(frame, str, cv::Point(blobs[i].xc, nav), cv::FONT_HERSHEY_SIMPLEX,1, cv::Scalar(255, 255, 0));
				str = std::string("Raio:").append(std::to_string(px_mm(blobs[i].perimeter) / (2 * 3.14)));

				putText(frame, str, cv::Point(blobs[i].xc, nav + 20),cv::FONT_HERSHEY_SIMPLEX,1, cv::Scalar(255, 255, 0));
				
			}

		};



		for (int i = 0; i < nblobs; i++)
		{

			printf("area %d \n", blobs[i].area);
			if (px_mm(blobs[i].perimeter) / (2 * 3.14)>=53)
			{
				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 53 && px_mm(blobs[i].perimeter) / (2 * 3.14) < 60)
				{
					calibre =13 ;
				}


				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 56 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 63)
				{
					calibre = 12;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 58 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 66)
				{
					calibre = 11;
				}


				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 60 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 68)
				{
					calibre = 10;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 62 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 70)
				{
					calibre = 9;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 64 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 73)
				{
					calibre = 8;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 67 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 76)
				{
					calibre = 7;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 70 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 80)
				{
					calibre = 6;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 73 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 84)
				{
					calibre = 5;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 77 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 88)
				{
					calibre = 4;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 81 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 92)
				{
					calibre = 3;
				}

				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 84 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 96)
				{
					calibre = 2;
				}
				
				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 87 && px_mm(blobs[i].perimeter) / (2 * 3.14) <= 100)
				{
					calibre = 1;
				}


				if (px_mm(blobs[i].perimeter) / (2 * 3.14) >= 100 )
				{
					calibre = 0;
				}

				//memcpy(frame.data, image_blob->data, video.width* video.height * 1);


			}
			putText(frame, str, cv::Point(20, 300), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 0));
			str = std::string("Raio:").append(std::to_string(px_mm(blobs[i].perimeter) / (2 * 3.14)));
		}
		
		vc_image_free(image);
		vc_image_free(image_hsv);
		vc_image_free(image_segmented);
		vc_image_free(image_erode);





		/* Exibe a frame */
		cv::imshow("VC - VIDEO", frame);

		/* Sai da aplica??o, se o utilizador premir a tecla 'q' */
		key = cv::waitKey(1);
	}

	/* Fecha a janela */
	cv::destroyWindow("VC - VIDEO");

	/* Fecha o ficheiro de video */
	capture.release();

	return 0;

}










