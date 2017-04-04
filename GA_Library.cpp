#include "stdafx.h"
#include "GA_Library.h"

/// Development Notes:
// -ListManage could use updating... some kind of vector modification or something?  Numbering options?  Definitely needs easy-exits.
// -Need a "reportNoChanges" function, integrate.

/// Debugging Tools:
// "verbose" - Set this to "true" if you want to see Debugging Tool output in the console
extern bool DebugTools::verbose = true;
// "consoleMarker" - Increments every time the program passes a consoleDebugMarker() method
extern unsigned int DebugTools::consoleMarker = 0;
// "consoleDebugMarker" - Place these in the code (with an optional custom message) to mark a position in the runtime
void DebugTools::consoleDebugMarker(std::string customMarker)
{
	if (DebugTools::verbose)
	{
		if (customMarker == "")
			std::cout << "\n" << "Debug Point #: " << consoleMarker << "\n";
		else if (customMarker != "")
			std::cout << "\n" << "Debug Point #: " << consoleMarker << " - " << customMarker << "\n";
		DebugTools::consoleMarker++;
		UI::pressEnterToContinue();
	}
}

/// UI Elements:
// "win32Console"
extern bool UI::win32Console = true;
// "clearIt" - Clears the console string by flooding it with newlines.
void UI::clearIt()
{
	if (UI::win32Console)
		system("cls");
	else { std::cout << std::string(100, '\n'); }
}
// "cinBufferClear" - Clears the cin buffer for new usage.
void UI::cinBufferClear()
{
	std::cin.clear();														// clear the failed state
	// std::cin.ignore(INT_MAX);											// throw away all the input (saved for possible usage)
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');		// throw away evrerything until newline is found
}
// "error" - Reports errors based on a number code.
void UI::error(int x)
{
	switch (x)
	{
	case 0:
		std::cout << "\n<< Unspecified Error (case 0). >>\n\n";
		break;
	case 1:
		std::cout << "\n<< Unspecified Error (case 1). >>\n\n";
		break;
	case 2:
		std::cout << "\n<< File not found. >>\n\n";
		break;
	case 3:
		std::cout << "\n<< No files found. >>\n\n";
		break;
	case 4:
		std::cout << "\n<< No parameters specified for current operation. >>\n\n";
		break;
	case 5:
		std::cout << "\n<< No entries found for the specified data type. >>\n\n";
		break;
	case 6:
		std::cout << "\n<< No database information >>\n\n";
		break;
	case 7:
		std::cout << "\n<< No information provided for the called method. >>\n\n";
		break;
	case 8:
		std::cout << "\n<< Bad parameters specified for method. >>\n\n";
		break;
	default:
		std::cout << "\n<< Please specify an error code. >>\n\n";
	}
	UI::pressEnterToContinue();
}
// "selector" - Prompts the user for an integer.
unsigned int UI::selector(unsigned int max)
{
	if (max == 0)
	{
		std::cout << "No selections available, skipping function...\n\n";
		return 0;
	}
	unsigned int x;
	while (std::cout << "Please enter your selection (1 - " << max << ") (0 to skip/go back): ")
	{
		std::cin >> x;
		if (std::cin.fail() || x > max)
		{
			UI::cinBufferClear();
			std::cout << "\n<< Bad number.  Stop that. >>\n\n";
		}
		else if (x >= 0 && x <= max) 
			break;				// a number was entered; exit the loop
	}
	UI::cinBufferClear();
	return x;
}
// "yesNo" - Prompts the user for a yes or no.
bool UI::yesNo()
{
	char c;
	std::cout << "(press y/n)? ";
	do {
		c = _getch();
		tolower(c);
		if (c == 110)
		{
			std::cout << "n";
			break;
		}
		if (c == 121)
		{
			std::cout << "y";
			break;
		}
	} while (c > 0);
	switch (c) {
	case 110:
		return false;
		break;
	case 121:
		return true;
		break;
	default:
		UI::error(0);
		return false;
	}
}
// "makeChanges" - Quick prompt for confirming changes via yesNo.
bool UI::commitChanges()
{
	std::cout << "Commit changes ";
	bool makeChanges = UI::yesNo();
	return makeChanges;
}
// "getField" - Prompts the user for a line of text.
std::string UI::getField(bool punctuationFilter)
{
	std::string grabField;
	if (punctuationFilter)
	{
		bool punctuationFound = false;
		do {
			if (punctuationFound)
				std::cout << "\n" << "Alphanumeric only.  Try again: ";
			punctuationFound = false;
			std::getline(std::cin, grabField);
			grabField.erase(std::remove(grabField.begin(), grabField.end(), '\n'), grabField.end());
			for (unsigned int x = 0; x < grabField.size(); x++)
				if (!isalnum(grabField[x]) && grabField[x] != ' ')
					punctuationFound = true;
		} while (punctuationFound);
		return grabField;
	}
	// default
	std::getline(std::cin, grabField);
	grabField.erase(std::remove(grabField.begin(), grabField.end(), '\n'), grabField.end());
	return grabField;
}
// "fieldModify" - Modifies a given field and returns the new field.
std::string UI::fieldModify(std::string enteredField, bool punctuationFilter)
{
	std::cout << "\nCurrent data:  " << enteredField;
	std::cout << "\n\nPlease enter the new data:  ";
	std::string newField = getField(punctuationFilter);
	return newField;
}
// "pressEnterToContinue" - Halts the program until enter is pressed.
void UI::pressEnterToContinue()
{
	int c;
	std::cout << "\n\n>> Press enter to continue...\n\n";
	do
	{
		c = _getch();
	} while (c != 13);
}

/// Menu Elements:
// "clearMenu" - Clears all menu choices.  Encapsulated in class "Menu".
void Menu::clearMenu()
{
	choices = {};
}
// "setMenu" - Sets a menu item.  Encapsulated in class "Menu".
void Menu::setMenu(std::string choice)
{
	choices.push_back(choice);
}
// "displayMenu" - Displays the current menu options in a numbered list.  Encapsulated in class "Menu".
void Menu::displayMenu()
{
	for (unsigned int x = 0; x < choices.size(); x++)
		std::cout << x + 1 << ".  " << choices[x] << "\n";
	std::cout << "\n";
}

