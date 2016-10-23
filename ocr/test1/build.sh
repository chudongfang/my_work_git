#!/bin/bash

echo fontyp 0 0 0 0 0 >font_properties
tesseract $1.$2.exp0.tif $1.$2.exp0   box.train
unicharset_extractor $1.$2.exp0.box 
shapeclustering -F font_properties -U unicharset -O $1.unicharset $1.$2.exp0.tr
mftraining -F font_properties -U unicharset -O $1.unicharset $1.$2.exp0.tr
cntraining $1.$2.exp0.tr
mv normproto $1.normproto
mv inttemp $1.inttemp
mv pffmtable $1.pffmtable 
mv unicharset $1.unicharset
mv shapetable $1.shapetabl
combine_tessdata $1.
