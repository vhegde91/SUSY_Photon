#!/bin/bash
#!-----------------------------------
#! Replace "oldtext" with "newtext"
#! inside all pdf files in the directory.
#! The old files are saved as *.pdf.bak
#!
#! Needs the pdftk package
#! http://www.pdflabs.com/tools/pdftk-the-pdf-toolkit/
#!                  Claudio xx-xxx-2012
#!    LANG=C hack   Claudio 09-mar-2017
#!-------------------------------------
#
oldtext=Preliminary
newtext=

for pdffile in T6ttZg_exclusion.pdf
do
   echo "Processing $pdffile..."
   cp $pdffile $pdffile.bak
   pdftk $pdffile output $pdffile.tmp uncompress
   rm $pdffile
   # the LANG=C hack may or may not be needed
   LANG=C && sed -i -e "s/$oldtext/$newtext/g" $pdffile.tmp
   #sed -i -e "s/$oldtext/$newtext/g" $pdffile.tmp
   pdftk $pdffile.tmp output $pdffile compress
   rm $pdffile.tmp
   rm $pdffile.tmp-e
done