/// File Operations - Target Management:
// "setDir" - Sets the working directory for file operations
void FileOps::TargetManage::setDir(Data &passThrough, std::string input)
{
	passThrough.workingDirectory = input;
	if (passThrough.workingDirectory.size() > 0)
	{
		passThrough.slash = "/";
		passThrough.workingDirectory = passThrough.workingDirectory + passThrough.slash;
	}
	else if (passThrough.workingDirectory.size() == 0)
		passThrough.slash = "";
}
// "setFileName" - Sets the working filename for file operations
void FileOps::TargetManage::setFileName(Data &passThrough, std::string fileName)
{
	passThrough.targetFile = fileName;
	passThrough.workingFilename = fileName + "." + passThrough.targetExtension;
}
// "setExtension" - Sets the working file extension for file operations
void FileOps::TargetManage::setExtension(Data &passThrough, std::string extension)
{
	passThrough.targetExtension = extension;
	passThrough.workingFilename = passThrough.targetFile + "." + extension;
}
// "checkForDir" - Checks to see if the directory specified in the object exists.  Returns true if it does.
bool FileOps::TargetManage::checkForDir(Data &passThrough)
{
	if (!boost::filesystem::is_directory(passThrough.workingDirectory))
	{
		std::cout << "<< Warining!  Directory does not exist for specified object! >>";
		UI::pressEnterToContinue();
		return false;
	}
	else return true;
}

/// File Operations - File Management:
// "listFiles" - Lists the files with the given file [extension] in the working directory.
void FileOps::FileManage::listFiles(Data &passThrough)
{
	if (!FileOps::TargetManage::checkForDir(passThrough))
		return;
	if (passThrough.targetExtension == "")
	{
		UI::error(4);
		return;
	}
	passThrough.fileList = {};
	namespace fs = boost::filesystem;
	fs::path directory(passThrough.workingDirectory);
	fs::directory_iterator iter(directory), end;
	for (; iter != end; ++iter)
		if (iter->path().extension() == "." + passThrough.targetExtension)
		{
			// assigns filename w/o double-quotes to string fileFile and removes occurrences of "[extension]"
			std::string fileFile = iter->path().filename().string();
			boost::erase_all(fileFile, "." + passThrough.targetExtension);

			// outputs string fileFile as plain text
			passThrough.fileList.push_back(fileFile);
		}
	size_t listSize = passThrough.fileList.size();
	if (passThrough.fileList.size() == 0)
	{
		std::cout << "Requested file type: " << passThrough.targetExtension;
		UI::error(3);
	}
	else if (passThrough.fileList.size() > 0)
	{
		std::cout << "Requested file type: " << passThrough.targetExtension << "\n\n";
		for (size_t x = 1; x <= listSize; x++)
			std::cout << x << ".  " << passThrough.fileList[x - 1] << "\n";
		std::cout << "\n";
	}
}
// "setFileList" - Populates the internal file listing.
void FileOps::FileManage::setFileList(Data &passThrough)
{
	if (!FileOps::TargetManage::checkForDir(passThrough))
		return; 
	passThrough.fileList = {};
	namespace fs = boost::filesystem;
	fs::path directory(passThrough.workingDirectory);
	fs::directory_iterator iter(directory), end;
	for (; iter != end; ++iter)
		if (iter->path().extension() == "." + passThrough.targetExtension)
		{
			std::string fileFile = iter->path().filename().string();
			boost::erase_all(fileFile, "." + passThrough.targetExtension);
			passThrough.fileList.push_back(fileFile);
		}
}
// "duplicateFileSeek" - Searches the working directory for [target] filename with given file [extension].  Returns true if a match is found.
bool FileOps::FileManage::fileSeek(Data &passThrough)
{
	if (!FileOps::TargetManage::checkForDir(passThrough))
		return false;
	if (!checkParams(passThrough))
	{
		UI::error(4);
		return false;
	}
	setFileList(passThrough);
	size_t listSize = passThrough.fileList.size();
	for (size_t x = 0; x < listSize; x++)
	{
		if (passThrough.targetFile == passThrough.fileList[x])
			return true;
	}
	return false;
}
// "deleteFile" - Permanently deletes the [target] filename with file [extension].  Uses "yesNo" to confirm this deletion.
void FileOps::FileManage::deleteFile(Data &passThrough, bool requestConfirmation)
{
	if (!FileOps::TargetManage::checkForDir(passThrough))
		return;
	bool fileExists = fileSeek(passThrough);
	std::string selectedForDeletion;
	if (requestConfirmation)
	{
		std::cout << "\n\nFile selected for deletion: " << passThrough.targetFile << "." << passThrough.targetExtension << "\n\n";
		if (fileExists == false)
			UI::error(2);
		else if (fileExists == true)
		{
			selectedForDeletion = passThrough.workingDirectory + passThrough.workingFilename;
			std::cout << "\nAre you sure ";
			bool x = UI::yesNo();
			if (x == true)
			{
				remove((passThrough.workingDirectory + passThrough.workingFilename).c_str());
				std::cout << "File successfully deleted.";
			}
		}
	}
	else if (!requestConfirmation)
		remove((passThrough.workingDirectory + passThrough.workingFilename).c_str());
}
// "chooseFile" - Prompts the user for a file selection
std::string FileOps::FileManage::chooseFile(Data &passThrough)
{
	if (!FileOps::TargetManage::checkForDir(passThrough))
		return "";
	setFileList(passThrough);
	listFiles(passThrough);
	if (passThrough.fileList.size() == 0)
		return "";
	unsigned int x = UI::selector(passThrough.fileList.size());
	if (x == 0)
		return "";
	std::string chosenFile = passThrough.fileList[x - 1];
	return chosenFile;
}
// "setDirList" - Populates the Data struct with the subdirectories found in the workingDirectory
void FileOps::FileManage::setDirList(Data &passThrough)
{
	if (!FileOps::TargetManage::checkForDir(passThrough))
		return; 
	namespace fs = boost::filesystem;
	fs::path directory(passThrough.workingDirectory);
	fs::directory_iterator iter(directory), end;
	for (; iter != end; ++iter)
		if (is_directory(iter->path()))
		{
			std::string dirOut = iter->path().filename().string();
			passThrough.dirList.push_back(dirOut);
		}
}
// "createFile" - creates a file based on the object's parameters
void FileOps::FileManage::createFile(Data &passThrough)
{
	if (passThrough.targetFile == "" || passThrough.targetExtension == "" || passThrough.workingDirectory == "")
	{
		UI::error(4);
		UI::pressEnterToContinue();
		return;
	}
	std::ofstream makeFile(passThrough.workingDirectory + passThrough.workingFilename);
	makeFile.close();
}

