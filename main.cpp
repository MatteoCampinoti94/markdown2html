#include <fstream>
#include "toHTML.h"


void options (int argc, char* argv[], char& p_align, char& i_align, std::string& title, char& title_display);
void align (char& p, const string option);
bool check_file (std::string name);


int main(int argc, char* argv[])
{
  if (argc<=1) { printf("No file selected\n"); exit(1); }
  if (!check_file(argv[1])) { printf("File does not exist\n"); exit(2); }

  std::ifstream ifile(argv[1]);
  char align=0, align_image=0;
  std::string title=""; char title_display=0;

  options(argc, argv, align, align_image, title, title_display);

  printf("<html>\n");

  printf("<head>\n<title>%s</title>\n</head>\n\n", title.c_str());

  printf("<body>\n");

  if (title!="")
  {
    switch (title_display)
    {
      case 'l' : printf("<h1 align=\"left\">%s</h1>\n<br>\n\n", title.c_str()); break;
      case 'r' : printf("<h1 align=\"right\">%s</h1>\n<br>\n\n", title.c_str()); break;
      case 'c' : printf("<h1 align=\"center\">%s</h1>\n<br>\n\n", title.c_str()); break;
      case 'j' : printf("<h1 align=\"justify\">%s</h1>\n<br>\n\n", title.c_str()); break;
      default  : printf("<h1>%s</h1>\n\n", title.c_str()); break;
    }
  }

  toHTML(ifile, std::cout, true, align, align_image);
  printf("</body>\n</html>\n");

  ifile.close();

  return 0;
}


bool check_file (std::string name)
{
  std::ifstream file(name.c_str());

  if (file.fail()) { return false; }
  else { file.close(); return true; }
}


void align (char& p, const string option)
{
  if (option=="left") { p='l'; }
  else if (option=="right") { p='r'; }
  else if (option=="center") { p='c'; }
  else if (option=="justify") { p='j'; }
}

void options (int argc, char* argv[], char& p_align, char& i_align, std::string& title, char& title_display)
{
  std::string op;

  if (argc>2)
  {
    bool align_b=0;
    bool align_image_b=0;
    bool title_b=0;

    for (int i=2; i<argc; i++)
    {
      op=argv[i];

      if (op=="text" && align_b==0 && i<argc-1)
      {
        align(p_align, string(argv[i+1]));
        align_b=1;
        i+=1;
      }
      else if (op=="image" && align_image_b==0 && i<argc-1)
      {
        align(i_align, string(argv[i+1]));
        align_image_b=1;
        i+=1;
      }
      else if (op=="title" && title_b==0 && i<argc-1)
      {
        title=argv[i+1]; i+=1;

        if ((title=="display" || title=="display-left"|| title=="display-center"|| title=="display-right" || title=="display-justify") && i<argc-1)
        {
          title_display=title[8];
          title=argv[i+1];
          i+=1;
        }

        title_b=1;
      }
    }
  }
}
