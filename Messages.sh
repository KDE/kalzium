#! /bin/sh
$EXTRACTRC src/*.ui src/*.rc src/*.kcfg >> rc.cpp || exit 11
$EXTRACTATTR --attr=name,origin data/data.xml >> rc.cpp || exit 12
#$EXTRACTRC --tag-group=none --tag=name data/data.xml >> rc.cpp || exit 13
$EXTRACTRC --tag-group=none --tag=name --tag=desc --tag=refitem data/knowledge.xml >> rc.cpp || exit 14
$EXTRACTRC --tag-group=none --tag=name --tag=desc data/tools.xml >> rc.cpp || exit 15
$XGETTEXT src/*.cpp -o $podir/kalzium.pot
