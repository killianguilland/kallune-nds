# Kallune (port Nintendo DS)

Petit port Nintendo DS du jeu Kallune, compilé avec devkitPro / devkitARM.

## Installation et compilation

Prérequis
- devkitPro (incluant devkitARM) installé sur votre machine. Voir https://devkitpro.org
- Les outils fournis par devkitPro (grit, mmutil, bin2o) sont utilisés par le Makefile.

Installation et compilation rapide
1. Installez devkitPro / devkitARM selon la documentation officielle.
2. Dans votre shell (zsh), exportez la variable d'environnement utilisée par le Makefile :

	export DEVKITARM=/chemin/vers/devkitARM

3. À la racine du dépôt, lancez la compilation :

	make

Le Makefile s'appuie sur les règles de `ds_rules` (fourni par devkitARM). Si tout est configuré, cela produira un fichier `.nds` exécutable.

Notes
- Ce projet utilise la toolchain et les utilitaires de devkitPro pour convertir les graphismes et l'audio (grit, maxmod/mmutil, bin2o).
- Pour tester la ROM (melon / no$gba), utilisez un émulateur NDS compatible ou bien tout simplement une console Nintendo DS.

## Crédits

Port réalisé à partir du [code source de Kallune](https://github.com/guillaume-tritsch/Kallune.git) pour la Nintendo DS. 
Code original par @guillaume-tritsch / @BQuent1 / @killianguilland.

Avec des assets graphiques CC BY 4.0 créés par [scrabling](https://scrabling.itch.io/pixel-isometric-tiles).