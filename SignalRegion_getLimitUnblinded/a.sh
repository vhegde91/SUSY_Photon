#!/bin/sh
anaArg=$1
dataSetType="-------"

echo $dataSetType

if [[ $anaArg=*"T5qqqq"* ]];then
    echo "got it"
    dataSetType='T5qqqqHg'
fi
echo "result"
echo $dataSetType

#string='My long string'
#if [[ $string = *"My long"* ]]; then
#  echo "It's there!"
#fi