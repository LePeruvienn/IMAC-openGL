#!/bin/bash

# Le but de ce script est de pouvoir compiler et lancer facilement les binaires des différents exercices.
# Pour cela on lance le script puis on passe en arguement tout les filtre de nom qu'on veut.
#
# Exemple : `load.sh tp2 triangle couleur`
#
# - D'abord on prend seulement les fichiers qui ont les caractère tp2 (majuscule et minuscule), donc on as tout les fichier du tp2
# - On cherche apres dans ceux la ceux qui contienne le mot triangle.

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# On se déplace dans le dossier on est le script
cd SCRIPT_DIR

# On build le projet
cmake -B build/
cd build/
make

cd ..

echo -e "\n✅ Tout les projets on bien été compilé.\n"

# Si aucun argument n'as été fournis on affiche un message build on ferme le programme
if [ $# -eq 0 ]; then
	exit 0
fi

# Il y on as entré des arguments ...
# On va dans le dossier des binaires
cd bin/

# On récupère tout les fichiers dans le répertoire courant (`bin/`)
files=(*)

# On désactive le fait que les majuscule compte lors des comparaison de string
shopt -s nocasematch

# On filtre tout les fichiers par rapport aux arguments donner
for arg in "$@"; do

	# On créer un tableau vide, ou l'ont rajoutera les fichier filtrer au fur à mesure
	matched_files=()

	# On parcours tout les fichiers qui sont dans files
	for file in "${files[@]}"; do

		# Si le fichier actuelles contient la valeurs dans arg et c'est un executable
		# -> Alors on le rajoute la la liste
		if [[ "$file" == *"$arg"* ]] && [ -x "$file" ]; then
			matched_files+=("$file")
		fi
	done

	# On copie le tableau matched_files dans files pour recommencer
	files=("${matched_files[@]}")
done

# On reactive le fait que les majuscule compte dans les compairason (au cazou)
shopt -u nocasematch

# Vérifie qu'il reste des fichiers
if [ ${#files[@]} -eq 0 ]; then
	echo "❌ Aucun binaire correspondant à : \`$@\`"
	exit 1
fi

# Si il existe seulement 1 ficher dans la liste
# -> On lance alors ce fichier la
if [ ${#files[@]} -eq 1 ]; then
	echo "⚡ Lancement du fichier : ${files[0]}"
	./"${files[0]}"
	exit 0
fi

# Menu interactif
echo -e "▶️ Choisis un binaire :"
select chosen_file in "${files[@]}"; do
	if [[ -n "$chosen_file" ]]; then
		echo -e "\n ⚡ Lancement du fichier : $chosen_file ...\n"
		./$chosen_file
		break
	else
		echo "❌ Choix invalide, réessaie."
	fi
done

# On ferme le programme tout est bien qui finis bien !
exit 0
