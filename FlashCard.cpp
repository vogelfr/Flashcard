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
getline(file, value);
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
