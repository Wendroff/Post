#ifndef TECPLOTPARSER_H
#define TECPLOTPARSER_H

#include "TecplotVar.h"
#include "TecplotZone.h"
#include "TecplotData.h"
#include "defines.h"

#include <string>

using namespace std;

class TecplotParser
{
public:
	virtual TecplotData parse() = 0;
	virtual ~TecplotParser(){DEBUG("Destroing abstract parser ");};
	virtual int parsingPercent() = 0;
};

class TecplotAsciiParser : public TecplotParser
{
public:
	TecplotAsciiParser(const string &filename);
	~TecplotAsciiParser()
	{DEBUG("Destroing TecplotAsciiParser  "<<filename_);}
	TecplotData parse();
	TecplotData parseASCII();
	TecplotData parseGZIP();
	int parsingPercent();

private:

	int parsing_percent_;
	string filename_;

	string title;

	TecplotVars vars;
	map <int, string> var_pos;

	TecplotZones zones;
	map <int, string> zone_pos;

	size_t data_read;

	bool processComment(const string & comment);
	bool processTecplot(const string & line);

	bool getTitle(const string &line);
	bool getVars(const string &line);
	bool getZone(const string &line);
	bool getData(const string &line);

	bool isFileGziped();
};

#endif