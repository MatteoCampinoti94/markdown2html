#include <iostream>
#include <sstream>

using namespace std;

void to_code (bool& c, ostream& out);

void bold_italic (char& c, bool& bd, bool& it, istream& ifile, ostream& out);
void strike (char& c, bool& st, istream& ifile, ostream& out);
void hbar (char& c, istream& ifile, ostream& out, std::string& prg, bool& pr);

void breakline (char& c, bool& pr, istream& ifile, ostream& out);
void newl (std::istream& ifile, std::ostream& out);

bool image (char& c, istream& ifile, ostream& out);
void url (char& c, istream& ifile, ostream& out);
