#include <sstream>

using namespace std;

void to_code (bool& c);

void bold_italic (char& c, bool& bd, bool& it, istream& ifile);
void strike (char& c, bool& st, istream& ifile);
void hbar (char& c, istream& ifile, std::string& prg, bool& pr);

void breakline (char& c, bool& pr, istream& ifile);
void newl (std::istream& ifile);

bool image (char& c, istream& ifile);
void url (char& c, istream& ifile);

void heading (istream& ifile, const char& align, bool& pr, const string& prg);
