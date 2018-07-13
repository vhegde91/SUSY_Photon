#!/bin/bash
for i in $(ls *.pdf)
do 
    pdfcrop --clip $i aa.pdf
    mv aa.pdf $i
done
