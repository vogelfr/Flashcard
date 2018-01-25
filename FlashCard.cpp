#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <random>
#include <sstream>
#include <stdio.h>

//#define DEBUG

using namespace std;

random_device rng;

pair<string, string> getRandomCard(vector< pair<string, string> > &cards) {
int num = rng() % cards.size();
return cards[num];
}

string DELIMITER = "\n\e[4m                                                  \e[0m\n";
string DELIMITER_WITHOUT_NEWLINES = "\e[4m                                                  \e[0m";
string LINE_COMMENT = "//";
string VERSION = "2.0.2";
string ARG_VERSION = "--version";
string LAST_UPDATED = "Jan. 2018";

vector<string> &splits(const std::string &s, char delim, std::vector<std::string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const std::string &s, char delim) {
    vector<string> elems;
    splits(s, delim, elems);
    return elems;
}

void addCards(ifstream &file, vector< pair<string, string> > &cards) {
string key, value;
while (getline(file, key)) {
	// if it starts with a line comment "//", ignore that line completely
	if(key.substr(0,LINE_COMMENT.size())==LINE_COMMENT){
		continue;
	}
	// else get the value to that key. But if it is a comment, skip until it is not
	do{
		getline(file, value);
	} while (value.substr(0,LINE_COMMENT.size())==LINE_COMMENT);

cards.emplace_back(key,value);
#ifdef DEBUG
cout << "Added card pair (" << key << ", " << value << ")" << endl;
#endif

}
}

int main(int argc, char *argv[]) {
	vector< pair<string, string> > cards;
	string fileName;
	if (argc > 1) {
		int count = 0;
		for (int i = 1; i < argc; i++) {
			// test for arguments other than card files
			if (argv[i] == ARG_VERSION) {
				std::cout << "Flashcard tool version " << VERSION << "\nLast updated "<<LAST_UPDATED<<std::endl;
				// if there was no file specified after/before --version, exit
				if(argc < 3){
					exit(0);
				}
				continue;
			
			}
			// if argument is probably a filename
			fileName = argv[i];
			ifstream afile(fileName);
			addCards(afile, cards);
			count += cards.size();
		}
		cout << "Successfully added " << cards.size() << " cards!" << endl;
	} else {
		cout << "Please enter the name of a flashcard file!" << endl;
		cin >> fileName;
		while (fileName != "c") {
			ifstream afile(fileName);
			addCards(afile, cards);
			cout << "Successfully added "<< cards.size() <<" cards! Enter c to continue or the name of another file!" << endl;
			cin >> fileName;
		}
		cin.get();
	}
	cout << "Beginning training!!" << DELIMITER;
	while (true) {
		pair<string, string> card = getRandomCard(cards);
		cout << "\e[1;96m" << card.first << "\e[0m"; // escape characters for bold text
		cin.get();
		vector<string> lines = split(card.second, '|');
		int count = lines.size();
		bool firstLineInCard = true; // used to make sure that no card key is overwritten by the tab mechanism for $. This is not about the file lines but about lines within the displayed card.
		for (string line : lines) {
			vector<string> chunks = split(line, '$');
			int chunkcount = chunks.size();
			string lineToPrint = ""; // used to rewrite the line after the user pressed enter to show the words after \t
			bool firstChunkInLine = true;
			for (string chunk : chunks) {
					lineToPrint.append(chunk);
					
					if (chunkcount > 1) {
						chunkcount--;
						lineToPrint.append("\t");
					}
					// delete the current line and write it anew
					// prepend "\033[A" to also clear the previous line
					// prepend \33[2K to clear the current line
					// prepend \r to return the cursor to the start of the current line
					string prepend = "";
					string append = "";
					if(firstLineInCard){
						// only rewrite the current line, unless this is not the first chunk.
						// but then, print also a newline because otherwise it will look ugly (it would move the line upwards after the first chunk)
						if(firstChunkInLine){
							prepend = "\33[2K\r";
							append = "\n\033[A"; // see above description: adds an empty line on the bottom but keeps the cursor on top
						} else {
							// same as if not first line
							prepend = "\033[A\33[2K\r";
						}
					} else {
						if(firstChunkInLine){
							prepend = "";
							append = "\n\033[A"; // see above description: adds an empty line on the bottom but keeps the cursor on top
						} else {
							// rewrite the previous line because there was a newline. I don't know where that newline is added, maybe it's directly from the flashcard inputfile.
							prepend = "\033[A\33[2K\r";
						}
					}
					
					cout << prepend << lineToPrint << append << std::flush;
					firstChunkInLine = false;
					cin.get();

			}
			//cout << std::endl;
			firstLineInCard = false;
			count--;
		}
		cout << DELIMITER_WITHOUT_NEWLINES << "\n";// TODO: without newline.
	}

	return 0;
}
