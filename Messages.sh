#! /bin/sh
$EXTRACTRC `find src -name \*.ui` src/*.rc src/*.kcfg >> rc.cpp || exit 11
$EXTRACTRC --tag-group=none --tag=name --tag=desc --tag=refitem data/knowledge.xml >> rc.cpp || exit 14
$EXTRACTRC --tag-group=none --tag=name --tag=desc data/tools.xml >> rc.cpp || exit 15
$XGETTEXT -ktr:1,1t -ktr:1,2c,2t -kQT_TRANSLATE_NOOP:1c,2,2t -kQT_TR_NOOP:1,1t -ktranslate:1c,2,2t -ktranslate:2,3c,3t rc.cpp compoundviewer/*.cpp libavogadro-kalzium/src/*.cpp libavogadro-kalzium/src/engines/*.cpp libavogadro-kalzium/src/tools/*.cpp `find src -name \*.cpp` -o $podir/kalzium.pot
rm -f rc.cpp
