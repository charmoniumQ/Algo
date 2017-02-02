import itertools
import collections
import random

def classify(e, partitioning):
    for partition in partitioning:
        if e in partition:
            return partition

def latex_set(sett, str=str):
    return '\{' + ', '.join(map(str, sett)) + '\}'

def latex_partitioning(partitioning):
    return latex_set(partitioning, str=latex_set)

class FSA(object):
    def __init__(self, alphabet, transitions, initial, accepting):
        self.alphabet = alphabet
        self.transitions = transitions
        self.initial = initial
        self.state = initial
        self.accepting = frozenset(accepting)
        for state in self.transitions.keys():
            for letter in self.alphabet:
                if (state not in self.transitions or letter not in self.transitions[state]):
                    raise RuntimeError('FSA is not fully defined (for {state}, {letter})'
                                       .format(**locals()))

    def run(self):
        return FSA_run(self)

    def eq_classes(self, i):
        if i == 0:
            accepting = frozenset(self.accepting)
            others = frozenset(self.transitions.keys()) - accepting
            return frozenset([others, accepting])
        else:
            alphabet = list(self.alphabet)
            result = frozenset()
            eq_classes = self.eq_classes(i - 1)
            for eq_class in eq_classes:
                subclasses = collections.defaultdict(set)
                for state in eq_class:
                    tr = []
                    for letter in alphabet:
                        tr.append(classify(self.transitions[state][letter], eq_classes))
                    subclasses[tuple(tr)].add(state)
                result |= frozenset(frozenset(subclass) for subclass in subclasses.values())
            return result

    def minimize(self):
        print(r'\begin{tabular}{ll}')
        old_eq_classes = self.eq_classes(0)
        print(r'0 & {} \\'.format(latex_partitioning(old_eq_classes), **locals()))
        for i in itertools.count(1):
            eq_classes = self.eq_classes(i)
            print(r'{i} & {} \\'.format(latex_partitioning(eq_classes), **locals()))
            if eq_classes == old_eq_classes:
                break
            old_eq_classes = eq_classes
        print(r'\end{tabular}')

    def latex(self, translation=None):
        print(r'''
\begin{tikzpicture}[
  scale=1.2,
  shorten >= 1pt,
  node distance=2cm,
  accepting/.style=accepting by arrow,
  initial text=
  ]''')

        if translation is None:
            translation = {}

        node = r'\node[state{options}] ({state}) at ({x}, {y}) {{{text}}};'
        for i, state in enumerate(self.transitions.keys()):
            y = i + random.randint(-3, 3)
            x = i + random.randint(-3, 3)
            options = ''
            if state == self.initial:
                options += ',initial'
            if state in self.accepting:
                options += ',accepting'
            if state in translation:
                text = translation[state]
            else:
                text = state
            print(node.format(**locals()))

        link = r'\path[->] ({state}) edge {options} node [above] {{{letter}}} ({new_state});'
        for state, transitions in self.transitions.items():
            for letter, new_state in transitions.items():
                if state == new_state:
                    options = '[loop below]'
                else:
                    options = ''
                print(link.format(**locals()))

        print(r'\end{tikzpicture}')

class FSA_run(object):
    def __init__(self, fsa):
        self.fsa = fsa
        self.state = self.fsa.initial

    def __call__(self, string):
        if len(string) == 1:
            letter = string
            if letter not in self.fsa.alphabet:
                return False
            else:
                self.state = self.fsa.transitions[self.state][letter]
                return self.state in self.fsa.accepting
        else:
            for letter in string:
                result = self(letter)
            return result

fsa = FSA({'a', 'b'}, {
    1: dict(a=2, b=3),
    2: dict(a=4, b=5),
    3: dict(a=6, b=7),
    4: dict(a=5, b=4),
    5: dict(a=7, b=5),
    6: dict(a=2, b=7),
    7: dict(a=7, b=4),
}, 1, {1, 2, 7})

fsa.minimize()

fsa = FSA({'a', 'b'}, {
    1: dict(a=2, b=3),
    2: dict(a=4, b=5),
    3: dict(a=6, b=7),
    4: dict(a=4, b=5),
    5: dict(a=6, b=7),
    6: dict(a=4, b=5),
    7: dict(a=6, b=7),
}, 1, {2, 6})

fsa.minimize()

fsa.latex()
