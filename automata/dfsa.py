import functools
import itertools
import collections
from eq_partition import Partitioning

class DFSA(object):
    def __init__(self, alphabet, transitions, initial, accepting, allow_underdefined=False):
        '''If allow_underdefined, underdefined DFSA state transitions will
result in the string being rejected. Otherwise, underdefined DFSAs will throw an
error during execution.'''
        self.alphabet = alphabet
        self.transitions = transitions
        self.initial = initial
        self.state = initial
        self.accepting = frozenset(accepting)
        self.allow_underdefined = allow_underdefined

    def check(self):
        if not self.allow_underdefined:
            for state in self.transitions.keys():
                for letter in self.alphabet:
                    if (state not in self.transitions or
                        letter not in self.transitions[state]):
                        raise RuntimeError('FSA is not fully defined (for {state}, {letter})'
                                       .format(**locals()))

    def run(self):
        '''Returns a new object which can be called with a string to determine
if the string is accepted by this DFSA. The resulting object will not modify 
this object.'''
        return FSA_run(self)

    def final_eq_states(self):
        '''Returns a partitioning of states which are indistinguishable under
any string'''
        old_eq_classes = self.eq_states(0)
        for i in itertools.count(1):
            eq_classes = self.eq_states(i)
            if eq_classes == old_eq_classes:
                break
            old_eq_classes = eq_classes
        return eq_classes

    def minimize(self):
        '''Returns a new DFSA with minimal states'''
        E = self.final_eq_states()
        transitions = collections.defaultdict(dict)
        for state, these_transitions in self.transitions.items():
            for letter, new_state in these_transitions.items():
                transitions[E.eq_class(state)][letter] = E.eq_class(new_state)
        return DFSA(self.alphabet, transitions, E.eq_class(self.initial),
                    set(E.eq_class(a) for a in self.accepting), self.allow_underdefined)

    @functools.lru_cache()
    def eq_states(self, i):
        '''Returns a partitioning of states which are indistinguishable under a
string of length i'''
        if i == 0:
            accepting = frozenset(self.accepting)
            others = frozenset(self.transitions.keys()) - accepting
            return Partitioning([others, accepting])
        else:
            alphabet = list(self.alphabet)
            eq_classes = self.eq_states(i - 1)
            result = Partitioning([])
            for eq_class in eq_classes:
                subclasses = collections.defaultdict(set)
                for state in eq_class:
                    tr = []
                    for letter in alphabet:
                        tr.append(eq_classes.classify(self.transitions[state][letter]))
                    subclasses[tuple(tr)].add(state)
                for subclass in subclasses.values():
                    result.add_part(frozenset(subclass))
            return result

class DFSA_run(object):
    '''Runs a DSFA on a given string
calling this object this on a string will return True if the language is
accepted by the DFSA, False otherwise.
self.states will then contain list of states in the path taken by the DFSA'''
    def __init__(self, dfsa):
        self.dfsa = dfsa
        self.states = [self.fsa.initial]

    def letter(self, letter):
        '''Respond to letter'''
        if letter not in self.fsa.alphabet:
            if dfsa.allow_underdefined:
                return False
            else:
                raise RuntimeError('Underdefined for {state} given {letter}'.format(**locals()))
        else:
            state = self.fsa.transitions[self.states[-1]][letter]
            self.states.append(state)
            return state in self.dfsa.accepting

    def __call__(self, string):
        '''Returns if the string is accepted by the DFSA'''
        for letter in string:
            result = self.letter(letter)
        return result
