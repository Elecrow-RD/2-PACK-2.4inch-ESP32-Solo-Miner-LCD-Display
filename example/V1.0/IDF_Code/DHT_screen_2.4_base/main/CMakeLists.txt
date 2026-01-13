file(GLOB_RECURSE LIB_SRCS
        "..//components/DHT20/*.CPP"
)

idf_component_register(SRCS "CrowPanel_ESP32_2.4_2.8.cpp"
                    INCLUDE_DIRS "."
                    "../components/arduino-esp32-master"
                    "../components/TFT_eSPI"
                    "../components/lvgl-3"
                    "../components/ui"
                    "../components/DHT20"
                    )
