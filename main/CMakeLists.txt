set(SOURCES "app.cc" "drivers/sd_card.cc")
set(INCLUDE_DIRS, "." "drivers")

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS ${INCLUDE_DIRS})
