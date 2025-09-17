
set(SOURCES "app.cc"
        "drivers/storage/sd_card.cc" 

)

set(INCLUDE_DIRS "." 
            "drivers"
            "drivers/storage"
)


idf_component_register(SRCS ${SOURCES} 
                INCLUDE_DIRS ${INCLUDE_DIRS} 
                )