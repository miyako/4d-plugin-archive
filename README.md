# 4d-plugin-archive
Utility based on libarchive

#### About this project

4D `zip` commands are based on [nih-at/libzip](https://github.com/nih-at/libzip).

`.4darchive` is a proprietary format that works with a `cache.xml`, but it will soon be replaced by `.zip`.

For standard archives, the native `zip` commands are sufficient; there is no need for a plugin.

The purpose of this project is to support `.7z` and other archive formats to store away large but highly compressable files such as `.4BK`.

The operation is typically performed on the server and may take a while to complete.

The impact of high CPU needed for performing compression on the server should be minimal.

#### Design principle

In order not to slow down the server while compression is ongoing, and even if the application is running in interpretative mode, the plugin should perform compression in a **background thread**.

Instead of a callback method, the plugin should run asynchronously; the command should return a reference immediately, which can be used in 4D to query status at any time.

#### Technology

[libarchive](https://www.libarchive.org)
