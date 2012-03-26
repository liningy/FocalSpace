#ifndef SAMPLE_H
#define SAMPLE_H

#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

class Sample  {



public:
    // constructors/desctructor
    ~Sample();
    Sample();
	Sample(string tmpPath);

	// methods
	string getPath() { return myPath;}
	void setPath(string newPath);
	void setLooping(bool loop);
	void load(string tmpPath);
	bool read();
	long length();
	double play();
	double play(double speed);
	//double play(double frequency, double start, double end);
	//double stretch(double pitch, double frequency);
    bool save();
    char *getSummary();
    int getChannels();

    // public variables
    char* 	myData;

private:
	//char* 	myPath;
	string      myPath;
	int 	myChunkSize;
	int	mySubChunk1Size;
	short 	myFormat;
	short 	myChannels;
	int   	mySampleRate;
	int   	myByteRate;
	short 	myBlockAlign;
	short 	myBitsPerSample;
	int	myDataSize;
	double position;
	double speed;
	double output;
	bool    isLooping;
	//int     playbackSamplerRate;
};


#endif // SAMPLER_H
