#!/bin/bash 
tesseract xi.ha.exp0.tif xi.ha.exp0 -l $1  makebox

java -jar ../jTessBoxEditor/jTessBoxEditor.jar
    
