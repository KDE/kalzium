#!/usr/bin/env ruby1.8
#
# Ruby script for generating Kalzium tarball releases from SVN
#
# (c) 2005 Mark Kretschmann <markey@web.de>
# modified by Carsten Niehaus
# Some parts of this code taken from cvs2dist
# License: GPL V2


# Ask user for app version 
version  = `kdialog --inputbox "Kalzium version: "`.chomp

name     = "kalzium"
folder   = "kalzium-#{version}"
doi18n   = "no"
dogpg    = "yes"


# Some helper methods
def svn( command, dir )
	#	puts "svn: " + command + #{svnprepend} + dir
    `svn #{command} svn://anonsvn.kde.org/home/kde/#{dir}`
end

def svnQuiet( command )
    `svn #{command} > /dev/null 2>&1`
end

def svnup( dir )
    `svn up #{dir}`
end


# Prevent using unsermake
oldmake = ENV["UNSERMAKE"]
ENV["UNSERMAKE"] = "no"

# Remove old folder, if exists
`rm -rf #{folder} 2> /dev/null`
`rm -rf folder.tar.bz2 2> /dev/null`

Dir.mkdir( folder )
Dir.chdir( folder )

puts "Entering "+`pwd`

svn( "co -N", "/trunk/KDE/kdeedu/" )
svn( "co", "/trunk/KDE/kde-common/" )
Dir.chdir( "kdeedu")

puts "Entering "+`pwd`
puts "Checking out libkdeedu"
svnup("libkdeedu")
puts "Checking out kalzium"
svnup("kalzium")
`cp -R ../kde-common/admin .`

# we check out kde-i18n/subdirs in kde-i18n..
if doi18n == "yes"
	puts "\n"
	puts "**** i18n ****"
	puts "\n"

    svn( "co -P kde-i18n/subdirs" )
    i18nlangs = `cat kde-i18n/subdirs`

    # docs
    for lang in i18nlangs
        lang.chomp!
        if FileTest.exists? "doc/#{lang}"
            `rm -Rf doc/#{lang}`
        end
        docdirname = "kde-i18n/#{lang}/docs/kdeextragear-1/amarok"
        svnQuiet( "co -P #{docdirname}" )
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
        svnQuiet( "co -P #{pofilename}" )
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

puts "Removing svn-history files (almost 10 megabyte)"
`find -name ".svn" | xargs rm -rf`

Dir.chdir( "kalzium" )

# Move some important files to the root folder
`mv TODO ..`
`mv RELEASE-PLAN ..`
`mv ChangeLog ..`
`mv release_kalzium.rb ..`

Dir.chdir( "src" )

# Exchange APP_VERSION string with new version
`cat main.cpp | sed -e 's/APP_VERSION \".*\"/APP_VERSION \"#{version}\"/' | tee main.cpp > /dev/null`

Dir.chdir( ".." ) # kalzium
`rm -rf debian`

Dir.chdir( ".." ) # kdeedu 
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
`rm -rf kde-common`
`rm -rf kdeedu` # after the moving of the directory this is empty
Dir.chdir( ".." ) # root folder
`tar -cf #{folder}.tar #{folder}`
`bzip2 #{folder}.tar`
`rm -rf #{folder}`
puts "done.\n"


ENV["UNSERMAKE"] = oldmake

if dogpg == "yes"
	`gpg --detach-sign #{folder}.tar.bz2`
end


puts "\n"
puts "====================================================="
puts "Congratulations :) Kalzium #{version} tarball generated.\n"
puts "\n"
puts "MD5 checksum: " + `md5sum #{folder}.tar.bz2`
if dogpg == "yes"
	puts "The user can verify this package with "
	puts "\n"
	puts "gpg --verify #{folder}.tar.bz2.sig #{folder}.tar.bz2"
end
puts "\n"
puts "\n"


