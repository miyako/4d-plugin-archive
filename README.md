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

#### Special Considerations

The symbolic link (`Chromium Embedded Framework.framework`) inside 4D is not processed correctly.

Tested with 4D v17.5 

* `440,682,781` original
* `2,667` items
* `107,242,571` bytes .7z
* `438,374,385` restored (archive utility, double click Finder) 
* fault: `Chromium Embedded Framework.framework` is missing

It contains dot-dot

```
Components/WebViewerCEF.bundle/Contents/Frameworks/Chromium Embedded Framework.framework
=>/Applications/4D v17.5/4D.app/Contents/Frameworks/Chromium Embedded Framework.framework
=>../Native Components/WebViewerCEF.bundle/Contents/Frameworks/Chromium Embedded Framework.framework
```