/// File Operations - File Content:
// "displayFileContents" - displays the contents of a target file, line by line
void FileOps::FileContent::displayFileContents(Data &passThrough, bool numberedLines)
{
	std::ifstream displayFile(passThrough.workingDirectory + passThrough.workingFilename);
	std::string line = "";
	if (numberedLines)
	{
		std::vector<std::string> entryDB = {};
		while (std::getline(displayFile, line))
			entryDB.push_back(line);
		displayFile.close();
		for (unsigned int x = 0; x < entryDB.size(); x++)
		{
			std::cout << x + 1 << ".  " << entryDB[x];
			if (x < entryDB.size() - 1)
				std::cout << "\n";
		}
		return;
	}
	else if (!numberedLines)
	{
		if (displayFile.is_open())
		{
			while (displayFile.good())
				getline(displayFile, line);
			displayFile.close();
			return;
		}
	}
	// default error (shouldn't be here)
	UI::error(2);
	displayFile.close();
}
// "randomItem" - Grabs a random line from the working file
std::string FileOps::FileContent::randomLine(Data &passThrough)
{
	unsigned int randomEntrySize = 0;
	std::ifstream chosenFile(passThrough.workingDirectory + passThrough.workingFilename);
	std::string randomEntry;
	while (std::getline(chosenFile, randomEntry))
		randomEntrySize++;
	chosenFile.close();
	std::ifstream chosenFileAgain(passThrough.workingDirectory + passThrough.workingFilename);
	unsigned int entrySelected = RandomOps::upperLimit(randomEntrySize);
	for (unsigned int x = 1; x <= entrySelected; x++)
		std::getline(chosenFileAgain, randomEntry);
	chosenFileAgain.close();
	return randomEntry;
}
// "listManagement" - Manages a file line-by-line.  Display All, Add, Remove, Modify, Display random.
void FileOps::FileContent::listManagement(Data &passThrough)
{
	Menu list;
	std::string entry;
	unsigned int selection;
	do
	{
		UI::clearIt();
		std::cout << passThrough.targetFile << " Database Management:\n\n";
		list.clearMenu();
		list.setMenu("List " + passThrough.targetFile + " entries");
		list.setMenu("Add a " + passThrough.targetFile + " entry");
		list.setMenu("Modify a " + passThrough.targetFile + " entry");
		list.setMenu("Remove a " + passThrough.targetFile + " entry");
		list.setMenu("Display a random " + passThrough.targetFile + " entry");
		list.displayMenu();
		selection = UI::selector(5);
		switch (selection)
		{
		case 0:
		{
			return;
		}
		// List entries
		case 1:
		{
			UI::clearIt();
			std::cout << "List " << passThrough.targetFile << " entries" << "\n\n";
			displayFileContents(passThrough, true);
			UI::pressEnterToContinue();
			break;
		}
		// Add a new entry
		case 2:
		{
			std::ofstream chosenFile(passThrough.workingDirectory + passThrough.workingFilename, std::ios_base::app);
			std::string newEntry;
			std::string newLine;
			bool correctEntry = true;
			bool entryAgain = false;
			do {
				do
				{
					UI::clearIt();
					std::cout << "Add a " << passThrough.targetFile << " entry " << "\n\n";
					displayFileContents(passThrough, true);
					std::cout << "\n\n" << "Please enter a new " << passThrough.targetFile << " entry:  ";
					newEntry = UI::getField(false);
					std::cout << "\nYou have entered: " << "\"" << newEntry << "\"" << "\n\nIs this correct ";
					correctEntry = UI::yesNo();
				} while (!correctEntry);
				if (correctEntry == true)
				{
					std::cout << "\n\nWrite to file ";
					bool writeIt = UI::yesNo();
					if (writeIt == true)
						chosenFile << "\n" + newEntry;
				}
				std::cout << "\n\nEnter a new " << passThrough.targetFile << " entry ";
				entryAgain = UI::yesNo();
			} while (entryAgain);
			chosenFile.close();
			break;
		}
		// Modify an entry
		case 3:
		{
			bool modifyAnotherEntry = false;
			do
			{
				UI::clearIt();
				std::cout << "Modify a " << passThrough.targetFile << " entry" << "\n\n";
				std::cout << "Which " << passThrough.targetFile << " entry would you like to modify?\n\n";
				std::vector<std::string> entryDB = {};
				std::ifstream chosenFile(passThrough.workingDirectory + passThrough.workingFilename, std::ios_base::app);
				while (std::getline(chosenFile, entry))
					entryDB.push_back(entry);
				chosenFile.close();
				for (unsigned int x = 0; x < entryDB.size(); x++)
					std::cout << x + 1 << ".  " << entryDB[x] << "\n";
				std::cout << "\n";
				unsigned int modifyEntry = UI::selector(entryDB.size());
				if (modifyEntry == 0)
					break;
				std::string selectedForModification = entryDB[modifyEntry - 1];
				std::string modificationBuffer = entryDB[modifyEntry - 1];
				std::string modifiedEntry = UI::fieldModify(selectedForModification, false);
				std::cout << "\nProposed change: " << modificationBuffer << " --> " << modifiedEntry << "\n\n";
				std::cout << "Apply this modification ";
				bool modifyThisEntry = UI::yesNo();
				if (modifyThisEntry == true)
				{
					entryDB[modifyEntry - 1] = modifiedEntry;
					std::cout << "\n\nModifying selected " << passThrough.targetFile << " entry...";
					std::ofstream chosenFile;
					chosenFile.open(passThrough.workingDirectory + passThrough.workingFilename, std::ofstream::out | std::ofstream::trunc);
					for (unsigned int x = 0; x < entryDB.size(); x++)
					{
						if (x != entryDB.size() - 1)
							chosenFile << entryDB[x] + "\n";
						if (x == entryDB.size() - 1)
							chosenFile << entryDB[x];
					}
					chosenFile.close();
				}
				std::cout << "\n\nModify another " << passThrough.targetFile << " entry ";
				modifyAnotherEntry = UI::yesNo();
			} while (modifyAnotherEntry);
			break;
		}
		// Delete an entry
		case 4:
		{
			bool deleteAnotherEntry = true;
			do
			{
				UI::clearIt();
				std::cout << "Delete " << passThrough.targetFile << " entry" << "\n\n";
				std::vector<std::string> entryDB = {};
				std::ifstream chosenFile(passThrough.workingDirectory + passThrough.workingFilename, std::ios_base::app);
				while (std::getline(chosenFile, entry))
					entryDB.push_back(entry);
				chosenFile.close();
				for (unsigned int x = 0; x < entryDB.size(); x++)
					std::cout << x + 1 << ".  " << entryDB[x] << "\n";
				std::cout << "\nWhich " << passThrough.targetFile << " entry would you like to delete?\n\n";
				unsigned int deleteEntry = UI::selector(entryDB.size());
				if (deleteEntry == 0)
					break;
				std::string selectedForDeletion = entryDB[deleteEntry - 1];
				std::cout << "\nYou have selected the following " << passThrough.targetFile << " for deletion:\n\n" << selectedForDeletion << "\n\nAre you sure you wish to delete it ";
				bool deleteThisEntry = UI::yesNo();
				if (deleteThisEntry == true)
				{
					std::vector<std::string>::iterator deleteMe = std::find(entryDB.begin(), entryDB.end(), selectedForDeletion);
					if (deleteMe == entryDB.end())
					{
						std::cout << "Entry not found! (If you're seeing this, something is wrong)\n";
					}
					else
					{
						entryDB.erase(deleteMe);
						std::cout << "\n\nDeleting selected " << passThrough.targetFile << " entry...";
						std::ofstream chosenFile(passThrough.workingDirectory + passThrough.workingFilename);
						for (unsigned int x = 0; x < entryDB.size(); x++)
						{
							if (x != entryDB.size())
								chosenFile << entryDB[x] << "\n";
							if (x == entryDB.size())
								chosenFile << entryDB[x];
						}
					}
				}
				std::cout << "\n\nDelete another " << passThrough.targetFile + " entry ";
				deleteAnotherEntry = UI::yesNo();
			} while (deleteAnotherEntry);
			break;
		}
		// Display a random entry
		case 5:
		{
			std::cout << "\n" << randomLine(passThrough);
			UI::pressEnterToContinue();
			break;
		}
		default:
		{
			UI::error(0);
			break;
		}
		}
	} while (selection != 0);
}

