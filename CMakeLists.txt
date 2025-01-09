idf_component_register(
    SRCS 
        "eOTA.c"
    
    INCLUDE_DIRS 
        "include"
    
    EMBED_FILES
        "src/ota.html"
    
    REQUIRES 
        esp_http_server
        app_update
)
