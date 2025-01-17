idf_component_register(
    SRCS 
        "eOTA.c"
    
    INCLUDE_DIRS 
        "include"
    
    EMBED_FILES
        "src/ota.min.html"
        "src/css/ota.min.css"
        "src/js/ota.min.js"
    
    REQUIRES 
        esp_http_server
        app_update
)
