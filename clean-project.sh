find . -iwholename '*cmake*'  -not -path "./libs/gtest/*"  -not -path "./msgpack-c/*" -not -name CMakeLists.txt -delete
find . -iwholename '*Makefile'  -not -path "./libs/gtest/*"  -not -path "./msgpack-c/*" -not -name CMakeLists.txt -delete
find . -iwholename 'lib*'  -not -path "./libs/gtest/*"  -not -path "./msgpack-c/*" -not -name CMakeLists.txt -delete
find . -iwholename '*.a'  -not -path "./libs/gtest/*"  -not -path "./msgpack-c/*" -not -name CMakeLists.txt -delete
rm -rf gmock.pc gtest_main.pc gtest.pc gmock_main.pc Testing
