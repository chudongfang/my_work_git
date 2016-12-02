#!/bin/bash
mv xi.traineddata  $1.traineddata
sudo cp $1.traineddata /usr/share/tesseract-ocr/tessdata/
scp $1.traineddata root@115.159.205.168:/home/wwwroot/default/ocr_php/public/tessdata
scp $1.traineddata root@115.159.205.168:/usr/share/tesseract/tessdata/

