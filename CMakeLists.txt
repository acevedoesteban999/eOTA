idf_component_register(
    SRCS 
        "eOTA.c"
    
    INCLUDE_DIRS 
        "include"
    
    EMBED_FILES
        "src/ota.html"
        "src/css/ota.css"
        "src/js/ota.js"
    
    REQUIRES 
        esp_http_server
        app_update
)
