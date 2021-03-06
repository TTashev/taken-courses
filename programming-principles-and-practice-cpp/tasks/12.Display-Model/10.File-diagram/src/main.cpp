#include <math.h> 

#include "Simple_window.h"
#include "Graph.h"
#include "metrics.h"

using namespace std;

Rectangle * rectangles[RECTS_CNT];
Text * titles[RECTS_CNT * 2];
Line * links[RECTS_CNT * 2]; // max 2 links for every rectangle
Line * linksPointers[RECTS_CNT * 2 * 2]; // max 2 pointer parts per 2 links for every rectangle
int rectanglesIndex = 0;
int titlesIndex = 0;
int linksIndex = 0;
int linksPointersIndex = 0;

static void StyleRectangle(Rectangle & rect)
{
	rect.set_fill_color(Color::dark_green);
}

static void LinkRectangles(Simple_window & window,
						   int srcRectX,
						   int srcRectY,
						   int destRectsCnt,
						   int destRectX,
						   int destRectY)
{
	const double INTERSECTION_OFFSET = 10;
	const double ARROW_OFFSET = 3; 
	
	double srcX = srcRectX + (RECTS_WIDTH * 3 / 4);
	double srcY = srcRectY;
	double destX = destRectX + (RECTS_WIDTH * 3 / 4);
	double destY = destRectY + RECTS_HEIGHT + ((destRectsCnt - 1) * 5);
	links[linksIndex] = new Line(
								Point(srcX, srcY), 
								Point(destX, destY));

	links[linksIndex]->set_style(Line_style(Line_style::solid, 2));
	window.attach(*links[linksIndex++]);
	
	// Handle link Pointers (makes it look like an arrow)
	if (destX - srcX != 0)
	{
		double lineSlope = (destY - srcY) / (destX - srcX);
		double perpendicularLineSlope = -1/lineSlope;
		double sign = -1;
		if (lineSlope > 0)
		{
			sign = 1;
		}

		Point intersectionPoint(destX + (sign * INTERSECTION_OFFSET), 
								destY + (sign * INTERSECTION_OFFSET * lineSlope));
		Point leftPointPLine(intersectionPoint.x + (sign * ARROW_OFFSET), 
							 intersectionPoint.y + (sign * ARROW_OFFSET * perpendicularLineSlope));

		Point rightPointPLine(intersectionPoint.x - (sign * ARROW_OFFSET), 
							  intersectionPoint.y - (sign * ARROW_OFFSET * perpendicularLineSlope));
		
		linksPointers[linksPointersIndex] = new Line(Point(destX, destY),
													 leftPointPLine);
		
		linksPointers[linksPointersIndex + 1] = new Line(Point(destX, destY),
														 rightPointPLine);
		
		linksPointers[linksPointersIndex]->set_style(Line_style(Line_style::solid, 2));
		linksPointers[linksPointersIndex + 1]->set_style(Line_style(Line_style::solid, 2));

		window.attach(*linksPointers[linksPointersIndex]);
		window.attach(*linksPointers[linksPointersIndex + 1]);
		linksPointersIndex += 2;
	}
}

static void DrawTextInRectangle(Simple_window & window,
								int rectsCnt,
								int rectsStartX,
								int rectsStartY,
								const string & title,
								int rectsXOffset,
								int rectsYOffset,
								Color textColor)
{
	titles[titlesIndex] = 
		new Text(
			Point(rectsStartX + rectsXOffset, 
				  rectsStartY + rectsYOffset + (rectsCnt * 5)),
			title);

	titles[titlesIndex]->set_color(textColor);
	window.attach(*titles[titlesIndex++]);
}

static void DrawRectangleTitle(Simple_window & window,
							   int rectsCnt,
							   int rectsStartX,
							   int rectsStartY,
							   const string & title)
{
	DrawTextInRectangle(window, 
						rectsCnt,
						rectsStartX, 
						rectsStartY, 
						title, 
						0, 
						-(RECTS_HEIGHT / 4),
						Color::black);
}

