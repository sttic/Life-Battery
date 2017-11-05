/****************************************************************
	FILE:			charge.cpp
	AUTHOR:			Tommy Deng

	NOTES:			Colour values (specifically the greyscale
					ones?) of the exported bitmap can be wrong.
					It seems that they become full white.
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
	const int YEARS = 80;

	int BLOCK_SIZE = 1;
	int SPACE = 1;

	int PAD_X = 4;
	int PAD_Y = 4;

	// top battery lead (+'ve)
	int LEAD_PAD_Y = 8;
	int LEAD_WIDTH = 37;	// third of width

	const int SCALE = 1;

	//	*************************************************************

	BLOCK_SIZE *= SCALE;
	SPACE *= SCALE;

	PAD_X *= SCALE;
	PAD_Y *= SCALE;

	LEAD_PAD_Y *= SCALE;
	LEAD_WIDTH *= SCALE;

	//	*************************************************************

	int width = (WEEKS * BLOCK_SIZE) + ((WEEKS - 1) * SPACE) + (2 * PAD_X);
	int height = (YEARS * BLOCK_SIZE) + ((YEARS - 1) * SPACE) + (2 * PAD_Y) + LEAD_PAD_Y;

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

	int age_week = 0;
	const bool SHOW_LABELS = false;

	// *************************************************************

	int h = 0;
	int w = 0;
	int count = 0;

	// draw border
	img.fill(0);
	int border = BLOCK_SIZE;

	// horizontal borders
	img.draw_rectangle(0, LEAD_PAD_Y, width, LEAD_PAD_Y + border - 1, white_off);
	img.draw_rectangle(0, height - border, width, height, white_off);

	// vertical borders
	img.draw_rectangle(0, LEAD_PAD_Y, border - 1, height, white_off);
	img.draw_rectangle(width - border, LEAD_PAD_Y, width, height, white_off);

	// draw lead
	img.draw_rectangle(LEAD_WIDTH, 0, 2 * LEAD_WIDTH - 1, LEAD_PAD_Y, white_off);


	for (int Y = 0; Y < YEARS; Y++)
	{
		h = PAD_Y + Y * (BLOCK_SIZE + SPACE) + LEAD_PAD_Y;

		for (int X = 0; X < WEEKS; X++)
		{
			w = PAD_X + X * (BLOCK_SIZE + SPACE);

			img.draw_rectangle(w, h, w + BLOCK_SIZE - 1, h + BLOCK_SIZE - 1, white_off);

			count++;


			// week labels
			if (Y == 0 && SHOW_LABELS)
			{
				stringstream ss;
				ss << X + 1;
				img.draw_text(w, ((PAD_Y + LEAD_PAD_Y) + LEAD_PAD_Y) / 2, ss.str().c_str(), white);
			}
		}


		// year labels
		if (Y % 5 == 0 && SHOW_LABELS)
		{
			stringstream ss;
			ss << Y;
			img.draw_text(PAD_X / 2, h, ss.str().c_str(), white);
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

		// draw grey weeks over white blocks

		for (int Y = 0; Y < YEARS; Y++)
		{
			h = PAD_Y + Y * (BLOCK_SIZE + SPACE) + LEAD_PAD_Y;

			for (int X = 0; X < WEEKS; X++)
			{
				w = PAD_X + X * (BLOCK_SIZE + SPACE);
				age_week++;

				img.draw_rectangle(w, h, w + BLOCK_SIZE - 1, h + BLOCK_SIZE - 1, grey_dark);

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

				count++;
			}
		}
	

	// *************************************************************




	time_t time_end = time(0);
	time_t time_elapsed = time_end - time_start;

	cout << "time start (epoch): " << time_start << " seconds" << endl;
	cout << "time end (epoch): " << time_end << " seconds" << endl;
	cout << "time elapsed: " << time_elapsed << " seconds" << endl;

	cout << "frames: " << age_week + 1 << endl;

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
	logName << age_week;

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

		log << "frames: " << age_week + 1 << endl;

		log << "show labels: " << SHOW_LABELS << endl;
	}

	log.close();
}