#include "functions.h"
#include "toHTML.h"

void to_bold (bool& b)
{
  if (b) { printf("</b>"); b=0; }
  else { printf("<b>"); b=1; }
}

void to_italic (bool& i)
{
  if (i) { printf("</i>"); i=0; }
  else { printf("<i>"); i=1; }
}

void to_strike (bool& s)
{
  if (s) { printf("</del>"); s=0; }
  else { printf("<del>"); s=1; }
}

void to_code (bool& c)
{
  if (c) { printf("</code>"); c=0; }
  else { printf("<code>"); c=1; }
}


void bold_italic (char& c, bool& bd, bool& it, istream& ifile)
{
  char prev=c;
  c=ifile.peek();

  if (c!=EOF)
  {
    if (prev==c) { ifile.get(c); to_bold(bd); }
    else { to_italic(it); }
  }
  else { to_italic(it); }
}

void strike (char& c, bool& st, istream& ifile)
{
  char prev=c;
  c=ifile.peek();

  if (c!=EOF)
  {
    if (prev==c) { ifile.get(c); to_strike(st); }
    else { printf("%c", prev); }
  }
  else { printf("%c", c); }
}

void hbar (char& c, istream& ifile, std::string& prg, bool& pr)
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

      if (ifile.peek()=='\n') { printf("<hr>\n"); }
      else { printf("%s---", prg.c_str()); pr=1; }
    }
    else { printf("%s%c%c", prg.c_str(), prev, c); pr=1; }
  }
  else { printf("%s%c", prg.c_str(), prev); pr=1; }
}

void breakline (char& c, bool& pr, istream& ifile)
{
  char prev=c;
  c=ifile.peek();

  if (c!=EOF)
  {
    if (prev==c && pr) { ifile.get(c); printf("</p>\n\n"); pr=0; }
    else if (prev==c && !pr) { printf("<br>\n"); }
    else if (prev!=c && pr) { printf("<br>\n"); }
    else if (prev!=c && !pr) { printf("<br>\n"); }
  }
}

void newl (std::istream& ifile)
{
  if (ifile.peek()=='\n')
  {
    char c;
    ifile.get(c);

    if (ifile.peek()=='\n') { ifile.get(c); printf("\n\n"); }
    else { printf("<br>\n"); }
  }
}

void clean_src (string& src);
bool image (char& c, istream& ifile)
{
  char prev=c;
  c=ifile.peek();

  if (c=='[')
  {
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

    printf("<img src=\"%s\" alt=\"%s\" title=\"%s\" />", src.c_str(), alt, title);

    return true;
  }
  else { printf("%c", prev); return false; }
}

void url (char& c, istream& ifile)
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

  clean_src(href);

  printf("<a href=\"%s\" title=\"%s\">", href.c_str(), title);
  toHTML(iss_text, false);
  printf("</a>");
}

void str_pop (string& s, unsigned i);
bool legit_head (string& s, int& head);
void heading (istream& ifile, const char& align, bool& pr, const string& prg)
{
  string line; getline(ifile, line);
  int head;

  if (!legit_head(line, head))
  {
    printf("%s#%s", prg.c_str(), line.c_str());
    if (ifile.peek()=='\n') { char c; ifile.get(c); printf("</p>\n"); }
    else { printf("<br>\n"); pr=1; }
    return;
  }

  switch (align)
  {
    case 'l' : printf("<h%u align=\"left\">", head); break;
    case 'r' : printf("<h%u align=\"right\">", head); break;
    case 'c' : printf("<h%u align=\"center\">", head); break;
    case 'j' : printf("<h%u align=\"justify\">", head); break;
    default : printf("<h%u>", head);
  }

  str_pop(line, head);
  printf("%s</h%u>\n\n", line.c_str(), head);
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

void str_pop (string& s, unsigned i)
{
  string ret="";
  for (unsigned j=i; j<s.length(); j++) { ret+=s[j]; }
  s=ret;
}

bool legit_head (string& s, int& head)
{
  head=1;
  bool inhead=1;

  for (unsigned i=0; i<s.length(); i++)
  {
    if (s[i]=='#' && inhead==1) { head++; }
    else if (head>6) { return false; }
    else if (s[i]==' ' && inhead==1) { inhead=0; }
    else if (s[i]!='#' && inhead==1) { return false; }
    else if (inhead==0) { return true; }
  }

  return false;
}
