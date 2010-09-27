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
        <title>Gallery</title>
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

for i in $(ls *.jpg *.png 2>/dev/null); do
    echo "Generating thumbnail for $i"
    convert -thumbnail $SIZE $i thumbs/$i;
    mogrify -quality '75%' thumbs/$i;

    let MOD=COUNT%NBPL
    if [ $MOD = 0 ];
    then
        echo "            <tr>" >> index.html
    fi

    echo "                <td><a href=\"$i\"><img src=\"thumbs/$i\" alt=\"$i\" /></a></td>" >> index.html

    if [ $MOD = $LAST ];
    then
        echo "            </tr>" >> index.html
    fi

    let COUNT=COUNT+1
done

let MOD=COUNT%NBPL

if [ $MOD != 0 ];
then
    while [ $MOD -lt $NBPL ]; do
        echo "                <td>&nbsp;</td>" >> index.html;
        let MOD=MOD+1;
    done

    echo "            </tr>" >> index.html
fi

cat << EOF >> index.html
        </table>
    </body>
</html>
EOF
