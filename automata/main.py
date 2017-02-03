from latex import *
import dfsa

mydfsa = dfsa.DFSA({'a', 'b'}, {
    1: dict(a=2, b=3),
    2: dict(a=4, b=5),
    3: dict(a=6, b=7),
    4: dict(a=5, b=4),
    5: dict(a=7, b=5),
    6: dict(a=2, b=7),
    7: dict(a=7, b=4),
}, 1, {1, 2, 7})

print(mydfsa.final_eq_states())


mydfsa = dfsa.DFSA({'a', 'b'}, {
    1: dict(a=2, b=3),
    2: dict(a=4, b=5),
    3: dict(a=6, b=7),
    4: dict(a=4, b=5),
    5: dict(a=6, b=7),
    6: dict(a=4, b=5),
    7: dict(a=6, b=7),
}, 1, {2, 6})

print(mydfsa.final_eq_states())
print(latex_fsa(mydfsa.minimize()))
