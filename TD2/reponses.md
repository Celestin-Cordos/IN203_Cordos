#TP 2

##Question du cours 1

* Un scénario avec interblocage serait le scénario dans lequel deux processus commencent par une commande Reception ou une commande envoi

* Le cas non bloquant correspond au cas où les deux sont alternés par exemple le premier commence par un envoi et le second par une reception ou inversement 

La probabilité d'avoir un blocage est de 1/2

##Question du cours 2

La loi d'Amdhal est la suivante:
S(n) = n/(1+(n-1) * f) avec f la fraction en temps du code non parallélisable. 
Ici, f = 1-0.9 = 0.1. Donc, en faisant tendre n vers l'infini, on a
S(n)~1/f = 10
L'accélération maximale que pourra atteindre Alice est donc de 10.

Il semble, dans ce cas-là, raisonnalble de proposer 10 noeuds de calcul puisqu'on peut arriver à accélerer le calcul jusqu'a dix fois. il est, en revanche inutile de proposer plus, puisque d'après la formule, au-dessus, le onzième noeud ne servira à rien.

L'accélération en fonction du nombre de données est donnée par S_s (n) = n + (n-1) s ou s est la proportion du code qui s'exécute en séquentiel. Ici, on prend s = f = 0.25. On a alors:

S_s (n) = s + np

On prend n = 2n et on a S_s = 0.25 + 2 * 3.75 = 7.75



##Ensemble de Mandelbroot

1. Le temps d'execution est en moyenne de 30 (pour 8 processus) en utilisant le calcul parallele et de 45s environ pour un calcul sequentiel. En revanche, dans le cas parallele le temps peut augmenter lorsqu'on diminue les processus pour depasser les 100 secondes dans le cas de 5 processus. 
On peut alors penser que le temps gagné a diviser les taches est peut etre perdu par l'echange d'informations entre les processus.
