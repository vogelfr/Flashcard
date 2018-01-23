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
string LINE_COMMENT = "//";
string VERSION = "2.0.2";
string ARG_VERSION = "--version";
string LAST_UPDATED = "Jan. 2017";

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
		cout << "\e[1;96m" << card.first << "\e[0m";
		cin.get();
		vector<string> lines = split(card.second, '|');
		int count = lines.size();
		for (string line : lines) {
		
			vector<string> chunks = split(line, '$');
			int chunkcount = chunks.size();
			for (string chunk : chunks) {
					cout << chunk;
					if (chunkcount > 1) {
						chunkcount--;
						cout << '\t';
						if (cin.peek() == '\n') {
							//TODO: Somehow make it print on the same line....
						}
					}
			}

			if (count > 1) {
				count--;
				cin.get();
			}	
		}
		cout << DELIMITER;
	}

	return 0;
}
