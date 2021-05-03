static void __archive_write(PA_PluginParameters params) {

    PackagePtr pParams = (PackagePtr)params->fParameters;
    
    PA_CollectionRef src = PA_GetCollectionParameter(params, 1);
    
    C_TEXT dst;
    dst.fromParamAtIndex(pParams, 2);
    
    PA_ObjectRef options = PA_GetObjectParameter(params, 3);
    PA_ObjectRef status  = PA_CreateObject();
    
    ob_set_b(status, L"success", false);
    
    bool skipHidden = false;
    bool keepParent = false;
    
    if(options) {
        
        //these flags can be explicitly SET

        if(ob_is_defined(options, L"skipHidden")) {
            skipHidden = ob_get_b(options, L"skipHidden");
        }
        
        if(ob_is_defined(options, L"keepParent")) {
            keepParent = ob_get_b(options, L"keepParent");
        }
        
    }
#if DEBUG_RETURN_WARNINGS
    PA_CollectionRef warnings = PA_CreateCollection();
#else
    void *warnings = NULL;
#endif
    
#if DEBUG_RETURN_PATHS
    PA_CollectionRef paths = PA_CreateCollection();
#endif
    
    pathstrings relative_paths;
    uastrings absolute_paths;
    
    get_subpaths_colletion(src,
                 &relative_paths,
                 &absolute_paths,
                 skipHidden,
                 keepParent);
        
    int flags = set_flags(options);
    
    //--end of preparation
 
    la_int64_t total_progress = 0;
    la_int64_t total_size     = 0;
    
    uastring dstPath;
    
    int r = ARCHIVE_OK;
    
    struct archive *a = archive_write_new();
    archive_write_disk_set_options(a, flags);
    archive_write_set_format_7zip(a);//default=7zip
    
    if(options) {
        CUTF8String ext;
        if(ob_get_s(options, L"format", &ext)) {
            archive_write_set_format_filter_by_ext(a, (const char *)ext.c_str());
        }
    
    }
    
    if(archive_format(a) == ARCHIVE_FORMAT_ZIP) {

        CUTF8String compression;
        if(ob_get_s(options, L"compression", &compression)) {
            if(compression == (const uint8_t *)"store") {
                archive_write_zip_set_compression_store(a);
            }
            if(compression == (const uint8_t *)"deflate") {
                archive_write_zip_set_compression_deflate(a);
            }
        }
    }
    
    /*
     
     list of formats
     
     .7z
     .zip = .jar
     .cpio
     .iso
     .a = .ar
     .tar
     .tgz = .tar.gz
     .tar.bz2
     .tar.xz
    
     */
    
    //TODO: run this part in a thread
    
    r = open_archive_dst(r, a, dst, dstPath);
            
    if(r == ARCHIVE_OK) {
        
        unsigned char buf[LIBARCHIVE_BUFFER_SIZE];
        struct archive_entry *f = archive_entry_new();
        
        bool processing = true;
        
        size_t count = relative_paths.size();
                
        for (size_t i = 0; i < count; ++i) {
            uastring absolute_path = absolute_paths.at(i);
            total_size += get_file_size(absolute_path);
#if VERSIONMAC
//            total_size += listxattr(absolute_path.c_str(), NULL, 0, XATTR_NOFOLLOW);
#endif
        }
        
        ob_set_n(status, L"total", total_size);
        
        for (size_t i = 0; i < count; ++i) {
            
            pathstring relative_path = relative_paths.at(i);
            uastring absolute_path = absolute_paths.at(i);
            
            FILE *fd = open_path(r, a, f, relative_path, absolute_path);

            r = archive_write_header(a, f);
            
            if(fd) {
                
                ssize_t xattr = set_file_xattr(f, absolute_path);
//                total_progress += xattr;
                
                if (r < ARCHIVE_OK) {
                    processing = check_warning(processing, r, a, warnings);
                    if(!processing) {
#if DEBUG_RETURN_ERROR
                        ob_set_s(status, L"error", archive_error_string(a));
#endif
                        break;
                    }
                }else{
                    size_t len = fread(buf, 1, LIBARCHIVE_BUFFER_SIZE, fd);
                    while ( len > 0 ) {
                        total_progress += len;
                        archive_write_data(a, buf, len);
                        len = fread(buf, 1, LIBARCHIVE_BUFFER_SIZE, fd);
                    }
                    
                }
                fclose(fd);
            }
//            item_count++;
#if DEBUG_RETURN_PATHS
            collection_push(paths, relative_path.c_str());
#endif
            archive_entry_clear(f);
        }
        
        if(total_progress == total_size) {
            ob_set_b(status, L"success", true);
        }
        
        archive_entry_free(f);

#if DEBUG_RETURN_WARNINGS
        if(PA_GetCollectionLength(warnings) != 0) {
            ob_set_c(status, L"warnings", warnings);
        }else{
            PA_DisposeCollection(warnings);
        }
#endif

#if DEBUG_RETURN_PATHS
        ob_set_c(status, L"paths", paths);
#endif
    }

    ob_set_n(status, L"progress", total_progress);

    archive_write_close(a);
    archive_write_free(a);
    
    PA_ReturnObject(params, status);
}

