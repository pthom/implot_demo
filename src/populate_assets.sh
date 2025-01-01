#!/usr/bin/env bash

echo "Populating assets"

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $THIS_DIR/../external || exit 1
SRC_DIR=$THIS_DIR
EXTERNAL_DIR=$THIS_DIR/../external

echo "THIS_DIR=$THIS_DIR"
#exit 0

cd $THIS_DIR
mkdir -p assets/code/implot
cp -f $EXTERNAL_DIR/implot/implot_demo.cpp assets/code/implot/implot_demo.cpp

mkdir -p assets/code/implot3d
cp -f $EXTERNAL_DIR/implot3d/implot3d_demo.cpp assets/code/implot3d/implot3d_demo.cpp

