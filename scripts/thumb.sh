#! /bin/bash

COUNT=0
SIZE=150
NBPL=7

type -P convert &>/dev/null || { echo "I require convert but it's not installed.  Aborting." >&2; exit 1; }
type -P mogrify &>/dev/null || { echo "I require mogrify but it's not installed.  Aborting." >&2; exit 1; }

if [ $# -ge 1 ];
then
    SIZE=$1
fi

if [ $# -ge 2 ];
then
    NBPL=$2
fi
let LAST=NBPL-1

if [ ! -e thumbs ];
then
    mkdir thumbs;
fi

cat << EOF > index.html
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Index</title>
    </head>

    <body>
        <p style="font-size: 24px;">
EOF

for ENTRY in *; do
    if [ ! -d $ENTRY -o x"$ENTRY" = "xthumbs" ]; then
        continue
    fi

    TITLE=`echo $ENTRY | sed 's/_/ /g' | perl -ane ' foreach $wrd ( @F ) { print ucfirst($wrd)." "; } ; '`

    echo "            <a href=\"$ENTRY.html\">$TITLE</a><br />" >> index.html

    cat << EOF > $ENTRY.html
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>$TITLE</title>
        <style type="text/css">
            img {
                border: none;
            }
            table {
                margin: auto;
            }
            td {
                padding: 10px;
            }
        </style>
    </head>

    <body>
        <table>
EOF
    COUNT=0
    for i in $(find $ENTRY -name '*.jpg' -or -name '*.png' | sort); do
        echo "Generating thumbnail for $i"
        MD5=$(md5sum $i | cut -d ' ' -f 1)

        if [ ! -e thumbs/$MD5 ]; then
            convert -thumbnail $SIZE $i thumbs/$MD5
            mogrify -quality 85 thumbs/$MD5
        fi

        let MOD=COUNT%NBPL
        if [ $MOD = 0 ];
        then
            echo "            <tr>" >> $ENTRY.html
        fi

        echo "                <td><a href=\"$i\"><img src=\"thumbs/$MD5\" title=\"$i\" alt=\"$i\" /></a></td>" >> $ENTRY.html

        if [ $MOD = $LAST ];
        then
            echo "            </tr>" >> $ENTRY.html
        fi

        let COUNT=COUNT+1
    done

    let MOD=COUNT%NBPL

    if [ $MOD != 0 ];
    then
        while [ $MOD -lt $NBPL ]; do
            echo "                <td>&nbsp;</td>" >> $ENTRY.html;
            let MOD=MOD+1;
        done

        echo "            </tr>" >> $ENTRY.html
    fi

    cat >> $ENTRY.html << EOF
        </table>
    </body>
</html>
EOF
done

cat << EOF >> index.html
        </p>
    </body>
</html>
EOF
