#! /usr/bin/env bash
$EXTRACTATTR --context=Elementname --attr=label,value ../../../libkdeedu/libscience/data/elements.xml >>rc.cpp
$XGETTEXT *.cpp -o $podir/plasma_engine_kalzium.pot
