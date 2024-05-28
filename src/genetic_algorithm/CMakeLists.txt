add_subdirectory(Crossover)
add_subdirectory(Mutation)
add_library(GeneticAlgo Chromosome.h GeneticAlgo.h GeneticAlgo.cpp)

link_libraries(GeneticAlgo Crossover Mutation)
