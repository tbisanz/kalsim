#include <iostream>
#include <random>

#include "objects.h"
#include "cairoObject.h"

#include <cairo.h>
#include <cairo-pdf.h>

struct rgb{
	float r, g, b;
};

static std::vector<rgb> rgbTriplets;

void propagate_and_draw_from_source(source& s, int no, std::vector<plane> planeVec, cairo_t* pageOne, cairo_t* pageTwo) { 

		static std::random_device rd;	
		static std::default_random_engine generator(rd());
		static std::uniform_int_distribution<int> colDist(0,rgbTriplets.size()-1);

		for(int i = 0; i < no; i++) {
			auto tr1 = s.getTrack();
			tr1.setPlaneVec(planeVec);

			rgb col = rgbTriplets.at(colDist(generator));

			cairo_set_source_rgb (pageOne, col.r, col.g, col.b);
			cairo_set_source_rgb (pageTwo, 1, 0, 0);
			cairo_set_line_width (pageOne, 0.1);
			cairo_set_line_width (pageTwo, 0.1);

			cairo_arc(pageOne, tr1.currPos.z, 40-tr1.currPos.x, 0.5, 0, 2 * 3.142);
			cairo_stroke_preserve(pageOne);
			
		//	while(tr1.propagate2next()){
			for(auto& hit: tr1.hits) {

				cairo_arc(pageOne, tr1.currPos.z, 40-tr1.currPos.x, 0.5, 0, 2 * 3.142);
				cairo_arc(pageTwo, tr1.currPos.z, 40-tr1.currPos.x, 0.5, 0, 2 * 3.142);
				cairo_stroke_preserve(pageOne);
				cairo_stroke(pageTwo);
		//	}
			cairo_stroke(pageOne);
		}
}


int main() {

		cairoObject painter("file2.pdf", 240, 80);
		painter.addPage(2);

		cairo_t* pageOne = painter.getContext(0);
		cairo_t* pageTwo = painter.getContext(1);

		std::vector<plane> planeVec;

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
		std::uniform_int_distribution<int> colDist(0,rgbTriplets.size()-1);

		int dice_roll = distribution(generator);

		for(int i = 1; i <= 10; i++) {
			plane pl;
			pl.pos.z = 20*i;
			pl.pos.x = distribution(generator)*0.2;
			pl.angle = distribution(generator);
			pl.size = 40;
			pl.radLen = 0;
			pl.ID = i;
			planeVec.push_back(pl);
		}

		cairo_set_source_rgb (pageOne, 0, 0, 0);
		cairo_set_source_rgb (pageTwo, 0, 0, 0);
		cairo_set_line_width (pageOne, 0.2);
		cairo_set_line_width (pageTwo, 0.2);

		for(auto& pl: planeVec) {
			float xOrig = pl.pos.x;
			float zOrig = pl.pos.z;
			float angle = pl.angle*PI/180;

			float deltaX = pl.size*std::cos(angle)/2.0;
			float deltaZ = pl.size*std::sin(angle)/2.0;

			cairo_move_to (pageOne, zOrig-deltaZ, 40-(xOrig-deltaX));
			cairo_move_to (pageTwo, zOrig-deltaZ, 40-(xOrig-deltaX));
			cairo_line_to (pageOne, zOrig+deltaZ, 40-(xOrig+deltaX));
			cairo_line_to (pageTwo, zOrig+deltaZ, 40-(xOrig+deltaX));
			cairo_stroke (pageOne);
			cairo_stroke (pageTwo);
		}

		dist<float> df1(0, 1);
		dist<float> df2(0, 2);
		source s1(7.0, 0.0, df1, df2);
		source s2(-7.0, 0.0, df1, df2);

		propagate_and_draw_from_source(s1, 7, planeVec, pageOne, pageTwo);
		propagate_and_draw_from_source(s2, 7, planeVec, pageOne, pageTwo);

		painter.draw();

	return 1;

}