/// File Operations - Database Management:
// "searchQuery" - Table of database search elements.
FileOps::Data::searchTable searchQuery;
// "checkParams" - Checks to see if file information has been defined.
bool FileOps::checkParams(Data &passThrough)
{
	// Unnecessary unless checking files without extensions
	if (passThrough.workingFilename.size() == 0)
		return false;
	// Necessary
	if (passThrough.targetFile.size() == 0)
		return false;
	if (passThrough.targetExtension.size() == 0)
		return false;
	return true;
}
// "typeList" - Displays the internal database of specified [dataType].
void FileOps::DBManage::typeList(Data &passThrough)
{
	if (passThrough.infoDatabase.size() == 0)
	{
		UI::error(6);
		return;
	}
	std::cout << "dataTypes found:\n\n";
	for (unsigned int dBTypePos = 0; dBTypePos < passThrough.infoDatabase.size(); dBTypePos++)
		std::cout << dBTypePos + 1 << ":  " << passThrough.infoDatabase[dBTypePos].dataType << "\n";
	std::cout << "\n";
}
// "smartBuffer" - reads a file and searches for delimited (":  ") data types and populates the internal database.
void FileOps::DBManage::smartBuffer(Data &passThrough)
{
	// open file
	std::ifstream parseFile(passThrough.workingDirectory + passThrough.workingFilename);
	// establish the local vector for processing
	std::vector<std::string> typeProcessing = {};
	bool typeFound = false;
	// establish a few objects
	Data::databaseStruct databaseEntry;
	std::string lineGrab;
	std::string delimiter = ":  ";
	// begin the parsing sequence
	while (std::getline(parseFile, lineGrab))
	{
		size_t pos = lineGrab.find(delimiter);
		// search for delimiter
		if (pos != std::string::npos)
		{
			typeFound = false;
			// Section off the dataType
			std::string dataCheck = (lineGrab.substr(0, pos));
			// check if dataType has already been logged
			for (unsigned int x = 0; x < typeProcessing.size(); x++)
				if (dataCheck == typeProcessing[x])
					typeFound = true;
			if (typeFound == false)
				typeProcessing.push_back(lineGrab.substr(0, pos));
		}
	}
	// populates the class vector with data
	for (unsigned int z = 0; z < typeProcessing.size(); z++)
	{
		dataBuffer(passThrough, typeProcessing[z]);
	}
}
// "dataBuffer" - Parses data after a delimiter (":  ") in the working file as specified by [dataType]
void FileOps::DBManage::dataBuffer(Data &passThrough, std::string dataType)
{
	std::ifstream parseFile(passThrough.workingDirectory + passThrough.workingFilename);
	std::string dataParse;
	std::string delimiter = dataType + ":  ";
	
	// break for duplicates
	for (unsigned int x = 0; x < passThrough.infoDatabase.size(); x++)
		if (dataType == passThrough.infoDatabase[x].dataType)
			return;

	// declare a database Input
	Data::databaseStruct databaseEntry;
	databaseEntry.dataType = dataType;

	// Scan each line until the dataType is found
	std::string data;
	while (std::getline(parseFile, dataParse))
	{
		size_t x = dataParse.find(delimiter);
		// Assign data to dataType struct
		if (x != std::string::npos)
			databaseEntry.data.push_back(dataParse.substr(delimiter.length()));
	}
	passThrough.infoDatabase.push_back(databaseEntry);
	parseFile.close();
	if (passThrough.infoDatabase.size() == 0)
		UI::error(5);
}
// "searchSimple" - Returns a string based on the given [searchType].
std::string FileOps::DBManage::searchSimple(Data &passThrough, std::string searchType)
{
	if (passThrough.infoDatabase.size() == 0)
	{
		UI::error(5);
		return "infoDatabase is empty (Why?).";
	}
	// Returns the contents of requested dataTypePosition (if present)
	if (searchType == "dataTypePosition" && passThrough.infoDatabase.size() != 0 && passThrough.infoDatabase.size() - 1 >= searchQuery.dataTypePosition)
	{
		return passThrough.infoDatabase[searchQuery.dataTypePosition].dataType;
	}
	// Returns the position of search struct dataType (if found)
	if (searchType == "dataType")
	{
		for (unsigned int y = 0; y < passThrough.infoDatabase.size(); y++)
		{
			std::string boolCheck = passThrough.infoDatabase[y].dataType;
			std::string z1 = std::to_string(y);
			if (boolCheck == searchQuery.dataType)
				return z1;
		}
	}
	// Returns the contents of requested dataPosition (given specified dataType)
	if (searchType == "dataPosition" && passThrough.infoDatabase[searchQuery.dataTypePosition].data.size() != 0
		&& passThrough.infoDatabase[searchQuery.dataTypePosition].data.size() - 1 >= searchQuery.dataPosition)
	{
		return passThrough.infoDatabase[searchQuery.dataTypePosition].data[searchQuery.dataPosition];
	}
	// Returns the position of search struct data (if found)
	if (searchType == "data")
	{
		for (unsigned int y = 0; y < passThrough.infoDatabase[searchQuery.dataTypePosition].data.size(); y++)
		{
			std::string boolCheck = passThrough.infoDatabase[searchQuery.dataTypePosition].data[y];
			std::string z2 = std::to_string(y);
			if (boolCheck == searchQuery.data)
				return z2;
		}
	}
	// Returns a default failure message
	return "The information requested has not been found.";
}
// "searchDump" - Displays all search queries and resulting data.
void FileOps::DBManage::searchDump(Data &passThrough)
{
	if (passThrough.infoDatabase.size() == 0)
	{
		UI::error(5);
		return;
	}
	std::cout << "dataTypePosition <"	 << searchQuery.dataTypePosition	<< ">:  "	<< searchSimple(passThrough, "dataTypePosition")		<< "\n";
	std::cout << "dataType <"			 << searchQuery.dataType			<< ">:  "	<< searchSimple(passThrough, "dataType")				<< "\n";
	std::cout << "dataPosition <"		 << searchQuery.dataPosition		<< ">:  "	<< searchSimple(passThrough, "dataPosition")			<< "\n";
	std::cout << "data <"				 << searchQuery.data				<< ">:  "	<< searchSimple(passThrough, "data")					<< "\n";
}
// "globalDataSearch" - Searches the entire database for a data query.
void FileOps::DBManage::globalTermSearch(Data &passThrough)
{
	if (passThrough.infoDatabase.size() == 0)
	{
		UI::error(5);
		return;
	}
	// Create a vector to store all locations:
	unsigned int dataTypeMatches = 0;
	std::vector<Data::searchTable> searchIt;
	std::string globalDataSearchQuery;
	// Search from user input or from buffered data search term?
	std::cout << "Search for <" << searchQuery.data << "> (term left in search buffer) ";
	bool dBYes = UI::yesNo();
	switch (dBYes)
	{
	case true:
		globalDataSearchQuery = searchQuery.data;
		break;
	case false:
		std::cout << "\n\nPlease enter a new search term:  ";
		globalDataSearchQuery = UI::getField(false);
		break;
	}
	// Searching for a dataType match...
	for (unsigned int dBTypeSearchCounter = 0; dBTypeSearchCounter < passThrough.infoDatabase.size(); dBTypeSearchCounter++)
	{
		if (globalDataSearchQuery == passThrough.infoDatabase[dBTypeSearchCounter].dataType)
			dataTypeMatches = dBTypeSearchCounter;
	}
	// Searching for data...
	for (unsigned int dBTypePos = 0; dBTypePos < passThrough.infoDatabase.size(); dBTypePos++)
		for (unsigned int dataPos = 0; dataPos < passThrough.infoDatabase[dBTypePos].data.size(); dataPos++)
			if (globalDataSearchQuery == passThrough.infoDatabase[dBTypePos].data[dataPos])
				searchIt.push_back({ dBTypePos, passThrough.infoDatabase[dBTypePos].dataType, dataPos, passThrough.infoDatabase[dBTypePos].data[dataPos] });
	// Listing results, if any...
	if (dataTypeMatches > 0)
		std::cout << "\ndataType \"" << globalDataSearchQuery << "\" located at database position: " << dataTypeMatches << "\n\n";
	else if (!dataTypeMatches)
		std::cout << "\ndataType results for \"" << globalDataSearchQuery << "\" could not be found.  Please check your query and try again.\n";
	if (searchIt.size() > 0)
	{
		std::cout << "\ndata results for " << globalDataSearchQuery << ":\n\n";
		for (unsigned int x = 0; x < searchIt.size(); x++)
			std::cout << "dataType position <" << searchIt[x].dataTypePosition << "> labeled <" << searchIt[x].dataType << "> at dataPosition <" <<
				searchIt[x].dataPosition << ">.\n";
		std::cout << "\n";
	}
	// No search results found...
	else if (!searchIt.size())
		std::cout << "\ndata results for \"" << globalDataSearchQuery << "\" could not be found.  Please check your query and try again.\n";
}
// "globalDataSearch" - Searches all data fields for a field (default: searchTable.data)
void FileOps::DBManage::globalDataSearch(Data &passThrough)
{
	if (passThrough.infoDatabase.size() == 0)
	{
		UI::error(5);
		return;
	}
	// Create a vector to store all locations:
	std::vector<Data::searchTable> searchIt;
	std::string globalDataSearchQuery;
	// Search from user input or from buffered data search term?
	std::cout << "Search for <" << searchQuery.data << "> (term left in search buffer) ";
	bool dBYes = UI::yesNo();
	switch (dBYes)
	{
	case true:
		globalDataSearchQuery = searchQuery.data;
		break;
	case false:
		std::cout << "\n\nPlease enter a new search term:  ";
		globalDataSearchQuery = UI::getField(false);
		break;
	}
	// For loop nest: Searching and allocating to vector
	for (unsigned int dBTypePos = 0; dBTypePos < passThrough.infoDatabase.size(); dBTypePos++)
		for (unsigned int dataPos = 0; dataPos < passThrough.infoDatabase[dBTypePos].data.size(); dataPos++)
			if (globalDataSearchQuery == passThrough.infoDatabase[dBTypePos].data[dataPos])
				searchIt.push_back({ dBTypePos, passThrough.infoDatabase[dBTypePos].dataType, dataPos, passThrough.infoDatabase[dBTypePos].data[dataPos] });
	if (searchIt.size() > 0)
	{
		std::cout << "\nSearch results for " << globalDataSearchQuery << ":\n\n";
		for (unsigned int x = 0; x < searchIt.size(); x++)
		{
			std::cout << "dataType position <" << searchIt[x].dataTypePosition << "> labeled <" << searchIt[x].dataType << "> at dataPosition <" <<
				searchIt[x].dataPosition << ">.\n";
		}
	}
	else if (!searchIt.size())
		std::cout << "\"" << globalDataSearchQuery << "\" could not be found.  Please check your query and try again.";
}
// "globalDataDump" - Reports all data in all dataTypes.
void FileOps::DBManage::globalDataDump(Data &passThrough)
{
	if (passThrough.infoDatabase.size() == 0)
	{
		UI::error(6);
		return;
	}
	for (unsigned int x = 0; x < passThrough.infoDatabase.size(); x++)
	{
		std::cout << "dataType (" << x << "):  " << passThrough.infoDatabase[x].dataType << "\n\n";
		for (unsigned int y = 0; y < passThrough.infoDatabase[x].data.size(); y++)
			std::cout << y << ":  " << passThrough.infoDatabase[x].data[y] << "\n";
		std::cout << "\n";
	}
}
// "dataDump" - outputs a list of data for specified dataType.
void FileOps::DBManage::dataDump(Data &passThrough, std::string dataTypeSelection)
{
	unsigned int dataTypePos = 0;
	for (; dataTypePos < passThrough.infoDatabase.size(); dataTypePos++)
		if (dataTypeSelection == passThrough.infoDatabase[dataTypePos].dataType)
			break;
	if (dataTypePos == passThrough.infoDatabase.size())
	{
		std::cout << "No dataType \"" << dataTypeSelection << "\" found.";
		return;
	}
	for (unsigned int y = 0; y < passThrough.infoDatabase[dataTypePos].data.size(); y++)
		std::cout <<  y + 1 << ":  " << passThrough.infoDatabase[dataTypePos].data[y] << "\n";
	std::cout << "\n";
}
// "dataTypePicker" - Lists the dataTypes in a given database and returns the dataType as a string.
std::string FileOps::DBManage::dataTypeSelector(Data &passThrough)
{
	std::cout << "Please select the dataType you wish to use:\n\n";
	typeList(passThrough);
	unsigned int dataTypeSelection = UI::selector(passThrough.infoDatabase.size()) - 1;
	return passThrough.infoDatabase[dataTypeSelection].dataType;
}
// "dataPicker" - Lists the data in a given type and returns the chosen data as a string.
std::string FileOps::DBManage::dataSelector(Data &passThrough, std::string dataTypeSelection)
{
	std::cout << "Please select the " << dataTypeSelection << " data you wish to use:\n\n";
	dataDump(passThrough, dataTypeSelection);
	unsigned int dataTypeSize = 0;
	unsigned int dataTypePosition = 0;
	for (; dataTypePosition < passThrough.infoDatabase.size(); dataTypePosition++)
		if (passThrough.infoDatabase[dataTypePosition].dataType == dataTypeSelection)
		{
			dataTypeSize = passThrough.infoDatabase[dataTypePosition].data.size();
			break;
		}
	unsigned int dataSelection = UI::selector(dataTypeSize) - 1;
	return passThrough.infoDatabase[dataTypePosition].data[dataSelection];
}
// "dataTarget" - returns the data of given database at given target coordinates
std::string FileOps::DBManage::dataTarget(Data &passThrough, unsigned int dataTypePosition, unsigned int dataPosition)
{
	if (dataTypePosition >= passThrough.infoDatabase.size())
		return "Error: dataType position out of range.";
	else if (dataPosition >= passThrough.infoDatabase[dataTypePosition].data.size())
		return "Error: data position out of range.";
	return passThrough.infoDatabase[dataTypePosition].data[dataPosition];
}
// "manaualDataCrawl" - uses a menu-style system to return a specific data value.
std::string FileOps::DBManage::manualDataCrawl(Data &passThrough)
{
	unsigned int dataTypeSelection;
	unsigned int dataSelection;
	do
	{
		typeList(passThrough);
		dataTypeSelection = UI::selector(passThrough.infoDatabase.size());
		if (dataTypeSelection == 0)
			break;
		do
		{
			dataDump(passThrough, passThrough.infoDatabase[dataTypeSelection - 1].dataType);
			dataSelection = UI::selector(passThrough.infoDatabase[dataTypeSelection - 1].data.size());
			if (dataSelection != 0 && dataSelection < passThrough.infoDatabase[dataTypeSelection - 1].data.size())
				return passThrough.infoDatabase[dataTypeSelection - 1].data[dataSelection - 1];
		} while (dataSelection != 0);
	} while (dataTypeSelection != 0);
	return "";
}
/// In Progress:
// "dataManage" - manage the data at located at infodatabase[dataTypeIndex]
void FileOps::DBManage::dataManage (Data &passThrough, unsigned int dataTypeIndex)
{
	Menu thisMenu;
	std::string modEntry;
	unsigned int selectedEntry;

	dataDump(passThrough, passThrough.infoDatabase[dataTypeIndex].dataType);

	thisMenu.clearMenu();
	thisMenu.setMenu("Add a " + passThrough.infoDatabase[dataTypeIndex].dataType + " entry");
	thisMenu.setMenu("Modify a " + passThrough.infoDatabase[dataTypeIndex].dataType + " entry");
	thisMenu.setMenu("Delete a " + passThrough.infoDatabase[dataTypeIndex].dataType + " entry");
	// thisMenu.setMenu("Shift a " + passThrough.infoDatabase[dataTypeIndex].dataType + " entry");
	// thisMenu.setMenu("Swap a " + passThrough.infoDatabase[dataTypeIndex].dataType + " entry");
	thisMenu.displayMenu();
	unsigned int databaseEntryAction = UI::selector(thisMenu.choices.size());
	switch (databaseEntryAction)
	{
	case 0:
		return;
	case 1:
		UI::clearIt();
		dataDump(passThrough, passThrough.infoDatabase[dataTypeIndex].dataType);
		std::cout << "Please enter a data entry item for dataType \"" << 
			passThrough.infoDatabase[dataTypeIndex].dataType << "\": ";
		modEntry = UI::getField(false);
		if (modEntry == "0")
			break;
		if (UI::commitChanges())
			passThrough.infoDatabase[dataTypeIndex].data.push_back(modEntry);
		break;
	case 2:
		UI::clearIt();
		dataDump(passThrough, passThrough.infoDatabase[dataTypeIndex].dataType);
		if (passThrough.infoDatabase[dataTypeIndex].data.size() == 0)
		{
			std::cout << "No data entries found for dataType \"" + passThrough.infoDatabase[dataTypeIndex].dataType + "\".";
			UI::pressEnterToContinue();
			break;
		}
		std::cout << "Select the data you wish to modify.\n";
		selectedEntry = UI::selector(passThrough.infoDatabase[dataTypeIndex].data.size());
		if (selectedEntry == 0)
			break;
		std::cout << "Current data entry:  " << passThrough.infoDatabase[dataTypeIndex].data[selectedEntry - 1] << "\n\n";
		std::cout << "New data entry:  ";
		modEntry = UI::getField(false);
		if (UI::commitChanges())
			passThrough.infoDatabase[dataTypeIndex].data[selectedEntry - 1] = modEntry;
		break;
	case 3:
		UI::clearIt();
		dataDump(passThrough, passThrough.infoDatabase[dataTypeIndex].dataType);
		if (passThrough.infoDatabase[dataTypeIndex].data.size() == 0)
		{
			std::cout << "No data entries found for dataType \"" + passThrough.infoDatabase[dataTypeIndex].dataType + "\".";
			UI::pressEnterToContinue();
			break;
		}
		std::cout << "Select the style note you wish to delete.\n";
		selectedEntry = UI::selector(passThrough.infoDatabase[dataTypeIndex].data.size());
		if (selectedEntry == 0)
			break;
		if (UI::commitChanges())
			passThrough.infoDatabase[dataTypeIndex].data.erase(passThrough.infoDatabase[dataTypeIndex].data.begin() + selectedEntry - 1);
		break;
	case 4:
	{
		// shift an entry (move all others relative to change)
		DebugTools::consoleDebugMarker("This method under construction");
		break;
	}
	case 5:
	{
		// swap an entry (4 <-> 8, for example)
		DebugTools::consoleDebugMarker("This method under construction");
		break;
	}
	default:
	{
		// shouldn't be here
		DebugTools::consoleDebugMarker("Vector managemeng defualt case triggered");
		break;
	}
	}
}

