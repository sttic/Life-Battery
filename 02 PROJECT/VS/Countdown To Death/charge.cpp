/****************************************************************
	FILE:			charge.cpp
	AUTHOR:			Tommy Deng

	NOTES:			Colour values (specifically the greyscale
					ones?) of the exported bitmap can be wrong.
					It seems that they become full white.

					Horribly inefficient - currently it redraws
					the entire canvas instead of changing by
					one pixel at a time.
****************************************************************/

#include "CImg.h"
#include <ctime>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace cimg_library;
using namespace std;

int main()
{
	time_t time_start = time(0);

//	*************************************************************

	const int WEEKS = 52;
	const int YEARS = 85;

	const int BLOCK_SIZE = 1;
	const int SPACE = 1;

	const int PAD_X = 4;
	const int PAD_Y = 4;

	// top battery lead (+'ve)
	const int LEAD_PAD_Y = 8;
	const int LEAD_WIDTH = 37;	// third of width

	const int SCALE = 1;

//	*************************************************************

	int width = ((WEEKS * BLOCK_SIZE) + ((WEEKS - 1) * SPACE) + (2 * PAD_X)) * SCALE;
	int height = ((YEARS * BLOCK_SIZE) + ((YEARS - 1) * SPACE) + (2 * PAD_Y) + LEAD_PAD_Y) * SCALE;

	cout << "size: " << width << " x " << height << " pixels" << endl;
	cout << "scale: " << SCALE << endl;
	cout << "generating frames ... this may take a few minutes." << endl;

//	*************************************************************

	const unsigned char black[] = { 0,0,0 };
	const unsigned char grey_dark[] = { 32,32,32 };
	const unsigned char grey_light[] = { 128,128,128 };
	const unsigned char white[] = { 255,255,255 };
	const unsigned char white_off[] = { 247,247,247 };

	const unsigned char red[] = { 255,0,0 };
	const unsigned char green[] = { 0,255,0 };
	const unsigned char blue[] = { 0,0,255 };

	CImg<unsigned char> img(width, height, 1, 3);

//	*************************************************************
	
	int age_week;
	int total_weeks = YEARS * WEEKS;
	const bool SHOW_LABELS = false;

	for (age_week = 0; age_week <= total_weeks; age_week++)
	{
		int h = 0;
		int w = 0;
		int count = 0;

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


		for (int Y = 0; Y < YEARS; Y++)
		{
			h = (PAD_Y + Y * (BLOCK_SIZE + SPACE) + LEAD_PAD_Y) * SCALE;

			for (int X = 0; X < WEEKS; X++)
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
					ss << X + 1;
					img.draw_text(w, ((PAD_Y + LEAD_PAD_Y) + LEAD_PAD_Y) * SCALE / 2, ss.str().c_str(), white);
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

		stringstream frameName;
		frameName << "charge_";
		
		if (YEARS < 100) frameName << "0";
		frameName << YEARS;
			
		frameName << "x" << SCALE << "_";

		if (age_week < 10) frameName << "0000";
		else if (age_week < 100) frameName << "000";
		else if (age_week < 1000) frameName << "00";
		else if (age_week < 10000) frameName << "0";
		frameName << age_week;
			
		frameName << ".bmp";
		img.normalize(0, 255);
		img.save(frameName.str().c_str());

	}

	time_t time_end = time(0);
	time_t time_elapsed = time_end - time_start;

	cout << "time start (epoch): " << time_start << " seconds" << endl;
	cout << "time end (epoch): " << time_end << " seconds" << endl;
	cout << "time elapsed: " << time_elapsed << " seconds" << endl;

	cout << "frames: " << age_week << endl;

	cout << "show labels: " << SHOW_LABELS << endl;

//	*************************************************************

	// log file
	ofstream log;

	stringstream logName;
	logName << "_charge_";
	
	if (YEARS < 100) logName << "0";
	logName << YEARS;
		
	logName << "x" << SCALE << "_00000-";

	if (age_week < 10) logName << "0000";
	else if (age_week < 100) logName << "000";
	else if (age_week < 1000) logName << "00";
	else if (age_week < 10000) logName << "0";
	logName << age_week - 1;

	logName << ".log";

	log.open(logName.str());

	// check for error
	if (log.fail())
	{
		cerr << "Error opening file." << endl;
		exit(1);
	}

	if (log.is_open())
	{
		log << "size: " << width << " x " << height << " pixels" << endl;
		log << "scale: " << SCALE << endl;

		log << "time start (epoch): " << time_start << " seconds" << endl;
		log << "time end (epoch): " << time_end << " seconds" << endl;
		log << "time elapsed: " << time_elapsed << " seconds" << endl;

		log << "frames: " << age_week << endl;

		log << "show labels: " << SHOW_LABELS << endl;
	}

	log.close();
}