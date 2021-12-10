@ECHO off

pushd Vulpine\vendor

pushd benchmark
git pull origin main
popd

pushd glfw
git pull origin master
popd

pushd glm
git pull origin master
popd

popd