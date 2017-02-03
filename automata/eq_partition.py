def union(sets):
    result = set()
    for set in sets:
        result |= set

class Partitioning(object):
    '''An equivalence partition.

It contains multiple sets caleld 'parts'.
See https://en.wikipedia.org/wiki/Partition_of_a_set
See https://en.wikipedia.org/wiki/Equivalence_class'''
    def __init__(self, parts=None):
        if parts is None:
            parts = []
        self.parts = list(parts)

    def classify(self, x):
        '''Returns an integer representing the equivalence class of x'''
        for i, part in enumerate(self):
            if x in part:
                return i

    def eq(self, a, b):
        '''Returns if two elements are in the same equivalence class'''
        return b in self.eq_class(a)

    def eq_class(self, e):
        '''Returns equivalence class of e'''
        for part in self:
            if e in part:
                return part

    def add_part(self, part):
        self.parts.append(part)

    def check(self, A):
        '''Checks partitioning
Asserts that
- all partitions are not empty
- the union of all parts is A
- all of the sets are disjoint'''
        assert all(self.parts)
        assert union(self.parts) == A
        assert not any(i_ & j_
                       for i, i_ in enumerate(self)
                       for j, j_ in enumerate(self)
                       if i != j)

    def __iter__(self):
        return iter(self.parts)

    def freeze(self):
        '''Return a frozenset-of-frozensets version of this'''
        return frozenset({frozenset(part) for part in self})

    def __eq__(self, other):
        '''Returns if two partitionings describe the same equivalence relation'''
        return self.freeze() == other.freeze()

    def __str__(self):
        return str(list(map(set, self.parts)))
