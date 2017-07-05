#include <fstream>
#include <iostream>

#include "toHTML.h"


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

void options (int ac, char* av[], char& p_align, char& i_align, std::string& title, char& title_display)
{
  std::string op;
  
  if (ac>2)
  {
    bool align_b=0;
    bool align_image_b=0;
    bool title_b=0;
    
    for (int i=2; i<ac; i++)
    {
      op=av[i];
      
      if (op=="text" && align_b==0 && i<ac-1)
      { 
        align(p_align, string(av[i+1]));
        align_b=1;
        i+=1;
      }
      else if (op=="image" && align_image_b==0 && i<ac-1)
      { 
        align(i_align, string(av[i+1]));
        align_image_b=1;
        i+=1;
      }
      else if (op=="title" && title_b==0 && i<ac-1)
      {
        title=av[i+1]; i+=1;
        
        if ((title=="display" || title=="display-left"|| title=="display-center"|| title=="display-right" || title=="display-justify") && i<ac-1)
        {
          title_display=title[8];
          title=av[i+1];
          i+=1;
        } 
        
        title_b=1;
      }
    }
  }
}


int main(int argc, char* argv[])
{
  if (argc<=1) { exit(1); }
  if (!check_file(string(argv[1]))) { exit(2); }
  
  std::ifstream ifile(argv[1]);
  char align=0, align_image=0, title_display=0;;
  std::string title="";

  options(argc, argv, align, align_image, title, title_display);

  std::cout << "<html>\n";

  std::cout << 
    "<head>\n"
    "<title>" << title << "</title>\n"
    "</head>\n\n";
  
  std::cout << "<body>\n";

  if (title!="")
  {
    switch (title_display)
    {
      case 'l' : std::cout << "<h1 align=\"left\">" << title << "</h1>\n<br>\n\n"; break;
      case 'r' : std::cout << "<h1 align=\"right\">" << title << "</h1>\n<br>\n\n"; break;
      case 'c' : std::cout << "<h1 align=\"center\">" << title << "</h1>\n<br>\n\n"; break;
      case 'j' : std::cout << "<h1 align=\"justify\">" << title << "</h1>\n<br>\n\n"; break;
      default : std::cout << "<h1>" << title << "</h1>\n\n"; break;
    }
  }
  
  toHTML(ifile, std::cout, true, align, align_image);
  std::cout << "</body>\n</html>\n";

  ifile.close(); 

  return 0;
}
