#include "toHTML.h"
#include "functions.h"

std::string par (const char& al)
{
  switch (al)
  {
    case 0 : return "<p>";
    case 'l' : return "<p align=\"left\">";
    case 'r' : return "<p align=\"right\">";
    case 'c' : return "<p align=\"center\">";
    case 'j' : return "<p align=\"justify\">";
    default : return "<p>";
  }
}

void toHTML (istream& ifile, ostream& out, const bool& pr_on, char align, char align_image)
{
  char c;
  bool bd=0;
  bool it=0;
  bool st=0;
  bool cd=0;
  bool pr=1;
  string prg=par(align);
  string prgi=par(align_image);

  if (pr_on) { out << prg; }

  while (ifile.get(c))
  {
    switch (c)
    {
      case '*' :
      case '_' :
      {
        if (!pr && pr_on) { out << prg; pr=1; }
        bold_italic(c, bd, it, ifile, out);
      } break;

      case '~' :
      {
        if (!pr && pr_on) { out << prg; pr=1; }
        strike(c, st, ifile, out);
      } break;

      case '`' :
      {
        if (!pr && pr_on) { out << prg; pr=1; }
        to_code(cd, out);
      } break;

      case '-' :
      {
        if (!pr) { hbar(c, ifile, out, prg, pr); }
        else { out << c; }
      }break;

      case '\n' : { breakline(c, pr, ifile, out); } break;

      case '!' :
      {
        if (!pr && pr_on) { out << prgi; }
        if (!image(c, ifile, out)) { break; }
        if (!pr && pr_on) { out << "</p>"; newl(ifile, out); }
      } break;

      case '[' :
      {
        if (!pr && pr_on) { out << prg; }
        url(c, ifile, out);
        if (!pr && pr_on) { out << "</p>"; newl(ifile, out); }
      } break;

      case '#' :
      {
        if (!pr && pr_on) { heading(ifile, out, align, pr, prg); }
        else { out << c; }
      } break;

      default :
      {
        if (!pr && pr_on) { out << prg; pr=1; }
        out << c;
      }
    }
  }

  if (pr && pr_on) { out << "</p>\n"; }
}
