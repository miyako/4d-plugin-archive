/* --------------------------------------------------------------------------------
 #
 #	4DPlugin-archive.h
 #	source generated by 4D Plugin Wizard
 #	Project : archive
 #	author : miyako
 #	2021/04/29
 #  
 # --------------------------------------------------------------------------------*/

#ifndef PLUGIN_ARCHIVE_H
#define PLUGIN_ARCHIVE_H

#include "4DPluginAPI.h"
#include "4DPlugin-JSON.h"
#include "C_TEXT.h"

#include "archive.h"
#include "archive_entry.h"

#define LIBARCHIVE_BUFFER_SIZE 10240

#define DEBUG_RETURN_PATHS 0
#define DEBUG_RETURN_WARNINGS 0
#define DEBUG_RETURN_ERROR 0

#if VERSIONMAC
typedef std::string  uastring;
#else
typedef std::wstring uastring;
#endif
typedef std::vector<uastring>  uastrings;

typedef std::string  pathstring;
typedef std::vector<pathstring>  pathstrings;

#pragma mark -

static void archive_read(PA_PluginParameters params);
static void archive_write(PA_PluginParameters params);
static void archive_version(PA_PluginParameters params);

static int set_flags(PA_ObjectRef options);

static void collection_push(PA_CollectionRef c, const wchar_t *value);
static void collection_push(PA_CollectionRef c, const char *value);

static bool check_warning(bool processing, int r, struct archive *a, PA_CollectionRef warnings);
static bool check_eof(bool processing, int r, struct archive *a, PA_CollectionRef warnings, PA_ObjectRef status);
static int copy_data(struct archive *ar, struct archive *aw);

static FILE *ufopen(CUTF8String& path, const char *mode);

static bool set_file_symlink(struct archive_entry *f, uastring& absolute_path);
static bool set_file_info(struct archive_entry *f, uastring& absolute_path);
static bool set_file_size(struct archive_entry *f, FILE *fd);

static la_int64_t get_file_size(uastring& absolute_path);

static void set_pathname(struct archive_entry *f, PA_CollectionRef paths,  uastring& dstPath);

static FILE *open_path(struct archive_entry *f,
                       uastring& relative_path,
                       uastring& absolute_path);

static void get_path(C_TEXT& t, uastring& path);
static void get_folder_path(C_TEXT& t, uastring& path);

static int open_archive_src(int r, struct archive *a, C_TEXT& t, uastring& path);
static int open_archive_dst(int r, struct archive *a, C_TEXT& t, uastring& path);

#ifdef WIN32
#include <chrono>
#include <Shlobj.h>
#endif

static int wcs_to_utf8(uastring& wstr, pathstring& str);
static int utf8_to_wcs(pathstring& str, uastring& wstr);

static void unescape_path(pathstring &path);
static void unescape_path(uastring &path);

static void escape_path(pathstring &path);
static void escape_path(uastring &path);

static void get_subpaths(
                         uastring& spath,
                         pathstrings *relative_paths,
                         uastrings *absolute_paths,
                         bool skipHidden,
                         bool keepParent);

static void get_subpaths(
                         uastring& spath,
                         pathstrings *relative_paths,
                         uastrings *absolute_paths,
                         pathstring& folder_name,
                         bool skipHidden,
                         bool keepParent,
                         size_t absolutePathOffset);

static void get_subpaths(
                         uastring& spath,
                         pathstrings *relative_paths,
                         uastrings *absolute_paths,
                         bool skipHidden,
                         bool keepParent);

static void get_subpaths_colletion(PA_CollectionRef src,
                         pathstrings *relative_paths,
                         uastrings *absolute_paths,
                         bool skipHidden,
                         bool keepParent);

#endif /* PLUGIN_ARCHIVE_H */
