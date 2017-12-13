
add_executable( sophiatxd main.cpp )

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling sophiatxd with TCMalloc")
    #    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
endif()

# We have to link against graphene_debug_miner because deficiency in our API infrastructure doesn't allow plugins to be fully abstracted #246
target_link_libraries( sophiatxd
                       PRIVATE graphene_app graphene_account_history decent_seeding decent_messaging graphene_miner graphene_chain graphene_debug_miner graphene_egenesis_decent fc ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )

install( TARGETS
   sophiatxd

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
