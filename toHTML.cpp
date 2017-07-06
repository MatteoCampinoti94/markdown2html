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

void toHTML (istream& ifile, const bool& pr_on, char align, char align_image)
{
  char c;
  bool bd=0;
  bool it=0;
  bool st=0;
  bool cd=0;
  bool pr=1;
  string prg=par(align);
  string prgi=par(align_image);

  if (pr_on) { printf("%s", prg.c_str()); }

  while (ifile.get(c))
  {
    switch (c)
    {
      case '*' :
      case '_' :
      {
        if (!pr && pr_on) { printf("%s", prg.c_str()); pr=1; }
        bold_italic(c, bd, it, ifile);
      } break;

      case '~' :
      {
        if (!pr && pr_on) { printf("%s", prg.c_str()); pr=1; }
        strike(c, st, ifile);
      } break;

      case '`' :
      {
        if (!pr && pr_on) { printf("%s", prg.c_str()); pr=1; }
        to_code(cd);
      } break;

      case '-' :
      {
        if (!pr) { hbar(c, ifile, prg, pr); }
        else { printf("%c", c);; }
      } break;

      case '\n' :
      {
        breakline(c, pr, ifile);
      } break;

      case '!' :
      {
        if (!pr && pr_on) { printf("%s", prgi.c_str()); }
        if (!image(c, ifile)) { break; }
        if (!pr && pr_on) { printf("</p>"); newl(ifile); }
      } break;

      case '[' :
      {
        if (!pr && pr_on) { printf("%s", prg.c_str()); }
        url(c, ifile);
        if (!pr && pr_on) { printf("</p>"); newl(ifile); }
      } break;

      case '#' :
      {
        if (!pr && pr_on) { heading(ifile, align, pr, prg); }
        else { printf("%c", c);; }
      } break;

      default :
      {
        if (!pr && pr_on) { printf("%s", prg.c_str()); pr=1; }
        printf("%c", c);;
      }
    }
  }

  if (pr && pr_on) { printf("</p>\n"); }
}
