#!/bin/bash
BASEDIR=$(pwd)
#touch $BASEDIR/new.txt
echo $1
#sed '/s/ *\*\////w $BASEDIR/new.txt' $1
#sed  's/ *\*\///' $1  |tee $BASEDIR/new.txt
sed -i.bak 's/ *\*\///' $1

#sed -i 's/ *\/\*/#/g' $BASEDIR/new.txt
#sed -i 's/ *{ 1,(Byte_t \*)\"//' $BASEDIR/new.txt
#sed -i 's/\"},//' $BASEDIR/new.txt
sed -i 's/ *\/\*/#/g' $1
sed -i 's/ *{ 1,(Byte_t \*)\"//' $1
sed -i 's/\"},//' $1
