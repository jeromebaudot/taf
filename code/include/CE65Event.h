#ifndef CE65_EVENT_STR_h
#define CE65_EVENT_STR_h

#include <vector>

#define TREENAME "CE65Data"
#define DAC_BOARD_ADC_MAX_COUNTS  65536
#define X_MX_SIZE 64
#define Y_MX_SIZE 32

struct ce65_frame
{
	unsigned long fr_number;
	unsigned long time_stamp;
	unsigned long triggers_number;
	//std::vector<std::vector<unsigned short>> raw_amplitude;
	//std::vector<std::vector<short>> signal;
	short raw_amp[X_MX_SIZE][Y_MX_SIZE] = {{0}};
	short sig[X_MX_SIZE][Y_MX_SIZE] = {{0}};
   ce65_frame()  // initialization list
	: fr_number(0),
	  time_stamp(0),
	  triggers_number(0){}

};

struct ce65_event
{
	Long64_t ev_number;;
	int frames_per_event;
	std::vector<ce65_frame> frame;
  ce65_event()  // initialization list
	: ev_number(-1),
	  frames_per_event(-1){ }
};

#endif
