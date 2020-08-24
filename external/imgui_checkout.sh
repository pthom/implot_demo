#!/usr/bin/env bash

# This script checkouts external/imgui with a branch that mixes the docking and tables branches

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
REPO_DIR=$THIS_DIR/..
cd $REPO_DIR/external

if [ ! -d imgui ]; then
  git clone https://github.com/ocornut/imgui.git
fi

cd imgui
git merge --abort
git clean -fxd
git checkout docking
git pull --rebase
git merge origin/tables
echo "There might be some merge errors in the imgui examples files, this will have no consequence"
