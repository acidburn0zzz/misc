#! /usr/bin/env python
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
    """experimental, remplacer accents par la lettre sans accent"""
    strname = re.sub("[éèêë]", "e_accent", strname) #va remplacer 2 fois, prob a cause de utf-8
    strname = re.sub("(e_accent){2}", "e", strname)
    strname = re.sub("[áàâä]", "a_accent", strname) #remplace les a par ea ?!
    strname = re.sub("e_accenta_accent", "a", strname)
    strname = re.sub("[úùûü]", "u_accent", strname) #remplace les u par eu ?!
    strname = re.sub("e_accentu_accent", "u", strname)
    strname = re.sub("[óòôö]", "o_accent", strname) #remplace les o par eo ?!
    strname = re.sub("e_accento_accent", "o", strname)
    strname = re.sub("[íìîï]", "i_accent", strname) #remplace les i par ei ?!
    strname = re.sub("e_accenti_accent", "i", strname)
    strname = re.sub("[ýÿ]", "y_accent", strname) #remplace les y par ey ?!
    strname = re.sub("e_accenty_accent", "y", strname)
    strname = re.sub("[ç]", "c_cedil", strname) #remplace les c par ec ?!
    strname = re.sub("e_accentc_cedil", "c", strname)
    """fin de la partie experimentale"""

    return strname

def cleanup_foldername(strname):
    """cleans up the given folder name
    folder name should start and end with an alphabet
    and can have only a-z, 0-9 and _ in between
    """
    valid_dir = re.compile(r"^[a-zA-z0-9_-]+$")
    invalid_stuff = re.compile(r"[^a-zA-z0-9_-]+")
    strname = string.upper(strname)
    strname = enlever_accents(strname)
    strname = re.sub("^[^\w]+","",strname) #trim the beginning
    strname = re.sub("[^\w]+$","",strname) #trim the end
    strname = invalid_stuff.sub("_",string.strip(strname)) #replace invalid stuff by _
    strname = re.sub("_+","_",strname) #squeeze continuous _ to one _
    if valid_dir.match(strname):
        return strname
    else:
        return ''

def cleanup_filename(strname):
    """cleans up for a file name
    file name rules are similar to folder names, but it
    can and must have one extension"""
    strname = string.lower(strname)
    (strfile, strext) = os.path.splitext(strname)
    strext = string.strip(strext)
    strfile = string.replace(strfile,'.','_')
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
        print "%s -> %s" % (f,c)
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
