# markdown2html
*Console program to convert markdown syntax to html with extra functionalities written in C++*


## COMPILING
Only requirements to compile the program are a C++11 compiler and Linux system.
`g++ std=c++11 main.cpp toHTML.cpp functions.cpp`


## USAGE
*binary* [**FILE**] [**OPTIONS**]

Any plain text file will do, the program will go through it and try to recognize markdown syntax.

The options modify the formatting of the resulting html and are used in the following way: [**option**] [**comand**]

| Option | Possible commands | Result |
|:---:|:---:|---|
| `text`   | `left` `center` `right` `justify` | Aligns the text accordingly |
| `image`  | `left` `center` `right` `justify` | Aligns the images (if any) accordingly |
| `title`  | `display-left` `display-center` `display-right` `display-justify` | If any `display-` option is passed, display the title and align it accordingly, any different string after `title` and `display-` (if present) will be considered as the title itself (e.g `title display-center "markdown2html"` will set the title to 'markdown2html' and display it aligned to the center) |

The options can be given in any order but their specific command must follow them. For example `text justify image center` will have the same result as `image center text justify` but `text image center` won't pass any text aligning option, only for the image.

## RECOGNIZED SYNTAX

```markdown
**bold** __bold__
*italic* _italic_
~~strikethrough~~
**_~~mixed~~_**

# heading 1
## heading 2
### heading 3
#### heading 4
##### heading 5
###### heading 6

# *formatted headings*

`code`

--- horizontal bar

[link](url)
![image](url)
```

The program will also treat any newline as actual breaklines (if single '\n') or paragraph ends (if double '\n\n')
