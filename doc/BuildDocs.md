Building the document tree

Command line (Lyx > Markdown)
lyx --export latex file.lyx
pandoc --no-wrap -f latex -t markdown file.tex > file.md

Command line (Markdown > Lyx)
pandoc --wrap=none  -f markdown -t latex file.md > file.tex && tex2lyx
file.tex && lyx file.lyx

About Files
Changes shoudl be made in these files which are then integrated into the
aboutdialog.ui in src/
About1 - First tab. has version number and release date
About2 - Keyboard shortcuts
About3 - Credits



