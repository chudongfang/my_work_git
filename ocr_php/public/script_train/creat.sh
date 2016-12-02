#!/bin/bash
tesseract xi.ha.exp0.tif xi.ha.exp0 -l $1 box.train
unicharset_extractor xi.ha.exp0.box
echo ha 0 0 0 0 0 >> font_properties
shapeclustering -F font_properties -U unicharset xi.ha.exp0.tr
mftraining -F font_properties -U unicharset -O xi.unicharset xi.ha.exp0.tr 
cntraining xi.ha.exp0.tr
mv inttemp xi.inttemp 
mv pffmtable xi.pffmtable
mv normproto xi.normproto
mv shapetable xi.shapetable
combine_tessdata xi.
rm xi.inttemp
rm xi.unicharset
rm xi.pffmtable
rm xi.shapetable
rm xi.normproto
rm font_properties
rm unicharset
mv xi.traineddata  $2.traineddata
cp $2.traineddata /usr/local/share/tessdata/
