/****************************************************************
	FILE:			absolute.cpp
	AUTHOR:			Tommy Deng

	NOTES:			Colour values (specifically the greyscale
					ones?) of the exported bitmap can be wrong.
					It seems that they become full white.

					SCALE done poorly. Should've applied it
					directly to the variables after declaration/
					initialization, ie:
					var = var * SCALE;
					var *= SCALE; (?)
****************************************************************/

#include "CImg.h"
#include <ctime>
#include <sstream>
#include <iostream>

using namespace cimg_library;
using namespace std;

int main()
{

	// https://www.epochconverter.com/
	// Friday, March 26, 1999 12:00:00 AM GMT-04:00
	const time_t BIRTHDATE = 922420800;
	// birthday offset from Epoch time (time from 1970-01-01T00:00:00Z to 1970-03-26)
	const time_t EPOCH_OFFSET = 7257600;

	string username = "Sttic";

	// seconds since 01-Jan-1970;
	time_t t = time(0);

	time_t age_sec = t - BIRTHDATE;
	int age_week = (t - BIRTHDATE) / (60 * 60 * 24 * 7);
	int offset = 7257600 / (60 * 60 * 24 * 7);	// exactly 12 with my birthday

	cout << age_week << " weeks lived" << endl;

	//	*************************************************************

	const int WEEKS = 52;
	const int YEARS = 80;

	const int BLOCK_SIZE = 1;
	const int SPACE = 1;

	const int PAD_X = 4;
	const int PAD_Y = 4;

	// top battery lead (+'ve)
	const int LEAD_PAD_Y = 8;
	const int LEAD_WIDTH = 37;	// third of width

	const int SCALE = 1;

	cout << YEARS * WEEKS - age_week << " expected weeks left" << endl;

	//	*************************************************************

	int width = ((WEEKS * BLOCK_SIZE) + ((WEEKS - 1) * SPACE) + (2 * PAD_X)) * SCALE;
	int height = ((YEARS * BLOCK_SIZE) + ((YEARS - 1) * SPACE) + (2 * PAD_Y) + LEAD_PAD_Y) * SCALE;

	cout << width << "x" << height << " pixels" << endl;

	//	*************************************************************

	const unsigned char black[] = { 0,0,0 };
	const unsigned char grey_dark[] = { 17,17,17 };			// was previously 32,32,32
	const unsigned char grey_light[] = { 128,128,128 };
	const unsigned char white[] = { 255,255,255 };
	const unsigned char white_off[] = { 247,247,247 };

	const unsigned char red[] = { 255,0,0 };
	const unsigned char green[] = { 0,255,0 };
	const unsigned char blue[] = { 0,0,255 };

	CImg<unsigned char> img(width, height, 1, 3);

	// draw border
	//img.fill(247);
	//int border = BLOCK_SIZE * SCALE;
	//img.draw_rectangle(border, border + LEAD_PAD_Y * SCALE, width - (border + 1), height - (border + 1), black);

	// draw border
	img.fill(0);
	int border = BLOCK_SIZE * SCALE;
	int lead_pad = LEAD_PAD_Y * SCALE;
	int lead_padX = LEAD_WIDTH * SCALE;

	// horizontal borders
	img.draw_rectangle(0, lead_pad, width, lead_pad + border - 1, white_off);
	img.draw_rectangle(0, height - border, width, height, white_off);

	// vertical borders
	img.draw_rectangle(0, lead_pad, border - 1, height, white_off);
	img.draw_rectangle(width - border, lead_pad, width, height, white_off);

	// draw lead
	img.draw_rectangle(lead_padX, 0, 2 * lead_padX - 1, lead_pad, white_off);

	//	*************************************************************

	int h, w;
	int count = 0;
	const bool SHOW_LABELS = false;

	for (int Y = 0; Y < YEARS; Y++)
	{
		h = (PAD_Y + Y * (BLOCK_SIZE + SPACE) + LEAD_PAD_Y) * SCALE;

		for (int X = 0; X < WEEKS; X++)
		{
			// offset for birthday
			if (offset == 0)
			{
				w = (PAD_X + X * (BLOCK_SIZE + SPACE)) * SCALE;

				// past/present/future colour
				if (count + 1 < age_week)		// past
					img.draw_rectangle(w, h, w + BLOCK_SIZE * SCALE - 1, h + BLOCK_SIZE * SCALE - 1, grey_dark);
				else if (count + 1 > age_week)	// future
					img.draw_rectangle(w, h, w + BLOCK_SIZE * SCALE - 1, h + BLOCK_SIZE * SCALE - 1, white_off);
				else							// present
					img.draw_rectangle(w, h, w + BLOCK_SIZE * SCALE - 1, h + BLOCK_SIZE * SCALE - 1, grey_dark);

				count++;

				// week labels
				if (Y == 0 && SHOW_LABELS)
				{
					stringstream ss;
					ss << X;
					img.draw_text(w, PAD_Y * SCALE / 2, ss.str().c_str(), white);
				}
			}
			else
			{
				offset--;
			}
		}


		// year labels
		if (Y % 5 == 0 && SHOW_LABELS)
		{
			stringstream ss;
			ss << Y;
			img.draw_text(PAD_X * SCALE / 2, h, ss.str().c_str(), white);
		}
	}


	//	*************************************************************

	stringstream ss;
	ss << username << ".bmp";
	img.normalize(0, 255);
	img.save(ss.str().c_str());

	img.display("Death Clock");
}