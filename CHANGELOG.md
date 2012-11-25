### Version 0.4.8 - Unreleased

* Added @parent reference object
* Implemented attr() function
* Implemented visible property
* 5 and 7 digit colors are now supported
* Implemented color keywords (black, white, transparent)
* Implemented :firstChild, :lastChild, :even, :odd, :oddChild, :evenChild, and
  :parent filters
* Implemented :empty filter for elements
* Converted various platform-specific build systems to CMake
* Utilize CPack for automatically building distribution packages for all OSs
* Converted rendering and all other internals to use Qt
* Eliminated all third party dependencies (except the newly introduced Qt)
* Rearchitecture of how the library is used, and the ability for an application
  to render multiple AXR "documents" without requiring a separate thread for
  each document
* Partial support for weights on fonts
* Renamed "overflow" to "contained"
* xml-stylesheet now uses href instead of src
* Add ability to check AXR version at runtime
* Much improved parity across operating systems (no more AXRWrapper classes)
* Modularization of the project into several source repositories
* Massive reorganization of the source code structure
* More reliable XML parsing and file loading (no more 1 KB file size limit!)
* Doxygen docs now built in source tree
* Logging/debugging improvements
* Icons and QuickLook previews for HSS files on OS X
* Dozens of bug fixes
* Continuous Integration!

### Version 0.4.7 - 2012-06-17

* Add support for @polygon
* Improved readme to encourage participation
* Add support for the @root reference object
* Add support for a new contentAlignX value
* Fix many crashes and error messages
* Add "content" property (accepts strings to override the content text)
* Add basic infrastructure for layout tests
* Add documentation about most of the classes
* Make AXRCore a thread-specific singleton
* Implement selector grouping
* Add support for importing the HSS framework
* Minor fixes

### Version 0.4.6 - 2012-04-08

* Implement cloning on all HSS objects
* Implement flags
* Add flow property
* Add @click event (as a clone of mouseUp, for now)
* Add support for decimal values (eg. 1.257)
* Add support for #new(n) { }
* Add overflow property
* Massively improve layout algorithm in the secondary direction
