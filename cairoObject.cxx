#include "cairoObject.h"

cairoObject::cairoObject(std::string filename, int x, int y): x(x), y(y){
	mainSurface = cairo_pdf_surface_create(filename.c_str(), 240, 80);
	mainContext = cairo_create(mainSurface);
}

cairoObject::~cairoObject(){	
	for(auto& page: pageContexts){
		cairo_destroy (page);
	}
	cairo_destroy(mainContext);

	
	for(auto& pageS: pageSurfaces){
		cairo_surface_destroy(pageS);
	}
	cairo_surface_destroy(mainSurface);
}

void cairoObject::addPage(int n){
	for(int j = 0; j < n; ++j){
        cairo_surface_t* newPageS = cairo_surface_create_similar( mainSurface, cairo_surface_get_content(mainSurface), x, y);
		pageSurfaces.push_back(newPageS);
        cairo_t* newPage = cairo_create(newPageS);
		pageContexts.push_back(newPage);
	}
}

cairo_t* cairoObject::getContext(size_t page){
	return pageContexts.at(page);
}

void cairoObject::draw(){
	for(auto& pageS: pageSurfaces){
		cairo_set_source_surface(mainContext, pageS, 0.0, 0.0);
		cairo_paint(mainContext);
		cairo_show_page(mainContext);
	}
}
 
