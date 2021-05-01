# 4d-plugin-archive
Utility based on libarchive

#### About this project

4D `zip` commands are based on [nih-at/libzip](https://github.com/nih-at/libzip).

`.4darchive` is a proprietary format that works with a `cache.xml` file, but will soon be replaced by `.zip`.

For standard archives, the native `zip` commands are sufficient; **there is no need for a plugin**.

The purpose of this project is to support `.7z` and other archive formats to store away large but highly compressable files such as `.4BK`.

The operation is typically performed on the server, require significant computation power and may take a while to complete.

Nevertheless its **impact on server performance** should be minimal.

#### Design principle

In order not to slow down the server during compression, and in order to be performant even if the application is running in interpretative mode, the plugin should perform compression in a **background thread**.

Instead of a callback method, the plugin should run asynchronously; the command should return a reference immediately, which can be used in 4D to query status at any time.

#### Technology

[libarchive](https://www.libarchive.org)

* Definition of hidden item on Mac:
 
  * `NSURL` `-getResourceValue:forKey:NSURLIsHiddenKeyerror:`
  * `path.at(0) == '.'`
  * `path.find("/.") != std::string::npos`

* Options for archive:
  * skipHidden (default=false) 
  * keepParent (default=false) 
  * format (default=.7z) 

* Supported formats: 

  * .7z
  * .zip = .jar
  * .cpio
  * .iso
  * .a = .ar
  * .tar
  * .tgz = .tar.gz
  * .tar.bz2
  * .tar.xz
