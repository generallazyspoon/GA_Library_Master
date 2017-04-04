#ifndef GA_Library
#define GA_Library

#include <iostream>
#include <string>
#include <conio.h>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <random>
#include <limits>
#include <stdlib.h>

namespace UI
{
	extern bool win32Console;
	void clearIt();
	void cinBufferClear();
	void error(int x);
	unsigned int selector(unsigned int max);
	bool yesNo();
	bool commitChanges();
	std::string getField(bool punctuationFilter);
	std::string fieldModify(std::string enteredField, bool punctuationFilter);
	void pressEnterToContinue();
}

class Menu
{
public:
	std::vector<std::string> choices; 
	void clearMenu();
	void setMenu(std::string choice);
	void displayMenu();
};

namespace FileOps
{
	struct Data
	{
		std::string slash;
		std::string targetFile;
		std::string targetExtension;
		std::string workingFilename;
		std::string workingDirectory;
		std::vector<std::string> fileList = {};
		std::vector<std::string> dirList = {};
		struct databaseStruct
		{
			std::string dataType = {};
			std::vector<std::string> data = {};
		};
		std::vector<databaseStruct> infoDatabase = {};
		struct searchTable
		{
			unsigned int dataTypePosition;
			std::string dataType;
			unsigned int dataPosition;
			std::string data;
		};
		std::vector<searchTable> searchResults;
	};
	namespace TargetManage
	{
		void setDir(Data &passThrough, std::string input);
		void setFileName(Data &passThrough, std::string fileName);
		void setExtension(Data &passThrough, std::string extension);
		bool checkForDir(Data &passThrough);
	}
	namespace FileManage
	{
		void listFiles(Data &passThrough);
		void setFileList(Data &passThrough);
		bool fileSeek(Data &passThrough);
		void deleteFile(Data &passThrough, bool requestConfirmation);
		std::string chooseFile(Data &passThrough);
		void setDirList(Data & passThrough);
		void createFile(Data &passThrough);
	}
	namespace FileContent
	{
		void displayFileContents(Data &passThrough, bool numberedLines);
		std::string randomLine(Data &passThrough);
		void listManagement(Data &passThrough);
	}
	namespace DBManage
	{
		void typeList(Data &passThrough);
		void smartBuffer(Data &passThrough);
		void dataBuffer(Data &passThrough, std::string dataType);
		std::string searchSimple(Data &passThrough, std::string searchType);
		void searchDump(Data &passThrough);
		void globalDataSearch(Data &passThrough);
		void globalTermSearch(Data &passThrough);
		void globalDataDump(Data &passThrough);
		void dataDump(Data &passThrough, std::string dataTypeSelection);
		std::string dataTypeSelector(Data &passThrough);
		std::string dataSelector(Data &passThrough, std::string dataTypeSelection);
		std::string dataTarget(Data &passThrough, unsigned int dataTypePosition, unsigned int dataPosition);
		std::string manualDataCrawl(Data &passThrough);
		/// In progress:
		void dataManage(Data &passThrough, unsigned int dataTypeIndex);
	}
	bool checkParams(Data &passThrough);
}

namespace RandomOps
{
	unsigned int range(unsigned int lowNumber, unsigned int highNumber);
	unsigned int upperLimit(unsigned int highNumber);
	bool flipCoin();
	class Dicebag
	{
	public:
		std::vector<unsigned int> rollValues;
		unsigned int rollTotal;
		void rollDie(unsigned int dieType, unsigned int numberOfDice);
		void reset();
		void displayRoll();
	};
}

namespace DebugTools
{
	extern bool verbose;
	extern unsigned int consoleMarker;
	void consoleDebugMarker(std::string customMarker);
}

namespace GA_Misc
{
	std::vector<unsigned int> parseUnsignedInts(std::string parseMe);
	std::vector<int> parseInts(std::string parseMe);
	std::vector<std::string> parseStrings(std::string parseMe, char delimitChar);
}
#endif