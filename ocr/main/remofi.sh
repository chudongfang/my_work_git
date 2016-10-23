#!/bin/bash
mv xi.traineddata  $1.traineddata
sudo cp $1.traineddata /usr/share/tesseract-ocr/tessdata/
