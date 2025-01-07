idf_component_register(
    SRCS 
        "eOTA.c"
    
    INCLUDE_DIRS 
        "include"
    
    EMBED_FILES
        "src/ota.html"
    
    REQUIRES 
        app_update
)
