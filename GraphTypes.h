#ifndef GRAPHTYPES_H
#define GRAPHTYPES_H

// Типы графов
enum GraphType {
    COMPLETE,
    SPARSE,
    RANDOM,
    CYCLE,
    TREE
};

// Типы алгоритмов
enum AlgorithmType {
    SHORTEST_PATH,
    DYNAMIC_SHORTEST_PATH,
    STRONGLY_CONNECTED_COMPONENTS,
    HASSE_DIAGRAM,
    COLORING,
    MST,
    CONNECTED_COMPONENTS
};

#endif // GRAPHTYPES_H