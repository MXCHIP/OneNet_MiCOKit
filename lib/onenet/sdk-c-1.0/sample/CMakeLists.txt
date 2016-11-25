add_executable(regina-sample
  sample.c
  conf.c
  regina_notification.c
  regina_configuration.c
  regina_sensor_data.c
  regina_event.c
  regina_profile.c
  )

if(BUILD_SHARED_LIBS)
  target_link_libraries(regina-sample reginac)
elseif(BUILD_STATIC_LIBS)
  target_link_libraries(regina-sample reginac_s)
endif()