/// Random Number Operations:
// "range" - provides a random number given a provided range, low and high
unsigned int RandomOps::range(unsigned int lowNumber, unsigned int highNumber)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> randomPicker(lowNumber, highNumber);
	return randomPicker(rng);
}
// "upperLimit" - provides a random number given a provided upper limit (or maximum)
unsigned int RandomOps::upperLimit(unsigned int highNumber)
{
	unsigned int lowNumber = 1;
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> randomPicker(lowNumber, highNumber);
	return randomPicker(rng);
}
// "flipCoin" - returns a random bool
bool RandomOps::flipCoin()
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> randomPicker(0, 1);
	unsigned int headsTails = randomPicker(rng);
	if (headsTails == 1)
		return true;
	else if (headsTails == false)
		return false;
	DebugTools::consoleDebugMarker("RandomOps::flipCoin() defaulted to false for some reason");
	return false;
}
// "Dicebag::rollDie" - run RNG for [numbeOfDice] x [dieType] and appropriate values to the Dicebag object
void RandomOps::Dicebag::rollDie(unsigned int dieType, unsigned int numberOfDice)
{
	for (unsigned int rollQueue = 0; rollQueue < numberOfDice; rollQueue++)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomPicker(1, dieType);
		rollValues.push_back(randomPicker(rng));
	}
	for (unsigned int valueAdd = 0; valueAdd < rollValues.size(); valueAdd++)
		rollTotal = rollValues[valueAdd] + rollTotal;
}
// "Dicebag::reset" - resets the Dicebag object roll values and roll total
void RandomOps::Dicebag::reset()
{
	rollValues = {};
	rollTotal = 0;
}
// "Dicebag::displayRoll" - displays the contents of the Dicebag's roll
void RandomOps::Dicebag::displayRoll()
{
	if (rollValues.size() == 0)
	{
		UI::error(7);
		return;
	}
	std::cout << rollTotal << "\n";
	if (rollValues.size() == 1)
	{
		std::cout << rollValues[0];
		return;
	}
	else if (rollValues.size() > 1)
	{
		for (unsigned int x = 0; x < rollValues.size() - 1; x++)
			std::cout << rollValues[x] << ", ";
		std::cout << rollValues[rollValues.size() - 1];
		std::cout << "\n";
	}
}

