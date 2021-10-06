#include "disjoint_set.h"

#define ROOT -1 // объявили макрос (считайте константу) равный минус единице - чтобы не было "волшебной переменной", а была именованная константа "корень дерева"


DisjointSet::DisjointSet(int size)
{
    parents = std::vector<int>(size);
    ranks = std::vector<int>(size);
    sizes = std::vector<int>(size);

    // TODO - заполните вектора так чтобы на этапе конструирования эта система непересекающихся множеств состояла из:
    // size элементов, каждый из которых состоит в своем множестве (а значит ссылка на родителя у него - на корень, т.е. на ROOT, а какой тогда ранк и размер у множества каждого элемента?)
    // TODO заполнить parents
    // TODO заполнить ranks
    // TODO заполнить sizes

    for (int i = 0; i < size; i++){
        parents.push_back(ROOT);
        ranks.push_back(0);
        sizes.push_back(1);
    }
}

int	DisjointSet::get_set(int element)
{
    // номер этого корневого элемента - номер множества на данный момент (кто вверху тот и главный, множество названо в его честь)
    int first_of_their_kind = 0;
    while(element != ROOT){
        first_of_their_kind = element;
        element = parents[element];
        //TODO: бага
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
    int count = 0;
    while (element != ROOT){
        element = parents[element];
        count++;
    }
    // TODO сообщить сколько элементов в множестве, которому принадлежит данный элемент (да, это очень просто)
    return count;
}

int	DisjointSet::union_sets(int element0, int element1)
{
    // TODO узнать корневые элементы этих двух элементов и посмотрев на них - решить,
    // кого из них подвесить к другому (тем самым объединить два множества)
    // при этом стоит подвешивать менее высокое дерево к более высокому (т.е. учитывая ранк),
    // а так же важно не забыть после переподвешивания у корневого элемента обновить ранк и размер множества

    return 0; // вернуть номер объединенного множества
}
