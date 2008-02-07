#!/usr/bin/python
# 
# Copyright (C) 2008  Jan Boon (Kaetemi)
# 
# This file is part of OpenNeL Snowballs.
# OpenNeL Snowballs is free software; you can redistribute it and/or modify 
# it under the terms of the GNU General Public License as published by the 
# Free Software Foundation; either version 2 of the License, or (at your 
# option) any later version.
# 
# OpenNeL Snowballs is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 
# Public License for more details.
# 
# You should have received a copy of the GNU General Public License along 
# with OpenNeL Snowballs; if not, write to the Free Software Foundation, 
# Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# 
# $Id$
# 

import time, sys
from textwrap import TextWrapper

wrapper = TextWrapper()
indent = " * "
wrapper.initial_indent = indent
wrapper.subsequent_indent = indent
gmtime = time.gmtime(time.time())
filename = "NEEDED_FOR_buildLine"
newline = "\n"

print ""

print "This script will create .cpp and .h files for your class."
print "To use the defaults, simply hit ENTER, else type in the new value."

print ""

def askVar(name, default):
	sys.stdout.write(name + " (" + default + "): ")
	line = sys.stdin.readline()
	linestrip = line.strip()
	if linestrip == "":
		return default
	else:
		return linestrip

file = askVar("File", "your_class_name").replace(" ", "_")
classname = askVar("Class", "CYourClassName")
author = askVar("Author", "Jan Boon")
username = askVar("Username", "Kaetemi")
snowballs2 = askVar("Snowballs2", "False")
namespace = askVar("Namespace", "SBCLIENT")
product = askVar("Product", "OpenNeL Snowballs")
dir = askVar("Directory", "../client/src/")
year = askVar("Year", time.strftime("%Y", gmtime))
date = askVar("Date", time.strftime("%Y-%m-%d %H:%MGMT", gmtime))
hdefine = askVar("Define", namespace + "_" + file.upper() + "_H")

print ""

filedoc = [ ]
filedoc += [ "\\file {FILE}" ]
filedoc += [ "\\brief {CLASS}" ]
filedoc += [ "\\date {DATE}" ]
filedoc += [ "\\author {AUTHOR} ({USERNAME})" ]
filedoc += [ "{CLASS}" ]
filedoc += [ "" ]
filedoc += [ "$I" + "d$" ]

copyright = [ ]
copyright += [ "Copyright (C) {YEAR}  {AUTHOR} ({USERNAME})" ]
if snowballs2 != "False":
	copyright += [ "Based on NEVRAX SNOWBALLS, Copyright (C) 2001  Nevrax Ltd." ]
copyright += [ "" ]
copyright += [ "This file is part of {PRODUCT}." ]
copyright += [ "{PRODUCT} is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version." ]
copyright += [ "" ]
copyright += [ "{PRODUCT} is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details." ]
copyright += [ "" ]
copyright += [ "You should have received a copy of the GNU General Public License along with {PRODUCT}; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA." ]

hcode1 = [ ]
hcode1 += [ "#ifndef {HDEFINE}" ]
hcode1 += [ "#define {HDEFINE}" ]
hcode1 += [ "#include <nel/misc/types_nl.h>" ]
hcode1 += [ "" ]
hcode1 += [ "// Project includes" ]
hcode1 += [ "" ]
hcode1 += [ "// NeL includes" ]
hcode1 += [ "" ]
hcode1 += [ "// STL includes" ]
hcode1 += [ "" ]
hcode1 += [ "namespace {NAMESPACE} {" ]
hcode1 += [ "" ]

hcomment = [ ]
hcomment += [ "\\brief {CLASS}" ]
hcomment += [ "\\date {DATE}" ]
hcomment += [ "\\author {AUTHOR} ({USERNAME})" ]
hcomment += [ "{CLASS}" ]

hcode2 = [ ]
hcode2 += [ "class {CLASS}"]
hcode2 += [ "{"]
hcode2 += [ "protected:"]
hcode2 += [ "	// pointers"]
hcode2 += [ "	// ..."]
hcode2 += [ "	" ]
hcode2 += [ "	// instances"]
hcode2 += [ "	// ..."]
hcode2 += [ "public:"]
hcode2 += [ "	{CLASS}();"]
hcode2 += [ "	virtual ~{CLASS}();"]
hcode2 += [ "}; /* class {CLASS} */"]
hcode2 += [ "" ]
hcode2 += [ "} /* namespace {NAMESPACE} */"]
hcode2 += [ "" ]
hcode2 += [ "#endif /* #ifndef {HDEFINE} */"]
hcode2 += [ "" ]
hcode2 += [ "/* end of file */" ]

cppcode = [ ]
cppcode += [ "#include <nel/misc/types_nl.h>"]
cppcode += [ "#include \"{FILENOEXT}.h\""]
cppcode += [ "" ]
cppcode += [ "// Project includes" ]
cppcode += [ "" ]
cppcode += [ "// NeL includes" ]
cppcode += [ "// #include <nel/misc/debug.h>"]
cppcode += [ "" ]
cppcode += [ "// STL includes" ]
cppcode += [ "" ]
cppcode += [ "using namespace std;" ]
cppcode += [ "// using namespace NLMISC;" ]
cppcode += [ "" ]
cppcode += [ "namespace {NAMESPACE} {" ]
cppcode += [ "" ]
cppcode += [ "{CLASS}::{CLASS}()" ]
cppcode += [ "{" ]
cppcode += [ "	" ]
cppcode += [ "}" ]
cppcode += [ "" ]
cppcode += [ "{CLASS}::~{CLASS}()" ]
cppcode += [ "{" ]
cppcode += [ "	" ]
cppcode += [ "}" ]
cppcode += [ "" ]
cppcode += [ "} /* namespace {NAMESPACE} */" ]
cppcode += [ "" ]
cppcode += [ "/* end of file */" ]

def buildLine(line):
	newline = line.replace("{PRODUCT}", product)
	newline = newline.replace("{YEAR}", year)
	newline = newline.replace("{DATE}", date)
	newline = newline.replace("{AUTHOR}", author)
	newline = newline.replace("{USERNAME}", username)
	newline = newline.replace("{CLASS}", classname)
	newline = newline.replace("{FILENOEXT}", file)
	newline = newline.replace("{FILE}", filename)
	newline = newline.replace("{HDEFINE}", hdefine)
	newline = newline.replace("{NAMESPACE}", namespace)
	return newline

def printComment(f, lines):
	for line in lines:
		if line == "":
			f.write(indent + newline)
		else:
			for subline in wrapper.wrap(buildLine(line)):
				f.write(subline + newline)

def writeHeader(f):
	f.write("/**" + newline)
	printComment(f, filedoc)
	f.write(" */" + newline)
	f.write(newline)
	f.write("/* " + newline)
	printComment(f, copyright)
	f.write(" */" + newline)
	f.write(newline)
def writeCode(f, code):
	for line in code:
		f.write(buildLine(line) + newline)

#note: need filename for buildLine
filename = file + ".cpp"
filepath = dir + filename
f = open(filepath, 'w')
writeHeader(f)
writeCode(f, cppcode)
f.close()

filename = file + ".h"
filepath = dir + filename
f = open(filepath, 'w')
writeHeader(f)
writeCode(f, hcode1)
f.write("/**" + newline)
printComment(f, hcomment)
f.write(" */" + newline)
writeCode(f, hcode2)
f.close()

print "Done."
