
set(SOURCES "app.cc"
        "drivers/storage/sd_card.cc" 
        "drivers/audio/audio_es7210.cc" 
        "drivers/audio/wav_recorder.cc" 

)

set(INCLUDE_DIRS "." 
        "drivers"
        "drivers/storage"
        "drivers/audio/"
)


idf_component_register(SRCS ${SOURCES} 
                INCLUDE_DIRS ${INCLUDE_DIRS} 
                )
