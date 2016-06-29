#ifndef cairoObject_h
#define cairoObject_h

#include <string>
#include <vector>

#include <cairo.h>
#include <cairo-pdf.h>

class cairoObject {

  public:
	cairoObject(std::string filename, int x, int y);
    ~cairoObject();
	void addPage(int n); 
	cairo_t* getContext(size_t page);
	void draw();

  private:
	cairo_surface_t* mainSurface;
	cairo_t* mainContext;

	std::vector<cairo_surface_t*> pageSurfaces;
	std::vector<cairo_t*> pageContexts;

	size_t pageNo;
	size_t x; 
	size_t y;
};

#endif