static void __archive_read(PA_PluginParameters params) {

    PackagePtr pParams = (PackagePtr)params->fParameters;
    
    C_TEXT src;
    src.fromParamAtIndex(pParams, 1);
    
    C_TEXT dst;
    dst.fromParamAtIndex(pParams, 2);
    
    PA_ObjectRef options = PA_GetObjectParameter(params, 3);
    PA_ObjectRef status  = PA_CreateObject();
    
    ob_set_b(status, L"success", false);

#if DEBUG_RETURN_WARNINGS
    PA_CollectionRef warnings = PA_CreateCollection();
#else
    void *warnings = NULL;
#endif
    
#if DEBUG_RETURN_PATHS
    PA_CollectionRef paths = PA_CreateCollection();
#else
    void *paths = NULL;
#endif
    
    int flags = set_flags(options);

    //--end of preparation
    
    la_int64_t total_progress = 0;
    la_int64_t total_size     = 0;
//    la_int64_t item_count     = 0;
    
    total_size = get_archive_file_size(options, src);
    
    ob_set_n(status, L"total", total_size);
    
    uastring dstPath;
    uastring srcPath;
    
    struct archive *a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    
    if(options) {
        CUTF8String passphrase;
        if(ob_get_s(options, L"passphrase", &passphrase)) {
            archive_read_add_passphrase(a, (const char *)passphrase.c_str());
        }
    }
    
    struct archive *e = archive_write_disk_new();
    archive_write_disk_set_options(e, flags);
    archive_write_disk_set_standard_lookup(e);
    
    int r = ARCHIVE_OK;

    struct archive_entry *f = NULL;
    
    get_folder_path(dst, dstPath);

    r = open_archive_src(r, a, src, srcPath);
         
    if (ARCHIVE_OK == r) {

        bool processing = true;
        
        while (processing) {
            
            r = archive_read_next_header(a, &f);
            
            processing = check_eof(processing, r, a, warnings, status);

            if(processing) {

                set_pathname(f, paths, dstPath);

                r = archive_write_header(e, f);
                
                if (r < ARCHIVE_OK) {
                    processing = check_warning(processing, r, a, warnings);
                    if(!processing) {
#if DEBUG_RETURN_ERROR
                        ob_set_s(status, L"error", archive_error_string(a));
#endif
                    }
                }else{
                    la_int64_t len = archive_entry_size(f);
                    if (len > 0) {
                        r = copy_data(a, e);

                        processing = check_warning(processing, r, a, warnings);
                        if(!processing) {
#if DEBUG_RETURN_ERROR
                            ob_set_s(status, L"error", archive_error_string(a));
#endif
                        }else{
                            total_progress += len;
                        }
                    }
                    r = archive_write_finish_entry(e);
                    processing = check_warning(processing, r, a, warnings);
                    if(!processing) {
#if DEBUG_RETURN_ERROR
                        ob_set_s(status, L"error", archive_error_string(a));
#endif
                    }
                }
            }
        }
        
        if(total_progress == total_size) {
            ob_set_b(status, L"success", true);
        }
        
#if DEBUG_RETURN_WARNINGS
        if(PA_GetCollectionLength(warnings) != 0) {
            ob_set_c(status, L"warnings", warnings);
        }else{
            PA_DisposeCollection(warnings);
        }
#endif
        
#if DEBUG_RETURN_PATHS
        ob_set_c(status, L"paths", paths);
#endif
    }
    
    ob_set_n(status, L"progress", total_progress);
    
    archive_read_close(a);
    archive_read_free(a);
    
    archive_write_close(e);
    archive_write_free(e);
    
    PA_ReturnObject(params, status);
}
