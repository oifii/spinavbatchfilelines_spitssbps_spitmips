#include <string>
#include <fstream>
#include <vector>

#include <iostream>
#include <sstream>
using namespace std;
//#include <stdlib.h>
#include <Windows.h>

#include "spiwavsetlib.h"


int main(int argc, char *argv[])
{
	ofstream ofs_all("spinavbatchfilelines_folders-all.txt");
	ofstream ofs_notes("spinavbatchfilelines_folders-notes.txt");
	//ofstream ofs("spinavbatchfilelines.txt");

	bool debug=false;
	//bool debug=true;

	///////////////////
	//read in arguments
	///////////////////
	string filterstring=""; //"" for no filter

	if(argc>1)
	{
		filterstring = argv[1];
	}



	//////////////////////////////////////////
	//load batch file lines into string vector
	//////////////////////////////////////////
	vector<string> batchfilelines;
	ifstream ifs("spitonicstepsequencerbufferplayersynthvoroguiwin32.bat");
	string temp;
	int ii=-1;
	while(getline(ifs,temp))
	{
		ii++;
		if(!filterstring.empty())
		{
			string TEMP = "";
			for (std::string::size_type i=0; i<temp.length(); ++i)
			{
				TEMP+= toupper(temp[i]);
			}
			string FILTERSTRING = "";
			for (std::string::size_type i=0; i<filterstring.length(); ++i)
			{
				FILTERSTRING+= toupper(filterstring[i]);
			}
			size_t found = TEMP.find(FILTERSTRING, 64); //search from 65th char 
			if(found==string::npos) continue;
		}
		//only keep foldername
		size_t pos1 = temp.rfind("D:\\");
		if(pos1!=string::npos) 
		{
			temp = temp.substr(pos1);
			size_t pos2 = temp.find("\"");
			if(pos2!=string::npos) 
			{
				temp = temp.substr(0,pos2);
				if(!temp.empty()) 
				{
					batchfilelines.push_back(temp);
					//cout << temp << endl;
					ofs_all << temp << endl;
				}
			}
		}
	}

	
	/////////////////////////////////////////////////////
	//browse throught foldernames and execute dir command
	/////////////////////////////////////////////////////
	string quote;
	quote = "\"";
	string cmdbuffer;

	vector<string> samplefilenames;
	vector<string>::iterator it_samplefilename;
	vector<string>::iterator it;
	for ( it=batchfilelines.begin() ; it < batchfilelines.end(); it++ )
	{
		//1) execute cmd line to get all folder's wav filenames
		string quote = "\"";
		string pathfilter;
		string path = *it;
		//pathfilter = path + "\\*.bmp";
		pathfilter = path + "\\" + "*.wav";
		string systemcommand;
		//systemcommand = "DIR " + quote + pathfilter + quote + "/B /O:N > wsic_filenames.txt"; //wsip tag standing for wav set (library) instrumentset (class) populate (function)
		systemcommand = "DIR " + quote + pathfilter + quote + "/B /S /O:N > spinavbatchfilelines.txt"; // /S for adding path into "spiwtmvs_filenames.txt"
		system(systemcommand.c_str());
		//2) load in all "spiwtmvs_filenames.txt" file
		ifstream ifs("spinavbatchfilelines.txt");
		string temp;
		samplefilenames.clear();
		while (getline(ifs, temp))
		{
			//txtfilenames.push_back(path + "\\" + temp);
			samplefilenames.push_back(temp);
		}

		//3) check if folder has note information for each of its wav files 
		if(!samplefilenames.empty())
		{
			bool hasnote=true;
			for ( it_samplefilename=samplefilenames.begin() ; it_samplefilename < samplefilenames.end(); it_samplefilename++ )
			{
				string samplefilename = *it_samplefilename;
				hasnote=true;
				int midinotenumber = GetMidiNoteNumberFromString(samplefilename.c_str());
				if(midinotenumber<0) 
				{
					hasnote=false;
					break;
				}
			}
			if(hasnote==true)
			{
				cout << path << endl;
				ofs_notes << path << endl;
			}
		}
	}
	
	ofs_notes.flush();
	ofs_notes.close();

	return 0;
}