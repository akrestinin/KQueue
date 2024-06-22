# KQueue

## A fast and lightweight queue data structure written on C

**KQueue** project is an implementation of a queue data structure in pure C language. The project is designed using best practices (which are used, for example, in FreeRTOS) to create reliable and fast code. The project provides you:

* Fully tested and documented code
* Ability to build with CMake and Ninja

This queue data structure implementation has:

* An opaque queue data type
* Ability to create both statically and dynamically allocated objects
* All standard queue operations such as *Push, Pop etc.*

## Why KQueue?

The main goal of the **KQueue** project is to provide a flexible, easy-to-use, and hard-to-break implementation of the queue data structure. This is achieved by using opaque types and dummy structures. In this way, the data is hidden and user code cannot directly change the data structure due to a compilation error, but can create an object in both dynamic and static memory.

## How to build?

The **KQueue** project is initially built using *CMake*, *GCC* and *Ninja*. Therefore, this is the preferred build type. First, make sure you have all the necessary components installed. To do this, simply run the following commands:

```shell
make --version
cmake --version
gcc --version
ninja --version
```

If at any point you get an error, consider installing the missing software component.

### Using CLI

1) From the root project directory create build directory:

```shell
md build
cd build
```

Within the build directory, cmake must be configured before any build commands can be called. Therefore, we need to call the procedure to configure cmake:

```shell
cmake --preset configurePresetName
```

Here is the list of available configure presets:

* *Debug* - compile with the `-Og` flag for better debugging;
* *Release* - compile with the `-O2` flag for stable releases;
* *ReleaseMaxPerformance* - compilation with `-O2 -DNDEBUG -DEXPOSE_DATATATYPES` flags for maximum performance.

NOTE: the `ReleaseMaxPerformance` preset exposes data types to both compiler and user code. Thus, data encapsulation is broken. Also, all assertions are disabled in this preset. Therefore, if your implementation is not performance sensitive, it is better to use the `Release` configuration preset.

After successful CMake configuration you can build project using:

```shell
cmake --build --preset buildPresetName
```

For printing all available presets call:

```shell
cmake --list_presets    # Configure presets
cmake --build --list_presets # Build presets
```

### Using VSCode CMake extension

Before any actions you shgould install CMake extenson via *Extensions* tab in VSCode.

1) On the CMake extension tab, find the Configure option and select the desired configuration preset;
2) Find the Build option and select the build preset;
3) After all settings, find the Project outline tab with the target project and click build.

### Run builded

After the project has been successfully built, you can run the created executable file. This will start the testing procedure. To run the executable file go to `./build/buildPresetName/k_queue.exe`.

The test results should be similar to these:

```shell
Starting KQueue testing procedure...
--------------------------------------------------
Queue creation:
        TEST 1: Crate too big SUCCESS
        TEST 2: Create normal SUCCESS
        TEST 3: Is empty SUCCESS
        TEST 4: No items SUCCESS
--------------------------------------------------
Queue push items:
        TEST 1: Pushing items
                Push item [0]: First: 27293     Second: 19912   Third: 7844     SUCCESS
                Push item [1]: First: 8171      Second: 22450   Third: 30714    SUCCESS
                Push item [2]: First: 4347      Second: 9818    Third: 6988     SUCCESS
                Push item [3]: First: 23220     Second: 18803   Third: 8005     SUCCESS
                Push item [4]: First: 31953     Second: 14232   Third: 31662    SUCCESS
                Push item [5]: First: 17943     Second: 1803    Third: 30484    SUCCESS
                Push item [6]: First: 5113      Second: 9534    Third: 27844    SUCCESS
                Push item [7]: First: 19103     Second: 24699   Third: 27277    SUCCESS
                Push item [8]: First: 7059      Second: 17471   Third: 23218    SUCCESS
                Push item [9]: First: 23263     Second: 28917   Third: 1871     SUCCESS
        TEST 2: Push too much SUCCESS
        TEST 3: Is not empty SUCCESS
        TEST 4: Full of items SUCCESS
--------------------------------------------------
Queue pop items:
        TEST 1: Poping items 
                Poped item [0]: First: 27293    Second: 19912   Third: 7844     SUCCESS
                Is equal: SUCCESS
                Poped item [1]: First: 8171     Second: 22450   Third: 30714    SUCCESS
                Is equal: SUCCESS
                Poped item [2]: First: 4347     Second: 9818    Third: 6988     SUCCESS
                Is equal: SUCCESS
                Poped item [3]: First: 23220    Second: 18803   Third: 8005     SUCCESS
                Is equal: SUCCESS
                Poped item [4]: First: 31953    Second: 14232   Third: 31662    SUCCESS
                Is equal: SUCCESS
                Poped item [5]: First: 17943    Second: 1803    Third: 30484    SUCCESS
                Is equal: SUCCESS
                Poped item [6]: First: 5113     Second: 9534    Third: 27844    SUCCESS
                Is equal: SUCCESS
                Poped item [7]: First: 19103    Second: 24699   Third: 27277    SUCCESS
                Is equal: SUCCESS
                Poped item [8]: First: 7059     Second: 17471   Third: 23218    SUCCESS
                Is equal: SUCCESS
                Poped item [9]: First: 23263    Second: 28917   Third: 1871     SUCCESS
                Is equal: SUCCESS
        TEST 2: Pop too much SUCCESS
        TEST 3: Is empty SUCCESS
        TEST 4: No items SUCCESS
--------------------------------------------------
Queue random operations check:
        TEST 1: Random operations [100] SUCCESS
--------------------------------------------------
KQueue testing procedure completed!
```

## How to include **KQueue** in you project?

There are several ways of including **KQueue** in your project:

1) Copy files from *src* directory and place them in your project
2) Using *git submodules*
3) As a *static library*
4) As a *dynamic* library

**NOTE**: 3) and 4) aren't written in current CMake file, so you need to do it by yourself.

## Contributing

If you have found a bug or other error and want to help me with it, feel free to do so! Please don't forget to describe the problem you are solving and the way you solved it. Also keep the same code style as in the whole project, there is a *.clang-format* file to help you with this.

## Coming soon and knwon issues
