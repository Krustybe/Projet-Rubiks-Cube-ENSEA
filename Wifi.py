import socket
import subprocess

adresse_ip_du_serveur = "192.168.1.193"
port_du_serveur = 12345

# Créer un socket TCP
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Associer le socket à l'adresse IP et au port spécifiés
server_socket.bind((adresse_ip_du_serveur, port_du_serveur))

# Écouter les connexions entrantes
server_socket.listen(1)
print(f"Serveur en écoute sur {adresse_ip_du_serveur}:{port_du_serveur}")

while True:
    # Accepter une connexion entrante
    client_socket, client_address = server_socket.accept()
    print(f"Connexion reçue de {client_address}")

    # Lire le message du client
    data = client_socket.recv(1024)
    message = data.decode("utf-8")
    print(f"Message reçu : {message}")

    if message == "EXECUTE_CUBE_PROGRAM":
        # Remplacez cette ligne par le chemin absolu vers votre programme cube.exe
        cube_program_path = "C:\\Users\\FUZELLIER\\OneDrive\\Bureau\\Solver_Cube\\cube.exe"
        subprocess.run([cube_program_path])
    elif message == "EXECUTE_CAMERA_PROGRAM":
        # Remplacez cette ligne par le chemin absolu vers votre programme camera.exe
        camera_program_path = "C:\\Users\\FUZELLIER\\OneDrive\\Bureau\\Solver_Cube\\testvideo.py"
        subprocess.run(["python", camera_program_path])
    else:
        # Écrire le message dans un fichier texte nommé "reception.txt"
        with open("Reception.txt", "w") as fichier:
            fichier.write(f"{message}\n")

    # Fermer la connexion avec le client
    client_socket.close()
