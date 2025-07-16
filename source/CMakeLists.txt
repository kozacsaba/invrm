# Plugin #######################################################################

message(STATUS "### Configuring Plugin ###")

# juce_set_vst2_sdk_path(...)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

juce_add_plugin(${PREDEF_PROJECT_NAME}
    PRODUCT_NAME "${PREDEF_PROJECT_NAME}"
    COMPANY_NAME "Csaba Koza"
    FORMATS 
        VST3
    PLUGIN_NAME "${PREDEF_PROJECT_NAME}"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES
        Fx
    BUNDLE_ID "com.Csaba_Koza.${PREDEF_PROJECT_NAME}"
)

target_compile_features(InvRM PUBLIC cxx_std_20)

target_include_directories(InvRM PUBLIC
                           ${CMAKE_SOURCE_DIR}/source)

target_sources(InvRM PUBLIC
    proc/PluginProcessor.cpp
    gui/PluginEditor.cpp

    util/Logger.cpp
    proc/Modulator.cpp
    gui/vubar.cpp
    util/CoreTimer.cpp
    )

target_compile_definitions(InvRM PUBLIC
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PREDEF_PROJECT_NAME},JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PREDEF_PROJECT_NAME},JUCE_VERSION>"
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING
)

# juce_add_binary_data(GuiAppData SOURCES ...)

target_link_libraries(InvRM PUBLIC
    # GuiAppData
    juce::juce_core
    # juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_audio_basics
    # juce::juce_audio_formats
    juce::juce_gui_basics
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
    juce::juce_recommended_lto_flags
)
