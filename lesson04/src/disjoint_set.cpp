#include "disjoint_set.h"

#define ROOT -1 // объявили макрос (считайте константу) равный минус единице - чтобы не было "волшебной переменной", а была именованная константа "корень дерева"


DisjointSet::DisjointSet(int size)
{
    parents = std::vector<int>(size);
    ranks = std::vector<int>(size);
    sizes = std::vector<int>(size);

    for (int i = 0; i < size; i++){
        parents[i] = ROOT;
        ranks[i] = 0;
        sizes[i] = 1;
    }
}

int	DisjointSet::get_set(int element)
{
    // номер этого корневого элемента - номер множества на данный момент (кто вверху тот и главный, множество названо в его честь)
    int first_of_their_kind = 0;
    while(element != ROOT){
        first_of_their_kind = element;
        element = parents[element];
    }

    return first_of_their_kind;
}

int DisjointSet::count_differents() const
{
    int count = 0;
    for (size_t i = 0; i < this->parents.size(); i++) {
        if (parents[i] == ROOT) count++;
    }
    return count;
}

int DisjointSet::get_set_size(int element)
{
    while (parents[element] != ROOT){
        element = parents[element];
    }
    return sizes[element];
}

int	DisjointSet::union_sets(int element0, int element1)
{
    // кого из них подвесить к другому (тем самым объединить два множества)
    // при этом стоит подвешивать менее высокое дерево к более высокому (т.е. учитывая ранк),
    // а так же важно не забыть после переподвешивания у корневого элемента обновить ранк и размер множества

    if (ranks[element0] < ranks[element1]) {
        parents[element0] = element1;
        sizes[element1] += sizes[element0];
    }
    else {
        parents[element1] = element0;
        sizes[element0] += sizes[element1];
    }

    return get_set(element0); // вернуть номер объединенного множества
}
