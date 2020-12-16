

# TP2 de CORDOS Célestin

`pandoc -s --toc tp2.md --css=./github-pandoc.css -o tp2.html`





## lscpu

```
Architecture :                          x86_64
Mode(s) opératoire(s) des processeurs : 32-bit, 64-bit
Boutisme :                              Little Endian
Address sizes:                          39 bits physical, 48 bits virtual
Processeur(s) :                         1
Liste de processeur(s) en ligne :       0
Thread(s) par cœur :                    1
Cœur(s) par socket :                    1
Socket(s) :                             1
Nœud(s) NUMA :                          1
Identifiant constructeur :              GenuineIntel
Famille de processeur :                 6
Modèle :                                60
Nom de modèle :                         Intel(R) Core(TM) i7-4770K CPU @ 3.50GHz
Révision :                              3
Vitesse du processeur en MHz :          3500.020
BogoMIPS :                              7000.04
Constructeur d'hyperviseur :            KVM
Type de virtualisation :                complet
Cache L1d :                             32 KiB
Cache L1i :                             32 KiB
Cache L2 :                              256 KiB
Cache L3 :                              8 MiB
Nœud NUMA 0 de processeur(s) :          0
Vulnerability Itlb multihit:            KVM: Vulnerable
Vulnerability L1tf:                     Mitigation; PTE Inversion
Vulnerability Mds:                      Mitigation; Clear CPU buffers; SMT Host 
                                        state unknown
Vulnerability Meltdown:                 Mitigation; PTI
Vulnerability Spec store bypass:        Vulnerable
Vulnerability Spectre v1:               Mitigation; usercopy/swapgs barriers and
                                         __user pointer sanitization
Vulnerability Spectre v2:               Mitigation; Full generic retpoline, STIB
                                        P disabled, RSB filling
Vulnerability Srbds:                    Unknown: Dependent on hypervisor status
Vulnerability Tsx async abort:          Not affected
Drapaux :                               fpu vme de pse tsc msr pae mce cx8 apic 
                                        sep mtrr pge mca cmov pat pse36 clflush 
                                        mmx fxsr sse sse2 ht syscall nx rdtscp l
                                        m constant_tsc rep_good nopl xtopology n
                                        onstop_tsc cpuid tsc_known_freq pni pclm
                                        ulqdq monitor ssse3 cx16 pcid sse4_1 sse
                                        4_2 x2apic movbe popcnt aes xsave avx rd
                                        rand hypervisor lahf_lm abm invpcid_sing
                                        le pti fsgsbase avx2 invpcid md_clear fl
                                        ush_l1d

```

*Des infos utiles s'y trouvent : nb core, taille de cache*

NB: Les résultats donnés par lscpu (threads par coeur, coeur par socket...) sont différents de ceux donnés par Intel...



## Produit scalaire 

*Expliquer les paramètres, les fichiers, l'optimisation de compil, NbSamples, ...*

OMP_NUM    | samples=1024 | 
-----------|--------------|----------
séquentiel |   		1	| 			aucune accéleration 
1          |  1 | | 			aucune accéleration 
2          | 0.9  | | 			 
3          | 0.9  | | 			
4          | 0.9  | | 			
8          | 1.1  |   			


*Discuter sur ce qu'on observe, la logique qui s'y cache.*
Il n'y a pas d'accéleration pour un nombre de processus différent de 8 (optimal sur ma machine puisque j'ai 8 threads): en effet, on est en memory bound: le temps de calcul est tres ffaible, ce qui est long, c'est l'écriture, car on ecrit toujours au même endroit 

c++11 threads    | samples=1024 | 
-----------|--------------|----------
séquentiel |   		1	| 			aucune accéleration 
1          |  1 | | 			aucune accéleration 
2          |147  | | 			 
3          | 1.47  | | 			
4          | 1.2  | | 			
8          | 1 | 

Les temps de calcul sont meilleurs avec C++ threads. On ne peut pas améliorer les résultats obtenus en raison des étapes non parallélisables. 

## Produit matrice-matrice
Pour 1023 : 6.64049 secondes
Pour 1024 : 25.4070 secondes
Pour 1023 : 7.23941 secondes

Le temps pour une taille de 1024 est significativement plus long que pour 1023 car le processeur atteint la limite de sa mémoire cache, il est donc obligé de mettre des données en RAM

### Permutation des boucles

*Expliquer comment est compilé le code (ligne de make ou de gcc) : on aura besoin de savoir l'optim, les paramètres, etc. Par exemple :*

`make TestProduct.exe && ./TestProduct.exe 1024`


  ordre           | time    | MFlops  | MFlops(n=2048) 
------------------|---------|---------|----------------
i,j,k (origine)   | 151     | 113     |                
j,i,k             | 128.207 | 134.001 |    
i,k,j             | 365.278 | 47.0323 |    
k,i,j             | 195.586 | 87.8379 |    
j,k,i             | 43.6695 | 393.407 |    
k,j,i             | 39.5948 | 433.829 |    


*Discussion des résultats*
Le pronleme avec les boucles autres que (k,j,i) et (j, k, i), c'est que l'on accède trop souvent à la mémoire, on est donc dans un cas memory bound, autrement dit, on est limité par la capacité d'accès a la mémoire. 



### OMP sur la meilleure boucle 

`make TestProduct.exe && OMP_NUM_THREADS=8 ./TestProduct.exe 1024`

  OMP_NUM         | MFlops  | MFlops(n=2048) | MFlops(n=512)  | MFlops(n=4096)
------------------|---------|----------------|----------------|---------------
1                 |         |    445.072     |
2                 |         |    433.668
3                 |         |    432.858
4                 |         |    415.194
5                 |         |    436.819
6                 |         |    487.762
7                 |         |    481.806
8                 |         |    493.446




### Produit par blocs

`make TestProduct.exe && ./TestProduct.exe 1024`

  szBlock         | MFlops  | MFlops(n=2048) | MFlops(n=512)  | MFlops(n=4096)
------------------|---------|----------------|----------------|---------------
origine (=max)    |  |
32                |  |
64                |  |
128               |  |
256               |  |
512               |  | 
1024              |  |




### Bloc + OMP



  szBlock      | OMP_NUM | MFlops  | MFlops(n=2048) | MFlops(n=512)  | MFlops(n=4096)
---------------|---------|---------|------------------------------------------------
A.nbCols       |  1      |         | 
512            |  8      |         | 



## Bouddha 
Sans parallélisation, le temps de calcul sont de 6.2, 5.7 et 0.5 respectivement pour Bouddha1, Bouddha2 et Bouddha3. Avec parallelisation, ces temps passent à 13.5, 12.8 et 1.2 mais les taches s'effectuent en meme temps. Il n'y a donc pas d'accéleration. 



# Tips 

```
	env 
	OMP_NUM_THREADS=4 ./dot_product.exe
```

```
    $ for i in $(seq 1 4); do elap=$(OMP_NUM_THREADS=$i ./TestProductOmp.exe|grep "Temps CPU"|cut -d " " -f 7); echo -e "$i\t$elap"; done > timers.out
```
