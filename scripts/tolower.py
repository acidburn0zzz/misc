#! /usr/bin/env python3
#-*- coding: utf-8 -*-

"""
A script to make all the file names in the folder
changed according to certain rules:
    all small letters
    has only a-z, 0-9 and _
    only one period

vsbabu AT hotmail DOT com

I use it fairly often to rename files sent to me by
Microsoft Windows users, before uploading them to
any unix machine.

Unhandled error conditions include:
- no graceful processing if a target filename already exists.
"""
import re
import string
import os

def enlever_accents(strname):
    strname = re.sub("[éèêë]", "e", strname)
    strname = re.sub("[áàâä]", "a", strname)
    strname = re.sub("[úùûü]", "u", strname)
    strname = re.sub("[óòôö]", "o", strname)
    strname = re.sub("[íìîï]", "i", strname)
    strname = re.sub("[ýÿ]", "y", strname)
    strname = re.sub("[ç]", "c", strname)
    strname = re.sub("[&]", "and", strname) 

    return strname

def cleanup_foldername(strname):
    """cleans up the given folder name
    folder name should start and end with an alphabet
    and can have only a-z, 0-9 and _ in between
    """
    valid_dir = re.compile(r"^[a-zA-z0-9_-]+$")
    invalid_stuff = re.compile(r"[^a-zA-z0-9_-]+")
    strname = strname.lower()
    strname = enlever_accents(strname)
    strname = re.sub("^[^\w]+","", strname) #trim the beginning
    strname = re.sub("[^\w]+$","", strname) #trim the end
    strname = invalid_stuff.sub("_", strname.strip()) #replace invalid stuff by _
    strname = re.sub("_+","_", strname) #squeeze continuous _ to one _
    if valid_dir.match(strname):
        return strname
    else:
        return ''

def cleanup_filename(strname):
    """cleans up for a file name
    file name rules are similar to folder names, but it
    can and must have one extension"""
    strname = strname.lower()
    (strfile, strext) = os.path.splitext(strname)
    strext = strext.strip()
    strfile = strfile.replace('.', '_')
    if strext == '' or strext == '.' or strext[0]!='.':
        return ''
    strfile = cleanup_foldername(strfile)
    if strfile == '':
        return ''
    strext = cleanup_foldername(strext[1:])
    if strext == '':
        return ''
    return strfile + '.' + strext

def  ren_file(dir,f):
    """rename the file according to the rules"""
    c=cleanup_filename(f)
    if (c != "" and c != f):
        print("%s -> %s" % (f,c))
        os.rename(dir+os.sep+f, dir+os.sep+c)
    return


def process_dir(dir):
    """process all files in the folder"""

    for f in os.listdir(dir):
        file = dir + os.sep + f
        if os.path.isdir(file):
            process_dir(file)
        else:
            ren_file(dir,f)

    return

def main():
    """main routine"""

    process_dir('.')
    return

if __name__=='__main__':
    main()