/// String Parsing Tools:
// "parseUnsignedInts" - parse all the integers (unsigned Int range) from a string and pass the results back as a vector
std::vector<unsigned int> GA_Misc::parseUnsignedInts(std::string parseMe)
{
	// Allocate the memory for the containers
	std::string interimString;
	unsigned int interimInt;
	std::vector<unsigned int> pushThisValue = {};
	
	// Failsafe Exit
	// if (parseMe == "") { UI::error(7); return pushThisValue; }
	
	// process the string	
	for (unsigned int x = 0; x < parseMe.size(); x++)
	{
		// Parse any digits
		if (isdigit(parseMe[x]))
			interimString += parseMe[x];
		// push any present interimInts when reaching a non-digit
		if (!isdigit(parseMe[x]))
			if (interimString.size() > 0)
			{
				interimInt = stoi(interimString);
				pushThisValue.push_back(interimInt);
				interimString = "";
			}
		// Push any leftover interimInts when reaching the end of parseMe
		if (x == parseMe.size() - 1)
			if (interimString.size() > 0)
			{
				interimInt = stoi(interimString);
				pushThisValue.push_back(interimInt);
				interimString = "";
			}
	}
	return pushThisValue;
	// burp
}
// "parseInts" - parse all the integers (int range, negative OK) from a string and pass the results back as a vector
std::vector<int> GA_Misc::parseInts(std::string parseMe)
{
	// Allocate the memory for the containers
	std::string interimString;
	int interimInt;
	std::vector<int> pushThisValue = {};
	// Create a switch for use in encountering negative numbers
	bool negative = false;

	// Failsafe Exit
	// if (parseMe == "") { UI::error(7); return pushThisValue; }

	// process the string	
	for (unsigned int x = 0; x < parseMe.size(); x++)
	{
		// check for a negative marker
		if (x > 0 && isdigit(parseMe[x]) && parseMe.substr(x - 1, 1) == "-")
			negative = true;
		// Parse any digits
		if (isdigit(parseMe[x]))
			interimString += parseMe[x];
		// push any present interimInts when reaching a non-digit
		if (!isdigit(parseMe[x]))
			if (interimString.size() > 0)
			{
				interimInt = stoi(interimString);
				if (!negative)
					pushThisValue.push_back(interimInt);
				else if (negative)
					pushThisValue.push_back(-(interimInt));
				interimString = "";
				negative = false;
			}
		// Push any leftover interimInts when reaching the end of parseMe
		if (x == parseMe.size() - 1)
			if (interimString.size() > 0)
			{
				interimInt = stoi(interimString);
				if (!negative)
					pushThisValue.push_back(interimInt);
				else if (negative)
					pushThisValue.push_back(-(interimInt));
				interimString = "";
				negative = false;
			}
	}
	return pushThisValue;
	// burp
}
// "parseStrings" - parse all strings separated by the given delimiter (default:  comma) and pass the results back as a vector
std::vector<std::string> GA_Misc::parseStrings(std::string parseMe, char delimitChar)
{
	// Create the return container
	std::vector<std::string> gatheredStrings;
	// Failsafe exit
	if (parseMe == "")
	{
		UI::error(7);
		return gatheredStrings;
	}
	// Safety for delimiting character
	if (!delimitChar)
		delimitChar = ',';
	// Create a parsing index with a start position of 0
	std::vector<unsigned int> parseMarkers = { 0 };
	// Manufacture the parsing index
	for (unsigned int x = 0; x < parseMe.size(); x++)
		if (parseMe[x] == delimitChar)
		{
			parseMarkers.push_back(x);
				if (x != parseMe.npos)
				parseMarkers.push_back(x + 1);
		}
	parseMarkers.push_back(parseMe.npos);
	// Fill the return container with strings
	for (unsigned int y = 0; y < parseMarkers.size(); y++)
	{
		int length = parseMarkers[y + 1] - parseMarkers[y];
		gatheredStrings.push_back(parseMe.substr(parseMarkers[y], length));
		y++;
	}
	// Deliver the goods
	return gatheredStrings;
}

