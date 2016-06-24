#include <iostream>
#include <random>

#include "objects.h"

#include <cairo.h>
#include <cairo-pdf.h>
struct rgb{
	float r, g, b;
};

int main() {

        cairo_surface_t *surface = cairo_pdf_surface_create ("outFile.pdf", 240, 80);
        cairo_surface_t *surface2 = cairo_pdf_surface_create ("outFile2.pdf", 240, 80);
        cairo_t *cr = cairo_create (surface);
        cairo_t *cr2 = cairo_create (surface2);

		std::vector<plane> planeVec;
		
		std::vector<rgb> rgbTriplets;

		for(int r=0; r<=5; r++){
			for(int g=0; g<=5; g++){
				for(int b=0; b<=5; b++){
					rgb res;
					res.r = 0.2*r;
					res.g = 0.2*g;
					res.b = 0.2*b;
					rgbTriplets.push_back(res);
				}
			}
		}

		std::random_device rd;	
		std::default_random_engine generator(rd());
		std::uniform_int_distribution<int> distribution(-15,15);

		std::uniform_int_distribution<int> colDist(0,rgbTriplets.size());


		int dice_roll = distribution(generator);  // generates number in the range 1..6

		for(int i = 1; i <= 10; i++) {
			plane pl;
			pl.posZ = 20*i;
			pl.posX = distribution(generator)*0.2;
			pl.angle = distribution(generator);
			pl.size = 40;
			pl.radLen = 0;
			pl.ID = i;
			planeVec.push_back(pl);
		}

		cairo_set_source_rgb (cr, 0, 0, 0);
		cairo_set_source_rgb (cr2, 0, 0, 0);
		cairo_set_line_width (cr, 0.2);
		cairo_set_line_width (cr2, 0.2);

		for(auto& pl: planeVec) {
			float xOrig = pl.posX;
			float zOrig = pl.posZ;
			float angle = pl.angle*PI/180;

			float deltaX = pl.size*std::cos(angle)/2.0;
			float deltaZ = pl.size*std::sin(angle)/2.0;

			cairo_move_to (cr, zOrig-deltaZ, 40-(xOrig-deltaX));
			cairo_move_to (cr2, zOrig-deltaZ, 40-(xOrig-deltaX));
			cairo_line_to (cr, zOrig+deltaZ, 40-(xOrig+deltaX));
			cairo_line_to (cr2, zOrig+deltaZ, 40-(xOrig+deltaX));
			cairo_stroke (cr);
			cairo_stroke (cr2);
		}

		dist<float> df1(0, 5);
		dist<float> df2(0, 4);
		source s1(0.0, 0.0, df1, df2);

		for(int i = 0; i < 10; i++) {
			auto tr1 = s1.getTrack();
			tr1.setPlaneVec(planeVec);

			rgb col = rgbTriplets.at(colDist(generator));

			cairo_set_source_rgb (cr, col.r, col.g, col.b);
			cairo_set_source_rgb (cr2, 1, 0, 0);
			cairo_set_line_width (cr, 0.1);
			cairo_set_line_width (cr2, 0.1);

/*
			cairo_move_to (cr, tr1.posZ, tr1.posX+40);
			cairo_rel_line_to (cr, 55*std::cos(tr1.angle*PI/180), 55*std::sin(tr1.angle*PI/180));
			cairo_stroke (cr);
*/

			while(tr1.propagate()){
		//		cairo_set_line_width(cr, 0.2);  
		//		cairo_set_source_rgb(cr, 0.69, 0.19, 0);
				cairo_arc(cr, tr1.posZ, 40-tr1.posX, 1, 0, 2 * 3.142);
				cairo_arc(cr2, tr1.posZ, 40-tr1.posX, 1, 0, 2 * 3.142);
				cairo_stroke_preserve(cr);
				cairo_stroke(cr2);
			}
			cairo_stroke(cr);
		}

        //cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        //cairo_set_font_size (cr, 32.0);
        //cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
        //cairo_move_to (cr, 10.0, 50.0);
        //cairo_show_text (cr, "Hello, world");
	/*	
		cairo_set_source_rgb (cr, 0, 1, 0);
		for(int i = 1; i <= 10; i++) {
			cairo_move_to (cr, 20*i, 10);
			cairo_line_to (cr, 20, 30);
			cairo_rel_line_to (cr, 0, 60);	
			cairo_stroke (cr);
		}
		
		cairo_set_line_width(cr, 1);  
		cairo_set_source_rgb(cr, 0.69, 0.19, 0);
		cairo_arc(cr, 20, 40, 1, 0, 2 * 3.142);
		cairo_stroke_preserve(cr);
	*/	

        cairo_destroy (cr);
        cairo_destroy (cr2);
        //cairo_surface_write_to_png (surface, "hello.png");
        cairo_surface_destroy (surface);
        cairo_surface_destroy (surface2);

		//plane pl1;
		//distribution<float> df1 (100);
		//std::cout << "test: " << df1.getRandomNumber() << std::endl;

	return 1;

}
