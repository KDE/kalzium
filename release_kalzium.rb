#!/usr/bin/env ruby1.8
#
# Ruby script for generating Kalzium tarball releases from SVN
#
# (c) 2005 Mark Kretschmann <markey@web.de>
# modified by Carsten Niehaus
# Some parts of this code taken from cvs2dist
# License: GPL V2


# Ask user for app version and CVS username
version  = `kdialog --inputbox "Kalzium version: "`.chomp
username = `kdialog --inputbox "SVN username: "`.chomp


name     = "kalzium"
folder   = "kalzium-#{version}"
doi18n   = "no"


# Some helper methods
def svn( command )
    `cvs -z3 #{command}`
end

def cvsQuiet( command )
    `cvs -z3 #{command} > /dev/null 2>&1`
end


# Prevent using unsermake
oldmake = ENV["UNSERMAKE"]
ENV["UNSERMAKE"] = "no"

# Remove old folder, if exists
`rm -rf #{folder} 2> /dev/null`
`rm -rf folder.tar.bz2 2> /dev/null`

Dir.mkdir( folder )
Dir.chdir( folder )

cvs( "co kdeextragear-1/doc/amarok" )
Dir.chdir( "kdeextragear-1" )
cvs( "co admin" )

puts "\n"
puts "**** i18n ****"
puts "\n"


# we check out kde-i18n/subdirs in kde-i18n..
if doi18n == "yes"
    cvs( "co -P kde-i18n/subdirs" )
    i18nlangs = `cat kde-i18n/subdirs`

    # docs
    for lang in i18nlangs
        lang.chomp!
        if FileTest.exists? "doc/#{lang}"
            `rm -Rf doc/#{lang}`
        end
        docdirname = "kde-i18n/#{lang}/docs/kdeextragear-1/amarok"
        cvsQuiet( "co -P #{docdirname}" )
        next unless FileTest.exists?( docdirname )
        print "Copying #{lang}'s #{name} documentation over..  "
        `cp -R #{docdirname} doc/#{lang}`

        # we don't want KDE_DOCS = AUTO, cause that makes the
        # build system assume that the name of the app is the
        # same as the name of the dir the Makefile.am is in.
        # Instead, we explicitly pass the name..
        makefile = File.new( "doc/#{lang}/Makefile.am", File::CREAT | File::RDWR | File::TRUNC )
        makefile << "KDE_LANG = #{lang}\n"
        makefile << "KDE_DOCS = #{name}\n"
        makefile.close

        puts( "done.\n" )
    end

    puts "\n"

    $subdirs = false
    Dir.mkdir( "po" )

    for lang in i18nlangs
        lang.chomp!
        pofilename = "kde-i18n/#{lang}/messages/kdeextragear-1/amarok.po"
        cvsQuiet( "co -P #{pofilename}" )
        next unless FileTest.exists? pofilename

        dest = "po/#{lang}"
        Dir.mkdir( dest )
        print "Copying #{lang}'s #{name}.po over..  "
        `cp #{pofilename} #{dest}`
        puts( "done.\n" )

        makefile = File.new( "#{dest}/Makefile.am", File::CREAT | File::RDWR | File::TRUNC )
        makefile << "KDE_LANG = #{lang}\n"
        makefile << "SUBDIRS  = $(AUTODIRS)\n"
        makefile << "POFILES  = AUTO\n"
        makefile.close

        $subdirs = true
    end

    if $subdirs
        makefile = File.new( "po/Makefile.am", File::CREAT | File::RDWR | File::TRUNC )
        makefile << "SUBDIRS = $(AUTODIRS)\n"
        makefile.close
    else
        `rm -Rf po`
    end

    `rm -rf kde-i18n`
end

puts "\n"

# Remove CVS relevant files
#`find -name "CVS" | xargs rm -rf`
#`find -name ".cvsignore" | xargs rm`

Dir.chdir( "kalzium" )

# Move some important files to the root folder
`mv AUTHORS ..`
`mv ChangeLog ..`
`mv COPYING ..`
`mv INSTALL ..`
`mv README ..`
`mv TODO ..`

Dir.chdir( "src" )

# Exchange APP_VERSION string with new version
`cat main.h | sed -e 's/APP_VERSION \".*\"/APP_VERSION \"#{version}\"/' | tee amarok.h > /dev/null`

Dir.chdir( ".." ) # amarok
`rm -rf debian`


Dir.chdir( ".." ) # kalziumbasedir for libkdeedu
puts( "\n" )

`find | xargs touch`


puts "**** Generating Makefiles..  "
`make -f Makefile.cvs`
puts "done.\n"

`rm -rf autom4te.cache`
`rm stamp-h.in`


puts "**** Compressing..  "
`mv * ..`
Dir.chdir( ".." ) # kalzium-foo
`rm -rf kdeextragear-1`
Dir.chdir( ".." ) # root folder
`tar -cf #{folder}.tar #{folder}`
`bzip2 #{folder}.tar`
`rm -rf #{folder}`
puts "done.\n"


ENV["UNSERMAKE"] = oldmake


puts "\n"
puts "====================================================="
puts "Congratulations :) Kalzium #{version} tarball generated.\n"
puts "Now follow the steps in the RELEASE_HOWTO, from\n"
puts "SECTION 3 onwards.\n"
puts "\n"
puts "Then drink a few pints and have some fun on #amarok\n"
puts "\n"
puts "MD5 checksum: " + `md5sum #{folder}.tar.bz2`
puts "\n"
puts "\n"


