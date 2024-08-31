# Unit Test

We use GoogleTest to perform Unit Test for our project. You can simply run
```bash
mkdir build
cd build
cmake ..
make
```

to set up the environment you need for performing unit test.

You can then use
```bash
./unittest
```
to check if every test passes. Feel free to add more test cases to keep our project robust


Make sure you add your *.cpp files in the `add_executable` for `unittest` so that the linker won't failed.

Note: it is possible to use file(GLOB ...) to add all files in a directory, but it is not recommended by CMake and the suggested way is to list all files.
