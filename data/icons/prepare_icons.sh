#!/bin/bash

SMALL_SIZE_W=75
SMALL_SIZE_H=45

BIG_SIZE_W=128
BIG_SIZE_H=80

WIND_DIR_SIZE_W=50

#PATH_ICONS_SRC="$PWD/weathericons_src" ## pierwszy zestaw ikon
PATH_ICONS_SRC="$PWD/accuweather2_src"  ## drugi zestaw ikon
PATH_ICONS_WIND_SRC="$PWD/winddirection_src"  ## ikony przedstawiające kierunek wiatru
PATH_ICONS_SMALL="$PWD/icons_small"
PATH_ICONS_BIG="$PWD/icons_big"

rm $PATH_ICONS_SMALL/*
rm $PATH_ICONS_BIG/*

# Przygotowanie ikon przedstawiających pogodę (mały i duzy rozmiar)
for fullfile in $PATH_ICONS_SRC/*.svg; do
	filename=$(basename -- "$fullfile")
	extension="${filename##*.}"
	filename="${filename%.*}"
    	echo "fullfile: $fullfile filename: $filename extension: $extension"
	inkscape -z -e "$PATH_ICONS_SMALL/$filename.png" -h $SMALL_SIZE_H $fullfile
	convert "$PATH_ICONS_SMALL/$filename.png" -threshold 100% "$PATH_ICONS_SMALL/bw_$filename.png"

	inkscape -z -e "$PATH_ICONS_BIG/$filename.png" -h $BIG_SIZE_H $fullfile
	convert "$PATH_ICONS_BIG/$filename.png" -threshold 100% "$PATH_ICONS_BIG/bw_$filename.png"
done

# Przygotowanie ikon przedstawiających kierunek wiatru
for fullfile in $PATH_ICONS_WIND_SRC/*.svg; do
	filename=$(basename -- "$fullfile")
	extension="${filename##*.}"
	filename="${filename%.*}"
    	echo "fullfile: $fullfile filename: $filename extension: $extension"
	inkscape -z -e "$PATH_ICONS_SMALL/$filename.png" -h $WIND_DIR_SIZE_W $fullfile
	convert "$PATH_ICONS_SMALL/$filename.png" -threshold 100% "$PATH_ICONS_SMALL/bw_$filename.png"
done
