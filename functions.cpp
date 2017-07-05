#include "functions.h"

#include "toHTML.h"

void to_bold (bool& b, ostream& out)
{
  if (b) { out << "</b>"; b=0; }
  else { out << "<b>"; b=1; }
}

void to_italic (bool& i, ostream& out)
{
  if (i) { out << "</i>"; i=0; }
  else { out << "<i>"; i=1; }
}

void to_strike (bool& s, ostream& out)
{
  if (s) { out << "</del>"; s=0; }
  else { out << "<del>"; s=1; }
}

void to_code (bool& c, ostream& out)
{
  if (c) { out << "</code>"; c=0; }
  else { out << "<code>"; c=1; }
}


void bold_italic (char& c, bool& bd, bool& it, istream& ifile, ostream& out)
{
  char prev=c;
  c=ifile.peek();

  if (c!=EOF)
  {
    if (prev==c) { ifile.get(c); to_bold(bd, out); }
    else { to_italic(it, out); }
  }
  else { to_italic(it, out); }
}

void strike (char& c, bool& st, istream& ifile, ostream& out)
{
  char prev=c;
  c=ifile.peek();

  if (c!=EOF)
  {
    if (prev==c) { ifile.get(c); to_strike(st, out); }
    else { out << prev; }
  }
  else { out << c; }
}

void hbar (char& c, istream& ifile, ostream& out, std::string& prg, bool& pr)
{
  char prev=c;
  c=ifile.peek();

  if (prev==c)
  {
    ifile.get(c);
    char aft=ifile.peek();
    
    if (c==aft)
    {
      ifile.get(c);
      
      if (ifile.peek()=='\n') { out << "<hr>\n"; }
      else { out << prg << "---"; pr=1; }
    }
    else { out << prg << prev << c; pr=1; }
  }
  else { out << prg << prev; pr=1; }
}

void breakline (char& c, bool& pr, istream& ifile, ostream& out)
{
  char prev=c;
  c=ifile.peek();
  
  if (c!=EOF)
  {
    if (prev==c && pr) { ifile.get(c); out << "</p>\n\n"; pr=0; }
    else if (prev==c && !pr) { out << "<br>\n"; }
    else if (prev!=c && pr) { out << "<br>\n"; }
    else if (prev!=c && !pr) { out << "<br>\n"; }
  }
}

void newl (std::istream& ifile, std::ostream& out)
{
  if (ifile.peek()=='\n')
  {
    char c;
    ifile.get(c);
    
    if (ifile.peek()=='\n') { ifile.get(c); out << "\n\n"; }
    else { out << "<br>\n"; }
  }
}


void clean_src (string& src)
{
  unsigned l, b;
  string tmp="";

  for (unsigned i=src.length()-1; i>=0; i--)
  { 
    if (src[i]!=' ') { l=i; break; }
  }

  for (unsigned i=0; i<=l; i++)
  { 
    if (src[i]!=' ') { b=i; break; }
  }

  for (unsigned i=b; i<=l; i++)
  {
    if (src[i]==' ') { tmp+="%20"; }
    else { tmp+=src[i]; }
  }
  
  src=tmp;
}

bool image (char& c, istream& ifile, ostream& out)
{
  char prev=c;
  c=ifile.peek();

  if (c=='[')
  {
    out << "<img ";
    
    ifile.get(c);
    string src="";
    char alt[256], title[256];
    
    ifile.get(alt, 256, ']');
    ifile.get(c);
    
    while (c!='(' && c!=EOF) { ifile.get(c); }
    
    while (c!=')' && c!='"' && c!=EOF)
    {
      ifile.get(c);
      src+=c;
      c=ifile.peek();
    }
    ifile.get(c);
    
    if (c=='"')
    {
      c=ifile.peek();
      ifile.get(title, 256, '"');
      ifile.get(c);
    }
    while (c!=')' && c!=EOF) { ifile.get(c); }
    
    clean_src(src);
    
    out << "src=\"" << src << "\" alt=\"" << alt << "\" title=\"" << title << "\" />";
    
    return true;
  }
  else { out << prev; return false; }
}

void url (char& c, istream& ifile, ostream& out)
{
  char text[256], title[256];
  string href;

  ifile.get(text, 256, ']');
  while (c!='(' && c!=EOF) { ifile.get(c); }

  while (c!=')' && c!='"' && c!=EOF)
  {
    ifile.get(c);
    href+=c;
    c=ifile.peek();
  }

  ifile.get(c);
  if (c=='"')
  {
    ifile.get(title, 256, '"');
    ifile.get(c);
  }
  while (c!=')' && c!=EOF) { ifile.get(c); }

  istringstream iss_text(text);
  ostringstream oss_text; 

  toHTML(iss_text, oss_text, false);
  clean_src(href);

  out << "<a href=\"" << href << "\" title=\"" << title << "\">" << oss_text.str() << "</a>";
}
