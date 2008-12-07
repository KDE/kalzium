#! /bin/sh
$EXTRACTRC `find libavogadro-kalzium src -name \*.ui` src/*.rc src/*.kcfg >> rc.cpp || exit 11
$EXTRACTRC --tag-group=none --tag=name --tag=desc --tag=refitem data/knowledge.xml >> rc.cpp || exit 14
$EXTRACTRC --tag-group=none --tag=name --tag=desc data/tools.xml >> rc.cpp || exit 15
$XGETTEXT_QT `find libavogadro-kalzium -name \*.cpp` -o $podir/kalzium_qt.pot
$XGETTEXT rc.cpp compoundviewer/*.cpp `find src -name \*.cpp` -o $podir/kalzium.pot

