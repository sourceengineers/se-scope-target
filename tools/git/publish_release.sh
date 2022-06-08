#!/usr/bin/env bash

if [ $# -eq 0 ]
  then
    echo "Publish release failed. Pass the path to the se-scope-target repository as argument to this script."
    echo "Example:"
    echo ">  ./init_remote.sh ../../"
    exit 1
fi

scope_target_path="$1"
if [[ -d "$scope_target_path" ]]
then
    echo "se-scope-target git repo at $scope_target_path"
else
    echo "Path at $scope_target_path does not exist"
    exit 1
fi

scope_protocol_path="$scope_target_path/ext/protocol"
if [[ -d "$scope_protocol_path" ]]
then
    echo "se-scope-protocol git repo at $scope_protocol_path"
else
    echo "Path at $scope_protocol_path does not exist"
    exit 1
fi

se_lib_c="$scope_target_path/ext/se-lib-c"
if [[ -d "$se_lib_c" ]]
then
    echo "se-scope-protocol git repo at $se_lib_c"
else
    echo "Path at $se_lib_c does not exist"
    exit 1
fi

git -C "$scope_target_path" checkout release
git -C "$scope_protocol_path" checkout release
git -C "$se_lib_c" checkout release

git -C "$scope_target_path" push -u github release
git -C "$scope_protocol_path" push -u github release
git -C "$se_lib_c" push -u github release
