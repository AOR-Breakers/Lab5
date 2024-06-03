add_subdirectory(local_search)
add_subdirectory(genetic_algorithm)

add_executable(main main.cpp)
target_link_libraries(main GeneticAlgo LocalSearch)
