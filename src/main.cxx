#include <iostream>
#include <random>

#include "track.h"
#include "plane.h"
#include "source.h"
#include "dist.h"

#include "cairoObject.h"

#include <cairo.h>
#include <cairo-pdf.h>

struct rgb{
	float r, g, b;
};

static std::vector<rgb> rgbTriplets;
const int XPDFSIZE = 240;
const int YPDFSIZE = 80;


std::vector<track> propagate_and_draw_from_source(source& s, int no, std::vector<plane> planeVec, cairo_t* pageOne, cairo_t* pageTwo, cairo_t* pageThree) { 

		static std::random_device rd;	
		static std::default_random_engine generator(rd());
		static std::uniform_int_distribution<int> colDist(0,rgbTriplets.size()-1);

		std::vector<track> tracks;

		for(int i = 0; i < no; i++) {
			auto tr1 = s.getTrack();
			tr1.setPlaneVec(planeVec);
			tr1.propagateFully();
			tracks.push_back(tr1);

			rgb col = rgbTriplets.at(colDist(generator));

			cairo_set_source_rgb (pageOne, col.r, col.g, col.b);
			cairo_set_source_rgb (pageThree, col.r, col.g, col.b);
			cairo_set_source_rgb (pageTwo, 1, 0, 0);
			cairo_set_line_width (pageOne, 0.1);
			cairo_set_line_width (pageTwo, 0.1);
			cairo_set_line_width (pageThree, 0.1);

			for(auto& hit: tr1.hits) {
				cairo_arc(pageOne, hit.z, YPDFSIZE/2.0-hit.x, 0.5, 0, 2 * 3.142);
				cairo_arc(pageTwo, hit.z, YPDFSIZE/2.0-hit.x, 0.5, 0, 2 * 3.142);
				if(tr1.allPlaneHit){
					cairo_arc(pageThree, hit.z, YPDFSIZE/2.0-hit.x, 0.5, 0, 2 * 3.142);
					cairo_stroke_preserve(pageThree);
				}
				cairo_stroke_preserve(pageOne);
				cairo_stroke(pageTwo);
			}
			cairo_stroke(pageOne);
			if(tr1.allPlaneHit) cairo_stroke(pageThree);
		}

		return tracks;
}

void draw_planes(std::vector<plane> const& planeVec, cairo_t* cr) {
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, 0.2);

	for(auto& pl: planeVec) {
		float xOrig = pl.pos.x;
		float zOrig = pl.pos.z;
		float angle = pl.angle*PI/180;

		float deltaX = pl.size*std::cos(angle)/2.0;
		float deltaZ = pl.size*std::sin(angle)/2.0;

		cairo_move_to(cr, zOrig-deltaZ, YPDFSIZE/2.0-(xOrig-deltaX));
		cairo_line_to(cr, zOrig+deltaZ, YPDFSIZE/2.0-(xOrig+deltaX));
		cairo_stroke(cr);
	}
} 

int main() {
		cairoObject painter("file2.pdf", XPDFSIZE, YPDFSIZE);
		painter.addPage(3);

		cairo_t* pageOne = painter.getContext(0);
		cairo_t* pageTwo = painter.getContext(1);
		cairo_t* pageThree = painter.getContext(2);

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

		int dice_roll = distribution(generator);

		size_t noPlanes = 10;
		for(int i = 1; i <= noPlanes; i++) {
			plane pl;
			pl.pos.z = XPDFSIZE/(noPlanes+2.0)*i;
			pl.pos.x = distribution(generator)*0.2;
			pl.angle = distribution(generator);
			pl.size = YPDFSIZE/2.0;
			pl.radLen = 0;
			pl.ID = i;
			planeVec.push_back(pl);
		}

		draw_planes(planeVec, pageOne);
		draw_planes(planeVec, pageTwo);
		draw_planes(planeVec, pageThree);

		dist<float> df1(0, 5);
		dist<float> df2(0, 5);
		source s1(0.0, 0.0, df1, df2);
		source s2(0.0, 0.0, df1, df2);

		auto tracks = propagate_and_draw_from_source(s1, 20, planeVec, pageOne, pageTwo, pageThree);
//		propagate_and_draw_from_source(s2, 7, planeVec, pageOne, pageTwo, pageThree);

		painter.draw();

	return 1;

}
