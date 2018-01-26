#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <random>
#include <sstream>
#include <stdio.h>
#include <algorithm>

//#define DEBUG

using namespace std;

random_device rng;

pair<string, string> getRandomCard(vector< pair<string, string> > &cards) {
int num = rng() % cards.size();
return cards[num];
}

// get the cards in random order, but get all of them before starting over with a new random order
// shuffles the passed vector
unsigned int current_card_index = 0; // pls don't modify this outside of this function
pair<string, string> getNextCard(vector< pair<string, string> > &cards) {
	if(current_card_index == 0){
		std::random_shuffle(std::begin(cards), std::end(cards));
	}
	current_card_index ++;
	// if the new index is too large, scale it back down. This should usually result in 0 and thus the next call to this function will shuffle again
	if ( current_card_index >= cards.size() ){
		current_card_index = current_card_index % cards.size();
	}
	return cards[current_card_index];
}

// constants used in the code
string DELIMITER = "\n\e[4m                                                  \e[0m\n";
string DELIMITER_WITHOUT_NEWLINES = "\e[4m                                                  \e[0m";
string LINE_COMMENT = "//";
// constants that you should update from time to time (manually)
string VERSION = "2.0.2";
string LAST_UPDATED = "Jan. 2018";
// arguments that are handled
string ARG_VERSION = "--version";
string ARG_RANDOM = "-r"; string ARG_SEMIRANDOM = "-R";

int getcard_version = 0; // set the version to either 0 (normal) or 1 (only show the same card again after having shown every other card)

// get either a random card or a semirandom card, depending on how getcard_version is set
pair<string, string> getCard(vector< pair<string, string> > &cards) {
	pair<string, string> ret = (getcard_version == 0) ? getRandomCard(cards) : getNextCard(cards);
	return ret;
}

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
	string fileName = "";
	if (argc > 1) {
		int count = 0;
		for (int i = 1; i < argc; i++) {
			// test for arguments other than card files
			if (argv[i] == ARG_VERSION) {
				std::cout << "Flashcard tool version " << VERSION << "\nLast updated "<<LAST_UPDATED<<std::endl;
				continue;
			
			}
			
			// toggle getRandomCard() or getNextCard()
			if (argv[i] == ARG_RANDOM) {
				getcard_version = 0;
			} else if (argv[i] == ARG_SEMIRANDOM){
				getcard_version = 1;
			}
			
			// if argument is probably a filename
			fileName = argv[i];
			ifstream afile(fileName);
			addCards(afile, cards);
			count += cards.size();
		}
		
		if(fileName == ""){
			std::cout << "No filename was specified." << std::endl;
			exit(0);
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
		pair<string, string> card = getCard(cards); // get (default) a random card or (if passed -R) a semirandom card (random, but never repeated unless no cards left)
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