static void DrawRectangleContent(Simple_window & window,
							     int rectsCnt,
								 int rectsStartX,
							     int rectsStartY,
							     const string & title)
{
	DrawTextInRectangle(window, 
						rectsCnt,
						rectsStartX, 
						rectsStartY, 
						title, 
						(RECTS_WIDTH / 6), 
						(RECTS_HEIGHT / 4),
						Color::yellow);
}

static void DrawRectangles(Simple_window & window, 
						  int rectsCnt, 
					      int rectsStartX,
						  int rectsStartY,
						  int rectsWidth,
						  int rectsHeight)
{
	for (int index = 0; index < rectsCnt; ++index)
	{
		rectangles[rectanglesIndex] = 
			new Rectangle(
				Point(rectsStartX + (index * 5), 
				      rectsStartY + (index * 5)), 
				Point(rectsStartX + rectsWidth  + (index * 5), 
				      rectsStartY + rectsHeight + (index * 5)));
	
		StyleRectangle(*rectangles[rectanglesIndex]);
		window.attach(*rectangles[rectanglesIndex++]);
	}
}

static void DrawComponents(Simple_window & window)
{
	// Draw FLTK headers
	DrawRectangles(window, 
				   FLTK_HEADERS_CNT, 
	               FLTK_HEADERS_X,
				   FLTK_HEADERS_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);

	DrawRectangleTitle(window, 
				   	   FLTK_HEADERS_CNT, 
					   FLTK_HEADERS_X, 
					   FLTK_HEADERS_Y, 
					   FLTK_HEADERS_TITLE);

	DrawRectangleContent(window, 
				   	     FLTK_HEADERS_CNT, 
					     FLTK_HEADERS_X, 
					     FLTK_HEADERS_Y, 
					     FLTK_HEADERS_CONTENT);

	// Draw FLTK code
	DrawRectangles(window, 
				   FLTK_CODE_CNT, 
	               FLTK_CODE_X,
				   FLTK_CODE_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);

	DrawRectangleTitle(window, 
				   	   FLTK_CODE_CNT, 
					   FLTK_CODE_X, 
					   FLTK_CODE_Y, 
					   FLTK_CODE_TITLE);
	
	DrawRectangleContent(window, 
				   	     FLTK_CODE_CNT, 
					     FLTK_CODE_X, 
					     FLTK_CODE_Y, 
					     FLTK_CODE_CONTENT);
	
	// Draw Window.h
	DrawRectangles(window, 
				   WINDOW_H_CNT, 
	               WINDOW_H_X,
				   WINDOW_H_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);

	DrawRectangleTitle(window, 
				   	   WINDOW_H_CNT, 
					   WINDOW_H_X, 
					   WINDOW_H_Y, 
					   WINDOW_H_TITLE);
	
	DrawRectangleContent(window, 
				   	     WINDOW_H_CNT, 
					     WINDOW_H_X, 
					     WINDOW_H_Y, 
					     WINDOW_H_CONTENT);
	
	// Draw Point.h
	DrawRectangles(window, 
				   POINT_H_CNT, 
	               POINT_H_X,
				   POINT_H_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);
	
	DrawRectangleTitle(window, 
				   	   POINT_H_CNT, 
					   POINT_H_X, 
					   POINT_H_Y, 
					   POINT_H_TITLE);
	
	DrawRectangleContent(window, 
				   	     POINT_H_CNT, 
					     POINT_H_X, 
					     POINT_H_Y, 
					     POINT_H_CONTENT);
	
	// Draw Graph.h
	DrawRectangles(window, 
				   GRAPH_H_CNT, 
	               GRAPH_H_X,
				   GRAPH_H_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);

	DrawRectangleTitle(window, 
				   	   GRAPH_H_CNT, 
					   GRAPH_H_X, 
					   GRAPH_H_Y, 
					   GRAPH_H_TITLE);
	
	DrawRectangleContent(window, 
				   	     GRAPH_H_CNT, 
					     GRAPH_H_X, 
					     GRAPH_H_Y, 
					     GRAPH_H_CONTENT);
	
	// Draw Window.cpp
	DrawRectangles(window, 
				   WINDOW_CPP_CNT, 
	               WINDOW_CPP_X,
				   WINDOW_CPP_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);
	
	DrawRectangleTitle(window, 
				   	   WINDOW_CPP_CNT, 
					   WINDOW_CPP_X, 
					   WINDOW_CPP_Y, 
					   WINDOW_CPP_TITLE);
	
	DrawRectangleContent(window, 
				   	     WINDOW_CPP_CNT, 
					     WINDOW_CPP_X, 
					     WINDOW_CPP_Y, 
					     WINDOW_CPP_CONTENT);
	
	// Draw GUI.h
	DrawRectangles(window, 
				   GUI_H_CNT, 
	               GUI_H_X,
				   GUI_H_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);
	
	DrawRectangleTitle(window, 
				   	   GUI_H_CNT, 
					   GUI_H_X, 
					   GUI_H_Y, 
					   GUI_H_TITLE);
	
	DrawRectangleContent(window, 
				   	     GUI_H_CNT, 
					     GUI_H_X, 
					     GUI_H_Y, 
					     GUI_H_CONTENT);
	
	// Draw Graph.cpp
	DrawRectangles(window, 
				   GRAPH_CPP_CNT, 
	               GRAPH_CPP_X,
				   GRAPH_CPP_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);
	
	DrawRectangleTitle(window, 
				   	   GRAPH_CPP_CNT, 
					   GRAPH_CPP_X, 
					   GRAPH_CPP_Y, 
					   GRAPH_CPP_TITLE);
	
	DrawRectangleContent(window, 
				   	     GRAPH_CPP_CNT, 
					     GRAPH_CPP_X, 
					     GRAPH_CPP_Y, 
					     GRAPH_CPP_CONTENT);
	
	// Draw Simple_window.h
	DrawRectangles(window, 
				   SIMPLE_WINDOW_H_CNT, 
	               SIMPLE_WINDOW_H_X,
				   SIMPLE_WINDOW_H_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);

	DrawRectangleTitle(window, 
				   	   SIMPLE_WINDOW_H_CNT, 
					   SIMPLE_WINDOW_H_X, 
					   SIMPLE_WINDOW_H_Y, 
					   SIMPLE_WINDOW_H_TITLE);
	
	DrawRectangleContent(window, 
				   	     SIMPLE_WINDOW_H_CNT, 
					     SIMPLE_WINDOW_H_X, 
					     SIMPLE_WINDOW_H_Y, 
					     SIMPLE_WINDOW_H_CONTENT);
	
	// Draw GUI.cpp
	DrawRectangles(window, 
				   GUI_CPP_CNT, 
	               GUI_CPP_X,
				   GUI_CPP_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);
	
	DrawRectangleTitle(window, 
				   	   GUI_CPP_CNT, 
					   GUI_CPP_X, 
					   GUI_CPP_Y, 
					   GUI_CPP_TITLE);
	
	DrawRectangleContent(window, 
				   	     GUI_CPP_CNT, 
					     GUI_CPP_X, 
					     GUI_CPP_Y, 
					     GUI_CPP_CONTENT);
	
	LinkRectangles(window,
				   GUI_CPP_X,
				   GUI_CPP_Y,
				   GUI_H_CNT,
				   GUI_H_X,
				   GUI_H_Y);
	
	// Draw chapter12.cpp
	DrawRectangles(window, 
				   CHAPTER_12_CPP_CNT, 
	               CHAPTER_12_CPP_X,
				   CHAPTER_12_CPP_Y,
				   RECTS_WIDTH,
				   RECTS_HEIGHT);
	
	DrawRectangleTitle(window, 
				   	   CHAPTER_12_CPP_CNT, 
					   CHAPTER_12_CPP_X, 
					   CHAPTER_12_CPP_Y, 
					   CHAPTER_12_CPP_TITLE);
	
	DrawRectangleContent(window, 
				   	   CHAPTER_12_CPP_CNT, 
					   CHAPTER_12_CPP_X, 
					   CHAPTER_12_CPP_Y, 
					   CHAPTER_12_CPP_CONTENT);
	
	// Draw links
	LinkRectangles(window,
				   FLTK_CODE_X,
				   FLTK_CODE_Y,
				   FLTK_HEADERS_CNT,
				   FLTK_HEADERS_X,
				   FLTK_HEADERS_Y);
	
	LinkRectangles(window,
				   WINDOW_H_X,
				   WINDOW_H_Y,
				   FLTK_HEADERS_CNT,
				   FLTK_HEADERS_X,
				   FLTK_HEADERS_Y);
	
	LinkRectangles(window,
				   WINDOW_H_X,
				   WINDOW_H_Y,
				   POINT_H_CNT,
				   POINT_H_X,
				   POINT_H_Y);
	
	LinkRectangles(window,
				   GRAPH_H_X,
				   GRAPH_H_Y,
				   FLTK_HEADERS_CNT,
				   FLTK_HEADERS_X,
				   FLTK_HEADERS_Y);
	
	LinkRectangles(window,
				   GRAPH_H_X,
				   GRAPH_H_Y,
				   POINT_H_CNT,
				   POINT_H_X,
				   POINT_H_Y);

	LinkRectangles(window,
				   WINDOW_CPP_X,
				   WINDOW_CPP_Y,
				   WINDOW_H_CNT,
				   WINDOW_H_X,
				   WINDOW_H_Y);
	
	LinkRectangles(window,
				   GUI_H_X,
				   GUI_H_Y,
				   WINDOW_H_CNT,
				   WINDOW_H_X,
				   WINDOW_H_Y);
	
	LinkRectangles(window,
				   GUI_H_X,
				   GUI_H_Y,
				   FLTK_HEADERS_CNT,
				   FLTK_HEADERS_X,
				   FLTK_HEADERS_Y);
	
	LinkRectangles(window,
				   GRAPH_CPP_X,
				   GRAPH_CPP_Y,
				   GRAPH_H_CNT,
				   GRAPH_H_X,
				   GRAPH_H_Y);
	
	LinkRectangles(window,
				   SIMPLE_WINDOW_H_X,
				   SIMPLE_WINDOW_H_Y,
				   WINDOW_H_CNT,
				   WINDOW_H_X,
				   WINDOW_H_Y);
	
	LinkRectangles(window,
				   SIMPLE_WINDOW_H_X,
				   SIMPLE_WINDOW_H_Y,
				   GUI_H_CNT,
				   GUI_H_X,
				   GUI_H_Y);
	
	LinkRectangles(window,
				   CHAPTER_12_CPP_X,
				   CHAPTER_12_CPP_Y,
				   GRAPH_H_CNT,
				   GRAPH_H_X,
				   GRAPH_H_Y);
	
	LinkRectangles(window,
				   CHAPTER_12_CPP_X,
				   CHAPTER_12_CPP_Y,
				   SIMPLE_WINDOW_H_CNT,
				   SIMPLE_WINDOW_H_X,
				   SIMPLE_WINDOW_H_Y);
}

void DestroyResources()
{
	int index;
	for (index = 0; index < rectanglesIndex; ++index)
	{
		delete rectangles[index];
	}

	for (index = 0; index < titlesIndex; ++index)
	{
		delete titles[index];
	}

	for (index = 0; index < linksIndex; ++index)
	{
		delete links[index];
	}

	for (index = 0; index < linksPointersIndex; ++index)
	{
		delete linksPointers[index];
	}
}

int main()
{
	using namespace Graph_lib;

	Point tl(100, 100);
	Simple_window window(tl, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

	DrawComponents(window);

	window.wait_for_button();
	DestroyResources();
}
