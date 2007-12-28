#! /bin/sh
$EXTRACTRC `find src -name \*.ui` src/*.rc src/*.kcfg >> rc.cpp || exit 11
$EXTRACTRC --tag-group=none --tag=name --tag=desc --tag=refitem data/knowledge.xml >> rc.cpp || exit 14
$EXTRACTRC --tag-group=none --tag=name --tag=desc data/tools.xml >> rc.cpp || exit 15
$XGETTEXT rc.cpp compoundviewer/*.cpp libavogadro-kalzium/src/*.cpp libavogadro-kalzium/src/engines/*.cpp libavogadro-kalzium/src/tools/*.cpp `find src -name \*.cpp` -o $podir/kalzium.pot
