#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include <cctype>
#include <iomanip>

using namespace std;

int main() {
	string s;
	char buf[100];
	int total = 0, cursor = 0;
	ifstream ifs;
	set<string> stopwords;
	map<string, int> freq;
	multimap<int, string> sortedwords;

	ifs.open("stopwords.txt", ifstream::in);

	while (ifs.good()) {
		ifs.getline(buf, 100);
		s = buf;
		stopwords.insert(s);
		if (ifs.good())
			ifs.getline(buf, 100);
	}

	ifs.close();

	ifs.open("input.txt", ifstream::in);

	while (ifs.good()) {
		char *c, tempc;
		int f, e;
		ifs.get(tempc);

		if (tempc == ' ' || tempc == '\n') {
			buf[cursor] = '\0';
			s = "";
			for (c = buf; ispunct(*c); ++c);
			f = c - buf;
			for (c = buf + strlen(buf) - 1; c >= buf && ispunct(*c); --c);
			e = c - buf;
			if (e >= f) {
				for (int i = f; i <= e; ++i)
					s += tolower(buf[i]);

				if (stopwords.find(s) != stopwords.end()) {
					if (freq.find(s) != freq.end())
						freq.at(s) += 1;
					else
						freq.insert(make_pair(s, 1));
					++total;
				}
			}
			cursor = 0;
		}
		else
			buf[cursor++] = tempc;
	}

	ifs.close();

	map<string, int>::iterator it;

	for (it = freq.begin(); it != freq.end(); ++it)
		sortedwords.insert(make_pair(it->second, it->first));

	multimap<int, string>::reverse_iterator rit;

	for (rit = sortedwords.rbegin(); rit != sortedwords.rend(); ++rit) {
		cout << setw(10) << rit->second;
		cout << " ";
		cout << setw(3) << rit->first;
		cout << " %" << (float)rit->first * 100 / total << endl;
	}

	getchar();
	return 0;
}