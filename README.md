# Solver-Rubiks-Cube
Le projet sur lequel j'ai travaillé consiste à concevoir un système robotique pour résoudre le Rubik's Cube, qui est un puzzle complexe et bien connu.

Dans ce projet, j'ai travaillé sur les trois parties suivantes : une application Android, un serveur Wi-Fi et un algorithme de résolution.


# APPLICATION ANDROID

L'application Android est la partie du projet qui permet à l'utilisateur de rentrer le patron du cube pour ensuite le transmettre au serveur Wi-Fi qui s'occupera de résoudre le cube. Cette application est conçue pour être simple et intuitive, afin que l'utilisateur puisse facilement entrer le patron sans avoir besoin de connaissances préalables sur la résolution du cube.

L'interface de l'application est constituée d'un écran principal avec un dessin du patron du cube, ainsi que des boutons pour les différentes fonctionnalités de l'application. L'utilisateur peut cliquer sur les différentes faces du cube pour les remplir avec les couleurs correspondantes.

L'application n'utilise pas d'algorithme pour vérifier que le patron entré est valide et peut être résolu. Cela sera géré par l'algorithme de résolution. Tout type de patron peut être envoyé.

Une fois que le patron est entré, l'utilisateur peut l'envoyer au serveur Wi-Fi en appuyant sur le bouton correspondant. L'application utilise alors la connexion Wi-Fi pour transmettre le patron au serveur, qui s'occupe de résoudre le cube et d'enregistrer la solution.

En résumé, l'application Android est une interface intuitive et simple à utiliser qui permet à l'utilisateur de rentrer le patron du cube à résoudre en utilisant des boutons tactiles. Elle envoie le cube au serveur Wi-Fi pour résolution.


# SERVEUR WIFI

Le serveur Wi-Fi est un élément central de notre projet, car il permet la communication entre l'application Android et l'algorithme de résolution du cube. Le code du serveur a été écrit en Python et est relativement simple à comprendre.

Tout d'abord, nous avons spécifié l'adresse IP et le port du serveur sur lequel il sera en écoute pour toute demande entrante. Ensuite, nous avons créé un socket TCP qui permettra au serveur de recevoir des messages de la part de l'application Android.

Lorsqu'une connexion est établie avec un client, le serveur lit le message reçu, qui peut être soit "EXECUTE_CUBE_PROGRAM", soit "EXECUTE_CAMERA_PROGRAM", soit un message quelconque. Si le message est " EXECUTE_CUBE_PROGRAM", le serveur exécute le programme "cube.exe", qui est l'algorithme de résolution du cube. Si le message est "EXECUTE_CAMERA_PROGRAM", le serveur exécute le programme "camera.exe", qui permet de scanner les couleurs du cube en utilisant la caméra du smartphone.

Enfin, si le message est un message quelconque, le serveur écrit le message dans un fichier texte nommé "reception.txt". Ce fichier est utilisé pour stocker les patrons de cube entrés par l'utilisateur.

Le serveur Wi-Fi a été testé et validé avec succès sur le réseau local de l'école. Il a permis une communication fluide entre l'application Android et l'algorithme de résolution du cube.


# ALGORITHME DE RÉSOLUTION

L'algorithme de résolution de cube est implémenté dans le fichier cube.exe. Cet algorithme utilise une approche de recherche en profondeur pour trouver la solution optimale au Rubik's cube.

L'algorithme commence par initialiser le Rubik's cube en appelant la fonction cube_init(). Ensuite, il affiche la configuration initiale du cube en appelant cube_render(). Après cela, l'algorithme appelle la fonction cube_solve() pour trouver la solution au cube.

La fonction cube_solve() utilise une structure de données de tableau pour stocker les cubes créés lors de la recherche. Cette structure de données est définie comme suit :

typedef struct {

	int   open ;

	int   close;

	int   size ;

	CUBE *cubes;
} TABLE;

La fonction search_table_create() est utilisée pour initialiser la structure de données de tableau avec une taille spécifiée. La fonction search_table_destroy() est utilisée pour libérer la mémoire allouée pour le tableau.

La fonction search() est le cœur de l'algorithme de résolution. Elle effectue une recherche en profondeur pour trouver la solution au Rubik's cube. La fonction prend en entrée une configuration initiale du cube, un état cible, une liste d'opérations à effectuer sur le cube, un nombre d'opérations et une valeur de coupe. Elle retourne un pointeur vers la solution si elle est trouvée, sinon elle retourne NULL.

La fonction print_solve_oplist() est appelée une fois que la solution est trouvée. Elle génère une liste d'opérations pour résoudre le Rubik's cube. Cette liste est stockée dans une chaîne de caractères qui est ensuite écrite dans un fichier appelé "Solution.txt" en appelant la fonction writeFILE().

La fonction writeFILE() est utilisée pour écrire la solution dans un fichier. Elle prend en entrée une chaîne de caractères qui contient la liste d'opérations et écrit cette chaîne dans le fichier "Solution.txt".

La fonction ReadFILE() est utilisée pour lire les données d'entrée du fichier "Reception.txt". Cette fonction lit le fichier ligne par ligne et stocke chaque ligne dans une matrice. Les données lues sont ensuite utilisées pour initialiser le Rubik's cube.

Il faut noter que l’algorithme de résolution marche par paire. Dans un premier temps, il dit la face à tourner (F pour front, B, pour back, etc.) puis il rentre la direction de la rotation (T, sens trigonométrique, H pour sens horaire et 2 pour une rotation à 180°).

Enfin, la fonction main() appelle les différentes fonctions pour résoudre le Rubik's cube et afficher la solution. Elle lit également les données d'entrée à partir du fichier "Reception.txt" en appelant la fonction ReadFILE(). La solution est ensuite affichée en appelant la fonction print_solve_oplist(). La liste d'opérations est formatée et affichée sur la sortie standard.
