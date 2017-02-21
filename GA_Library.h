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
	void clearIt(std::string operatingSystem);
	void cinBufferClear();
	void error(int x);
	unsigned int selector(unsigned int max);
	bool yesNo();
	std::string getField();
	std::string fieldModify(std::string enteredField);
	void pressEnterToContinue();
}

class Menu
{
private:
	std::vector<std::string> choices;
public:
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
		void setFile(Data &passThrough, std::string target, std::string extension);
	}
	namespace FileManage
	{
		void listFiles(Data &passThrough, std::string manualExtension);
		void setFileList(Data &passThrough);
		bool fileSeek(Data &passThrough);
		void deleteFile(Data &passThrough);
		std::string chooseFile(Data &passThrough, std::string manualExtension);
	}
	namespace FileContent
	{
		void displayFileContents(Data &passThrough);
		std::string randomLine(Data &passThrough);
		void listManagement(Data &passThrough);
	}
	namespace DBManage
	{
		void typeList(Data &passThrough);
		void smartBuffer(Data &passThrough);
		void dataBuffer(Data &passThrough, std::string dataType);
		//	extern searchModeActivate searchQuery;
		std::string searchSimple(Data &passThrough, std::string searchType);
		void searchDump(Data &passThrough);
		void globalDataSearch(Data &passThrough);
		void globalTermSearch(Data &passThrough);
		void globalDataDump(Data &passThrough);
		void dataDump(Data &passThrough, std::string dataTypeSelection);
		std::string dataTypeSelector(Data &passThrough);
		std::string dataSelector(Data &passThrough, std::string dataTypeSelection);
		std::string dataTarget(Data &passThrough, unsigned int dataTypePosition, unsigned int dataPosition);
	}
	bool checkParams(Data &passThrough);
}
#endif
