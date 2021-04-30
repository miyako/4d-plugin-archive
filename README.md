# 4d-plugin-archive
Utility based on libarchive

4D `zip` commands are based on [nih-at/libzip](https://github.com/nih-at/libzip).

`.4darchive` is a proprietary format that works with a `cache.xml`.

The purpose of this project is to support `.7z` and other archive formats to store away large but highly compressable files such as `.4BK`.

In order not to slow down the server while compression is ongoing, and even if the application is running in interpretative mode, the plugin should perform compression in a **background thread**.