/// References & Templates:
// default template for menus
/*
void menuName(Menu &passThrough)
{
unsigned int thisMenuSelection;
do {
UI::clearIt();
// Populate the menu
passThrough.displayMenu();
thisMenuSelection = UI::selector(passThrough.choices.size());
switch (thisMenuSelection)
{
case 0:
break;
case 1:
// submenu or method
break;
case 2:
// submenu or method, etc
break;
default:
DebugTools::consoleDebugMarker("Warning: menu default case reached");
break;
}
} while (thisMenuSelection > 0);
}
*/

// color utility to adapt (1):
/*

/// preprocessor:
#include <iostream>
#include "string"
#include <Windows.h>

using namespace std;


enum Colors {
black,          //  0 text color - multiply by 16, for background colors
dark_blue,      //  1
dark_green,     //  2
dark_cyan,      //  3
dark_red,       //  4
dark_magenta,   //  5
dark_yellow,    //  6
light_gray,     //  7
dark_gray,      //  8
light_blue,     //  9
light_green,    // 10
light_cyan,     // 11
light_red,      // 12
light_magenta,  // 13
light_yellow,   // 14
white           // 15
};

#define on , // So I can use the function - void text(text_color on background_color)
// To more easily remember which is text color vs background color

// My text color function. Use it if you wish.


void text(int text_color = 7 on int paper_color = 0)
{
// defaults to light_gray on black
int color_total = (text_color + (paper_color * 16));
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_total);
}
///

/// in function:
char map[10][21] = {
"####################",
"#@     99   # #   9#",
"#### #   2 9# # 2  #",
"#  # ##### ## 2  9 #",
"#9   999      ######",
"#9 ####### ##2     #",
"#9 #2      #    99 #",
"#k ####  # ### #####",
"# 2   9  # 9     2!#",
"####################" };


for (int y = 0; y < 10; y++)
{
for (int x = 0; x < 20; x++)
{
if (map[y][x] == '#')
text(black on white);
else if (map[y][x] == '@')
text(dark_yellow on dark_red);
else if (map[y][x] == '9')
text(light_yellow on dark_gray);
else if (map[y][x] == '2')
text(black on dark_gray);
else if (map[y][x] == '!')
text(light_green on dark_gray);
cout << map[y][x];// cout the one char in the map array, after
// changing color
}
cout << endl;
}
cout << endl << endl;
///



*/

// color utility to adapt (2):
/*

//const WORD colors[] =
//{
//	0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
//	0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
//};

//HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
//HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
//WORD   index = 0;

//// Remember how things were when we started
//CONSOLE_SCREEN_BUFFER_INFO csbi;
//GetConsoleScreenBufferInfo(hstdout, &csbi);

//// Tell the user how to stop
//SetConsoleTextAttribute(hstdout, 0xEC);
//std::cout << "Press any key to quit.\n";

//// Draw pretty colors until the user presses any key
//while (WaitForSingleObject(hstdin, 100) == WAIT_TIMEOUT)
//{
//	SetConsoleTextAttribute(hstdout, colors[index]);
//	std::cout << "\t\t\t\t Hello World \t\t\t\t" << std::endl;
//	if (++index > sizeof(colors) / sizeof(colors[0]))
//		index = 0;
//}
//FlushConsoleInputBuffer(hstdin);

//// Keep users happy
//SetConsoleTextAttribute(hstdout, csbi.wAttributes);

*/