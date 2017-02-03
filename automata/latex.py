import random

def latex_set(sett, str=str):
    return '\{' + ', '.join(map(str, sett)) + '\}'

def latex_partition(partitioning):
    return latex_set(partitioning, str=latex_set)

def latex_fsa(fsa, translation=None, deterministic=True):
    output = ''
    output += r'''
\begin{tikzpicture}[
  scale=1.2,
  shorten >= 1pt,
  node distance=2cm,
  accepting/.style=accepting by arrow,
  initial text=
  ]''' + '\n'

    if translation is None:
        translation = {}

    node = r'\node[state{options}] ({state!s}) at ({x}, {y}) {{{text}}};' + '\n'
    for i, state in enumerate(fsa.transitions.keys()):
        y = i + random.randint(-3, 3)
        x = i + random.randint(-3, 3)
        options = ''
        if state == fsa.initial:
            options += ',initial'
        if state in fsa.accepting:
            options += ',accepting'
        if state in translation:
            text = str(translation[state])
        else:
            text = str(state)
        output += node.format(**locals())
    output += '\n'

    link = r'\path[->] ({state}) edge {options} node [above] {{{letter}}} ({new_state});' + '\n'
    for state, transitions in fsa.transitions.items():
        for letter, r in transitions.items():
            if deterministic:
                new_states = [r]
            else:
                new_states = r
            for new_state in new_states:
                if state == new_state:
                    options = '[loop below]'
                else:
                    options = ''
                output += link.format(**locals())

    output += r'\end{tikzpicture}' + '\n'
    return output

def minimize(dfsa):
    print(r'\begin{tabular}{ll}')
    old_eq_classes = dfsa.eq_classes(0)
    print(r'0 & {} \\'.format(old_eq_classes.latex(), **locals()))
    for i in itertools.count(1):
        eq_classes = dfsa.eq_classes(i)
        print(r'{i} & {} \\'.format(eq_classes.latex(), **locals()))
        if eq_classes == old_eq_classes:
            break
        old_eq_classes = eq_classes
    print(r'\end{tabular}')

__all__ = [var for var in locals().keys() if var.startswith('latex_')]
