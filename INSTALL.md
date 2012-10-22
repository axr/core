# WARNING!!! these instructions have not been completed, don't complain if they don't work for you

## Build instructions:

### Common for all of the platforms :

1.   Set up Git and configure GitHub [https://help.github.com/articles/set-up-git]

2.   Clone the repository:
 1.   Open the terminal and execute the following commands:
 2.   Go to the folder where you want the prototype folder to be created
    cd /path/to/directory'
 3.   Clone the repo:
    git clone git://github.com/AXR/Prototype.git'
 4.   Initialise git submodules
    git submodule update --init --recursive'
 4.   Download and install all required libraries:
  * Qt: [http://qt-project.org/downloads]

### OS X

Make sure you have Xcode installed, you can get it from Apple: http://itunes.apple.com/us/app/xcode/id497799835?mt=12

1.   Open the Xcode project (prototype/mac/AXR Test App.xcodeproj) and hit Run

### Linux

1.   Install the dependencies :
 *   Building dependency:
  *   CMake (2.8 +)

2. From the directory used earlier :
  1.    mkdir ../nameofdirectory
  2.    cd ../nameofdirectory
  3.    cmake ../nameofinitialdirectory
  4.    make

### Windows: coming soon...
