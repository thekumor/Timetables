mkdir build
Copy-Item src/ -Destination build/ -Recurse
Copy-Item data/ -Destination build/ -Recurse
cd build
cmake ..